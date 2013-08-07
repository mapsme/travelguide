import sys
import os
import urllib
from bs4 import BeautifulSoup

reload(sys)
sys.setdefaultencoding('utf-8')

def cleanUp(soup):
  content = soup.find("div", {"id": "content"})

  # remove all specified tags
  [s.decompose() for s in content(['noscript'])]

  [s.decompose() for s in content.findAll("a", {"id": "mw-mf-last-modified"})]
  [s.decompose() for s in content.findAll("span", {"class": "mw-editsection"})]
  [s.decompose() for s in content.findAll("table", {"class": "articleState"})]
  [s.decompose() for s in content.findAll("button", {"class": "languageSelector"})]
  [s.decompose() for s in content.findAll("a", {"class": "section_anchors"})]
  [s.decompose() for s in content.findAll("div", {"id": "mw-mf-language-section"})]

  # delete empty sections
  sections = content.findAll("div", {"class": "section"})
  for section in sections:
    if section.div.string:
      section.decompose()

  # Wrap content with our own header and body, and restore original div structure for css
  divContentWrapper = soup.new_tag("div", id="content_wrapper")
  divContentWrapper["class"] = "show"
  content = content.wrap(divContentWrapper)
  content = content.wrap(soup.new_tag("div", id="mw-mf-page-center"))
  content = content.wrap(soup.new_tag("div", id="mw-mf-viewport"))
  bodyTag = soup.new_tag("body")
  bodyTag["class"] = "mediawiki ltr sitedir-ltr mobile stable skin-mobile action-view"
  content = content.wrap(bodyTag)
  content = content.wrap(soup.new_tag("html", lang="en", dir="ltr"))
  # Here we add our own js and css into the <head>
  headTag = soup.new_tag("head")
  headTag.append(soup.new_tag("meta", charset="UTF-8"))
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

  return content

def imageExist(fileName):
  global imageFiles
  return urllib.unquote(fileName).lower() in imageFiles


def rewriteImages(soup):
  imgTag = soup.findAll("img");

  for imgElement in imgTag:
    del imgElement['alt']
    #todo rewrite srcset attr if we can get callback on image loading in webview
    del imgElement['srcset']

    index = -1
    if "thumb" in imgElement['src'] and not '.pdf' in imgElement['src'].split("/")[-2]:
      index = -2
    imageName = imgElement['src'].split("/")[index]
    if imageExist(imageName):
      imgElement['src'] = "images/" + imageName
    else:
      [s.decompose() for s in imgElement.fetchParents("div", {"class" : "thumb tright"})]

def writeHtml(content, fileName):
  global outDir
  open(os.path.join(outDir, fileName), "w").write(content.prettify().encode('utf-8'))

##############################################################################
if len(sys.argv) < 6:
  print "Usage: " + sys.argv[0] + " <directory with html articles> <images directory> <output directory> <threadIndex> <cpu core count>"
  exit(1)

inDir = sys.argv[1]
imageFiles = [unicode((urllib.unquote(file)).lower()) for file in os.listdir(sys.argv[2])]
outDir = sys.argv[3]
threadIndex = int(sys.argv[4])
coreCount = int(sys.argv[5])
files = [urllib.unquote(file) for file in os.listdir(sys.argv[1])]
thisFiles = files[threadIndex * len(files) / coreCount : (threadIndex + 1) * len(files) / coreCount]

for file in thisFiles:
  soup = BeautifulSoup(open(os.path.join(inDir, file)))
  soup = cleanUp(soup)
  rewriteImages(soup)
  writeHtml(soup, file)