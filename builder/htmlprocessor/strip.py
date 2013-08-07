import sys

if len(sys.argv) == 1:
  print "Usage: " + sys.argv[0] + " <html article file> [optional output file]"
  exit(1)

from bs4 import BeautifulSoup

soup = BeautifulSoup(open(sys.argv[1]))
content = soup.find("div", {"id": "content"})

# remove all specified tags
[s.decompose() for s in content(['noscript'])]

[s.decompose() for s in content.findAll("a", {"id": "mw-mf-last-modified"})]
[s.decompose() for s in content.findAll("span", {"class": "mw-editsection"})]
[s.decompose() for s in content.findAll("table", {"class": "articleState"})]
[s.decompose() for s in content.findAll("button", {"class": "languageSelector"})]
[s.decompose() for s in content.findAll("a", {"class": "section_anchors"})]
[s.decompose() for s in content.findAll("div", {"id": "mw-mf-language-section"})]

# Wrap content with our own header and body
content = content.wrap(soup.new_tag("body"))
content = content.wrap(soup.new_tag("html"))
# Here we add our own js and css into the <head>
headTag = soup.new_tag("head")
cType = soup.new_tag("meta", content="text/html; charset=UTF-8")
# workaround as we can't use dashes in python names
cType["http-equiv"] = "Content-Type"
headTag.append(cType)
headTag.append(soup.new_tag("link", rel="stylesheet", type="text/css", href="article.css"))
headTag.append(soup.new_tag("script", type="text/javascript", href="article.js"))
meta1 = soup.new_tag("meta", content="yes")
# workaround as "name" is used in python
meta1["name"] = "apple-mobile-web-app-capable"
headTag.append(meta1)
meta2 = soup.new_tag("meta", content="initial-scale=1.0, user-scalable=yes, minimum-scale=0.25, maximum-scale=1.6")
meta2["name"] = "viewport"
headTag.append(meta2)
content.body.insert_before(headTag)


if len(sys.argv) == 3:
  open(sys.argv[2], "w").write(content.prettify().encode('utf-8'))
else:
  print(content.prettify().encode('utf-8'))
