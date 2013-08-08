# -*- coding: utf-8 -*-
import sys
import os
import urllib
import shutil
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

def transformStringWithEncoding(str):
  return urllib.unquote(str.decode("latin-1").encode("utf-8"))

def transformString(s):
  unquoted = urllib.unquote(str(s));
  for i in u"\"',/\\@#$%^&*()!~`«»":
    unquoted = unquoted.replace(i, "_")
  unquoted = unquoted.strip("_")
  return unicode(unquoted.lower())

def imageExist(fileName):
  global imageFiles
  global imageSet
  unquotedName = transformString(fileName)
  if unquotedName in imageFiles:
    imageSet.add(unquotedName)
    return True

  return False


def rewriteImages(soup):
  imgTag = soup.findAll("img");

  for imgElement in imgTag:
    del imgElement["alt"]
    #todo rewrite srcset attr if we can get callback on image loading in webview
    del imgElement["srcset"]
    
    index = -1
    srcPath = imgElement["src"]
    splitedSrc = srcPath.split("/")
    if imageExist(splitedSrc[-1]):
      imgElement['src'] = "images/" + transformString(splitedSrc[-1])
    elif imageExist(splitedSrc[-2]):
      imgElement['src'] = "images/" + transformString(splitedSrc[-2])
    else:
      [s.decompose() for s in imgElement.fetchParents("div", {"class" : ["thumb tright", "thumbinner", "image"]})]

def rewriteCrossLinks(soup):
  global idMapping
  global redirectMapping
  links = soup.findAll("a")
 
  for link in links:
    destTitle = link["href"].split("/",2)[-1]
    destTitle = transformStringWithEncoding(destTitle)
    destTitle = redirectMapping.get(destTitle, destTitle);

    if destTitle in idMapping:
      link["href"] = idMapping.get(destTitle, link["href"]) + ".html"
      continue

    if "/wiki/File:" in link["href"] and "http" not in link["href"] and "www" not in link["href"]:
      imgElement = link.find("img")
      if imgElement:
        link["href"] = imgElement["src"]
        continue
  
    if "/wiki/" in link["href"]:
      if link.string:
        link.replace_with(link.string)
      else:
        link.replace_with("")

def writeHtml(content, fileName):
  global outDir
  open(os.path.join(outDir, fileName + ".html"), "w").write(content.prettify().encode('utf-8'))

##############################################################################
if len(sys.argv) < 8:
  print "Usage: " + sys.argv[0] + " <directory with html articles> <images directory> <article set info file> <redirect info file> <output directory> <threadIndex> <cpu core count>"
  exit(1)

inDir = sys.argv[1]
imagesSrcDir = sys.argv[2]
imageFiles = dict([(transformString(file), file) for file in os.listdir(imagesSrcDir)])
idMapping = dict([(unicode(i.split("\t")[1]), unicode(i.split("\t")[0])) for i in open(sys.argv[3])])
redirectMapping = dict([(unicode(line.split("\t")[1]), unicode(line.split("\t")[3].strip())) for line in open(sys.argv[4])])
outDir = sys.argv[5]
threadIndex = int(sys.argv[6])
coreCount = int(sys.argv[7])
files = [urllib.unquote(file) for file in idMapping.values()]
thisFiles = files[threadIndex * len(files) / coreCount : (threadIndex + 1) * len(files) / coreCount]
imageSet = set()

if not os.path.exists(outDir):
	os.makedirs(outDir)

for file in thisFiles:
	soup = BeautifulSoup(open(os.path.join(inDir, file)))
	soup = cleanUp(soup)
	rewriteImages(soup)
	rewriteCrossLinks(soup)
	writeHtml(soup, file)
imagesDstDir = os.path.join(outDir, "images")
if not os.path.exists(imagesDstDir):
	os.makedirs(imagesDstDir)

for image in imageSet:
	shutil.copy2(os.path.join(imagesSrcDir, imageFiles[image]), os.path.join(imagesDstDir, image))