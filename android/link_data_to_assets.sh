#!/bin/bash
set -x -u

SRC=../../data
DST=assets

rm -rf ${DST}/*

files=(thumbnails)

for item in ${files[*]}
do
  ln -s $SRC/$item $DST/$item
done
