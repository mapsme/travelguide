#!/bin/bash
set -x -u

SRC=../../data
DST=assets

rm -rf ${DST}/*

files=(thumbnails images styles GreatBritain.html Lancaster.html London.html)

for item in ${files[*]}
do
  ln -s $SRC/$item $DST/$item
done
