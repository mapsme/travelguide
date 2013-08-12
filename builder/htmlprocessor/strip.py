# -*- coding: utf-8 -*-
import sys
import os
import urllib
import shutil
import unicodedata
try:
    from bs4 import BeautifulSoup
except ImportError:
    import BeautifulSoup

reload(sys)
sys.setdefaultencoding('utf-8')

from strip_function import cleanUp


def insertMapLink(soup, lat, lon, title, pageId):
    hrefLink = "mapswithme://map?v=1&ll=%s,%s&n=%s&id=%s&backurl=guideswithme&appname=Guides%%20With%%20Me" % (lat, lon, urllib.quote(title), pageId)
    mapTag = BeautifulSoup().new_tag("a", href=hrefLink)
    mapTag["class"] = "geolink"
    soup.body.insert(0, mapTag)


def insertArticleTitle(soup, articleTitle):
    titleTag = BeautifulSoup().new_tag("div")
    titleTag["class"] = "articleHeader"
    titleTag.append(articleTitle)
    soup.body.insert(0, titleTag)


def insertArticleImage(soup, imagePath):
    imgTag = BeautifulSoup().new_tag("img", src=imagePath)
    imgTag["class"] = "articleImage"
    soup.body.insert(0, imgTag)


def insertBreadcrumb(soup, articleTitle, parentTitle, parentLink, grandParentTitle, grandParentLink):
    tagFactory = BeautifulSoup()

    bcWrapper = tagFactory.new_tag("div")
    bcWrapper["class"] = "breadcrumbs_wrapper"
    if (grandParentTitle):
        grandParentTag = tagFactory.new_tag("a", href=grandParentLink)
        grandParentTag["class"] = "breadcrumb bc1"
        grandParentTag.append(grandParentTitle)
        bcWrapper.append(grandParentTag)
    if (parentTitle):
        parentTag = tagFactory.new_tag("a", href=parentLink)
        parentTag["class"] = "breadcrumb bc2"
        parentTag.append(parentTitle)
        bcWrapper.append(parentTag)
    currTag = tagFactory.new_tag("span")
    currTag["class"] = "breadcrumb bc3"
    currTag.append(articleTitle)
    bcWrapper.append(currTag)
    soup.body.insert(0, bcWrapper)


def transformStringWithEncoding(string):
    return urllib.unquote(string.decode("latin-1").encode("utf-8"))

def sanitizeFileName(s):
    # unquote %20 and like that
    s = urllib.unquote(str(s))

    # remove punctuation
    for i in u"\"',/\\@#$%^&*()!~`«»":
        s = s.replace(i, "_")
    s = s.strip("_")

    # normalize unicode to NFKD (form with modifiers separated)
    s = unicodedata.normalize("NFKD", s)

    # drop modifiers from string (no diacritics)
    s = u"".join(x for x in s if not unicodedata.category(x).startswith("M"))

    # lowercase
    s = s.lower()
    return s


def imageSanitizedPath(fileName):
    """
    return path to image file if it's in dataset
    """
    global imageFiles
    global imageSet
    unquotedName = sanitizeFileName(fileName)
    if unquotedName in imageFiles:
        imageSet.add(unquotedName)
        return 'images/' + unquotedName


def rewriteImages(soup):
    imgTag = soup.findAll("img")

    for imgElement in imgTag:
        del imgElement["alt"]
        # todo rewrite srcset attr if we can get callback on image loading in webview
        del imgElement["srcset"]

        index = -1
        splitSrc = imgElement["src"].split("/")
        splitSrc.reserse()
        # checking just two last elements (preview name, real name)
        for fileName in splitSrc[:2]:
            fileName = imageSanitizedPath(fileName)
            if fileName:
                imgElement['src'] = fileName
                break
            else:
                print "Stripping image", imgElement["src"]
                [s.decompose() for s in imgElement.fetchParents("div", {"class": ["thumb tright", "thumbinner", "image"]})]


def rewriteCrossLinks(soup):
    global idMapping
    global redirectMapping
    links = soup.findAll("a")

    for link in links:
        destTitle = link["href"].split("/", 2)[-1]
        destTitle = transformStringWithEncoding(destTitle)
        destTitle = redirectMapping.get(destTitle, destTitle)

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
    open(os.path.join(outDir, fileName + ".html"), "w").write(content.encode('utf-8'))


def fixTitle(title):
    return title.split('/')[-1].replace('_', ' ')

if len(sys.argv) < 9:
    print "Usage: " + sys.argv[0] + " <directory with html articles> <images directory> <article set info file> <redirect info file> <geocoords file> <output directory> <threadIndex> <cpu core count>"
    exit(1)

inDir = sys.argv[1]

imagesSrcDir = sys.argv[2]
imageFiles = dict([(sanitizeFileName(file), file) for file in os.listdir(imagesSrcDir)])

idMappingFile = sys.argv[3]
idMapping = dict([(unicode(i.split("\t")[1]), unicode(i.split("\t")[0])) for i in open(idMappingFile)])

# pageId => [parentId, parentTitle, grandParentId, grandParentTitle], ids and titles can be "NULL"
ancestors = dict([(i.split("\t")[0], i.strip().split("\t")[4:8]) for i in open(sys.argv[3])])

redirectMapping = dict([(unicode(line.split("\t")[1]), unicode(line.split("\t")[3].strip())) for line in open(sys.argv[4])])

coords = dict([(line.split("\t")[0], (line.split("\t")[1], line.split("\t")[2])) for line in open(sys.argv[5])])

pageIdToTitle = {v:fixTitle(str(k)) for k, v in idMapping.iteritems()}

outDir = sys.argv[6]
threadIndex = int(sys.argv[7])
coreCount = int(sys.argv[8])
files = [urllib.unquote(file) for file in idMapping.values()]

thisFiles = files[threadIndex * len(files) / coreCount: (threadIndex + 1) * len(files) / coreCount]
imageSet = set()

# preload coords

if not os.path.exists(outDir):
    os.makedirs(outDir)

for file in thisFiles:
    soup = BeautifulSoup(open(os.path.join(inDir, file)))
    soup = cleanUp(soup)
    rewriteImages(soup)
    rewriteCrossLinks(soup)
    # insert article "header" - image with breadcrumbs and map link
    if file in coords:
        articleTitle = pageIdToTitle[file]

        insertMapLink(soup, coords[file][0], coords[file][1], articleTitle, file)

        insertArticleTitle(soup, articleTitle)

        parentTitle = fixTitle(ancestors[file][1]) if ancestors[file][1] != "NULL" else False
        parentLink = ancestors[file][0] + ".html" if ancestors[file][0] != "NULL" else False
        grandParentTitle = fixTitle(ancestors[file][3]) if ancestors[file][3] != "NULL" else False
        grandParentLink = ancestors[file][2] + ".html" if ancestors[file][2] != "NULL" else False
        insertBreadcrumb(soup, articleTitle, parentTitle, parentLink, grandParentTitle, grandParentLink)

        insertArticleImage(soup, "header_images/" + file + ".jpg")

    writeHtml(soup, file)

imagesDstDir = os.path.join(outDir, "images")
if not os.path.exists(imagesDstDir):
    os.makedirs(imagesDstDir)

for image in imageSet:
    shutil.copy2(os.path.join(imagesSrcDir, imageFiles[image]), os.path.join(imagesDstDir, image))
