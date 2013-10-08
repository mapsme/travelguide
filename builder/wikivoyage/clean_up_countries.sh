#!/bin/bash

set -e -u

cat countries_to_generate.txt | while read REGION
do
    rm $REGION.info.txt.tmp || true
    touch $REGION.info.txt.tmp

    cat $REGION.info.txt | while read LINE
    do
	ARTICLE=$(echo "$LINE" | cut -f1)
	if [ -f "articles/$ARTICLE" ]
	then
	    echo "${LINE}" >> $REGION.info.txt.tmp
	else
	    echo "Article $ARTICLE not found for $REGION"
	fi
    done
    # now we are ready to use new *.info.txt file
    mv $REGION.info.txt.tmp $REGION.info.txt
done
# for make, if we succeed
touch clean_up_countries
