
for interval in 0 1 2 3 4 5 6 7;
  do python strip.py $1 $2 $3 $4 $5 $interval 8 &
  done
wait
  