# -*- coding: utf-8 -*-
import sys
import os
import shutil
import unicodedata
import urllib
import pprint
import json
import hashlib
try:
    from bs4 import BeautifulSoup
except ImportError:
    from BeautifulSoup import BeautifulSoup

reload(sys)
sys.setdefaultencoding('utf-8')
"""
page_address = {
    "countrycodes": "CH",
    "country": "Switzerland",
    "city": "Zurich"
}
soup = BeautifulSoup(open("Zurich.html"))
"""

page_address = {
    "countrycodes": "GB",
    "country": "United Kingdom",
    "city": "London"
}
soup = BeautifulSoup(open("London.html"))

page_address = {
    "countrycodes": "RU",
    "country": "Russia",
    "city": "Murmansk"
}
soup = BeautifulSoup(open("Murmansk.html"))

page_address = {
    "countrycodes": "JP",
    "country": "Japan",
    "city": "Tokyo"
}
soup = BeautifulSoup(open("Tokyo.html"))

def nominatim_geocode(address):
    try:
        os.makedirs("nominatim")
    except OSError:
        pass
    key = os.path.join("nominatim", hashlib.md5(json.dumps(address)).hexdigest())
    if os.path.exists(key):
        val = open(key).read()
    else:
        val = urllib.urlopen("http://open.mapquestapi.com/nominatim/v1/search.php?format=json&accept-language=en" + urllib.urlencode(address)).read()
        open(key, "w").write(val)
    try:
        val = json.loads(val)
    except ValueError:
        val = ""
    return val

def nominatim_bbox_transform(bbox):
    return [float(bbox[2]), float(bbox[0]), float(bbox[3]), float(bbox[1])]

def geocode_bbox(string, city_bbox, place_params):
    place_params["q"] = string
    is_house = any(char.isdigit() for char in str(string))
    try:
        for place in nominatim_geocode(place_params):
            if is_house and place["class"] == "highway":
                continue
            #print city_bbox
            if not(float(place["lon"]) < city_bbox[0] or float(place["lon"]) > city_bbox[2] or float(place["lat"]) < city_bbox[1] or float(place["lat"]) > city_bbox[3]):
                print place
                break
        else:
            raise IndexError
 #       print >> osm, '<node id="%s" lat="%s" lon="%s" version="1"><tag k="name" v="%s" /></node>'%(id, place["lat"], place["lon"], i)
    except IndexError:
        return False
    return (float(place["lon"]), float(place["lat"]))

#pprint.pprint(texts_to_geocode)
#print len(texts_to_geocode)

city_geocode = nominatim_geocode(page_address)[0]
city_bbox = nominatim_bbox_transform(city_geocode["boundingbox"])
place_params = {"viewbox": ",".join([str(x) for x in city_bbox]), "countrycodes": page_address["countrycodes"]}
texts_to_geocode = set()

for s in soup.findAll("b") + soup.findAll("span", "label listing-address"):
    coord = geocode_bbox(s.getText(), city_bbox, place_params)
    if coord:
        print s.getText(), coord
        hrefLink = u"mapswithme://map?v=1&ll=%s,%s" % (coord[1], coord[0])
        hrefLink = "http://www.openstreetmap.org/?mlat=%s&mlon=%s#map=19/%s/%s" % (coord[1], coord[0], coord[1], coord[0])
        mapTag = soup.new_tag("a", href=hrefLink)
        mapTag["class"] = "geolink"
        mapTag.string = "[map]"
        s.append(mapTag)
      #  s["style"] = "color: red;"
    else:
        print s.getText(), "BAD!!!"

open('out.html', 'w').write(str(soup))


osm = open("file.osm", "w")
print >> osm, '<osm version="0.6">'
id = 0
bad_cnt = 0
for i in texts_to_geocode:
    id += 1
    place_params["q"] = i
    try:
        for place in nominatim_geocode(place_params):
            print place
            print city_bbox
            if not(float(place["lon"]) < city_bbox[0] or float(place["lon"]) > city_bbox[2] or float(place["lat"]) < city_bbox[1] or float(place["lat"]) > city_bbox[3]):
                break
        else:
            raise IndexError
        print >> osm, '<node id="%s" lat="%s" lon="%s" version="1"><tag k="name" v="%s" /></node>'%(id, place["lat"], place["lon"], i)

        #print >> osm, '<node id="%s" lat="%s" lon="%s" version="1"><tag k="name" v="%s" /></node>'%(id, place_params["viewbox"].split(",")[0], place_params["viewbox"].split(",")[1], i)
        #id += 1
        #print >> osm, '<node id="%s" lat="%s" lon="%s" version="1"><tag k="name" v="%s" /></node>'%(id, place_params["viewbox"].split(",")[2], place_params["viewbox"].split(",")[3], i)
    except IndexError:
        bad_cnt += 1
        print bad_cnt, i

print >> osm, '</osm>'
