import sys
import os
import json
import urllib


reload(sys)
sys.setdefaultencoding("utf-8")



if sys.argv[1] == '1':
    strings = [[l.replace("_"," ") for l in l.strip().split()] for l in open("geocodes_todo.txt")]
    #print strings
    for string in strings:
        print "wget 'http://nominatim.openstreetmap.org/search?"+ urllib.urlencode({'city':string[1], 'country':string[2], 'format': 'json'}) + "' -O dl/" + string[0] + ".json"

elif sys.argv[1] == '2':
    files = os.listdir('dl/')
    for file in files:
        try:
            c = json.loads(open(os.path.join('dl', file)).read())
            if c:
                print file.split('.')[0]+'\t'+ c[0]["lat"]+'\t'+c[0]["lon"]
        except ValueError:
            pass

elif sys.argv[1] == '3':
    strings = [[l.replace("_"," ").replace("/", " ").replace("(", " ").replace(")", " ") for l in l.strip().split()] for l in open("geocodes_todo.txt")]
    files = os.listdir('dl/')
    bad_ids = set()
    for file in files:
        try:
            c = json.loads(open(os.path.join('dl', file)).read())
            if c:
                continue
        except ValueError:
            pass
        bad_ids.add(file.split('.')[0])
    for string in strings:
        if string[0] in bad_ids:
            print "wget 'http://nominatim.openstreetmap.org/search?"+ urllib.urlencode({'q': string[1]+" "+string[2], 'format': 'json'}) + "' -O dl2/" + string[0] + ".json"
            #print string[0], string[1], string[2]
