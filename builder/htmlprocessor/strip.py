# Gets clean content from raw html

import sys

if len(sys.argv) == 1:
  print "Usage: " + sys.argv[0] + " <html article file> [optional output file]"
  exit(1)

from bs4 import BeautifulSoup

soup = BeautifulSoup(open(sys.argv[1]))
content = soup.find("div", {"id": "content"})

# remove all specified tags
[s.extract() for s in content(['noscript'])]

content.find("a", {"id": "mw-mf-last-modified"}).extract()
[s.extract() for s in content.findAll("span", {"class": "mw-editsection"})]
[s.extract() for s in content.findAll("table", {"class": "articleState"})] 

if len(sys.argv) == 3:
  open(sys.argv[2], "w").write(content.prettify().encode('utf-8'))
else:
  print(content.prettify().encode('utf-8'))
