#!/bin/bash

for interval in 0 1 2 3 4 5 6 7;
  do python `dirname $0`/strip.py $1 $2 $3 $4 $5 $6 $interval 8 $7 &
done
wait
