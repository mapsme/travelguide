DUMP_VERSION=latest
DUMP_URL_PREFIX="http://dumps.wikimedia.org/enwikivoyage/$(DUMP_VERSION)/enwikivoyage-$(DUMP_VERSION)"
DUMP_PAGE_URL=$(DUMP_URL_PREFIX)"-page.sql.gz"
DUMP_REDIRECT_URL=$(DUMP_URL_PREFIX)"-redirect.sql.gz"


HTML_ARTICLE_PREFIX="http://en.m.wikivoyage.org/wiki?curid="


DUMP_FILES = page.sql.gz redirect.sql.gz category.sql.gz page_props.sql.gz image.sql.gz site_stats.sql.gz interwiki.sql.gz \
             pagelinks.sql.gz imagelinks.sql.gz categorylinks.sql.gz langlinks.sql.gz externallinks.sql.gz templatelinks.sql.gz


.PHONY: all
all: download_images rename_articles countries.txt geocodes.txt

.PHONY: clean
clean:
	rm *.sql.gz || true

$(DUMP_FILES):
	wget $(DUMP_URL_PREFIX)"-"$@ -O $@

load_sql_dumps: $(DUMP_FILES)
	echo "CREATE DATABASE IF NOT EXISTS $(MYSQL_DATABASE)" | $(MYSQL_BINARY) --user=$(MYSQL_USER)
	for i in $?; do echo "Importing "$$i && pv $$i | gunzip | $(MYSQL_BINARY) --user=$(MYSQL_USER) --database=$(MYSQL_DATABASE); done
	touch load_sql_dumps

article_page_id.txt: load_sql_dumps
	$(MYSQL_BINARY) --user=$(MYSQL_USER) --database=$(MYSQL_DATABASE) --execute="SELECT page_id from page where page_namespace = 0" | tail +2 > article_page_id.txt

article_page_url.txt: article_page_id.txt
	cat article_page_id.txt | sed "s@^@$(HTML_ARTICLE_PREFIX)@" > article_page_url.txt

article_page_url_desktop.txt: article_page_url.txt
	cat article_page_url.txt | sed 's/[.]m[.]/./' > article_page_url_desktop.txt

download_articles: article_page_url.txt
	wget --wait=0.2 --random-wait --no-clobber --directory-prefix=articles --input-file=article_page_url.txt || true
	touch download_articles

download_articles_desktop: article_page_url_desktop.txt
	wget --wait=0.2 --random-wait --no-clobber --directory-prefix=articles_desktop --input-file=article_page_url_desktop.txt || true
	touch download_articles_desktop

image_url.txt: download_articles
	grep --only-matching --no-filename --mmap '<img[^/]*src=\"[^">]*"' -r articles/ | sed 's/<img.*src="//g' | sed 's/"$$//g' | sed 's:/thumb\(/.*\)/[0-9][0-9]*px-.*$$:\1:' | sed 's@^//@http://@' | sort -u > image_url.txt

image_url_desktop.txt: download_articles_desktop
	grep --only-matching --no-filename --mmap '<img[^/]*src=\"[^">]*"' -r articles_desktop/ | sed 's/<img.*src="//g' | sed 's/"$$//g' | sed 's:/thumb\(/.*\)/[0-9][0-9]*px-.*$$:\1:' | sed 's@^//@http://@' | sort -u > image_url_desktop.txt

download_images: image_url.txt image_url_desktop.txt
	wget --wait=0.2 --random-wait --no-clobber --directory-prefix=images --input-file=image_url.txt || true
	wget --wait=0.2 --random-wait --no-clobber --directory-prefix=images --input-file=image_url_desktop.txt || true
	touch download_images

rename_articles:
	for f in articles/*; do mv $$f $$(echo $$f | sed 's/wiki.curid=//g'); done
	touch rename_articles

countries.txt: load_sql_dumps
	$$BIN/generate_article_info.sh

geocodes_from_html.txt: download_articles
	grep '<span id="geodata" class="geo">[-0-9.]*; [-0-9.]*</span>' -R articles/ --only-matching | sed 's@articles//@@' | sed 's@:<span id=.geodata. class=.geo.>@	@' | sed 's@; @	@' | sed 's@</span>@@' | sort -u -b -k1 > geocodes_from_html.txt

geocodes_todo_all.txt: countries.txt
	rm geocodes_todo.txt || true
	touch geocodes_todo.txt
	for f in *.info.txt; do cat $$f | cut -f1,2 | sed "s/$$/	$$f/" | sed 's/.info.txt//' | sort -b -k1 >> geocodes_todo_all.txt; done

geocodes_todo.txt: geocodes_todo_all.txt geocodes_from_html.txt
	join -1 1 -2 1 -v1 geocodes_todo_all.txt geocodes_from_html.txt > geocodes_todo.txt

geocodes.txt: geocodes_from_html.txt geocodes_todo.txt
	cp geocodes_from_html.txt geocodes.txt
