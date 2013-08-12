#!/bin/bash

if test "$1" == "" ; then
    echo "USAGE: $0 [thumbsInDir] [imagesInDir] [outDir]"
    exit
fi

mkdir -p $3

mkdir -p $3/thumb
pushd $1

    for i in *.png; do convert $i -auto-orient -quality 53 -thumbnail '256x256>' $3/thumb/$(basename -s.png $i).jpg; echo $i; done
    for i in *.svg; do convert $i -auto-orient -quality 53 -thumbnail '256x256>' $3/thumb/$(basename -s.svg $i).jpg; echo $i; done
    for i in *.jpg *.JPG *.jpeg; do convert  -define jpeg:size=400x280 $i -auto-orient -quality 53 -thumbnail '500x280>' -strip -liquid-rescale '256x256!>' $3/thumb/$i; echo $i; done
popd

mkdir -p $3/images
pushd $2
    for i in *.jpg; do convert $i -auto-orient -quality 53 -strip -thumbnail '2048x1536>' $3/images/$i; echo $i; done
    for i in *.png; do convert $i -auto-orient -quality 99 -strip -thumbnail '4000x3000>' PNG8:$3/images/$i; echo $i; done
    cp *.svg $3/images/
popd

