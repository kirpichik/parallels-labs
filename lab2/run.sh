#!/bin/sh

CLOCK_SPEED=$(lscpu | grep 'CPU MHz' | awk '{print $3}')
CLOCK_SPEED=$(expr "$CLOCK_SPEED*1000*1000")
CLOCK_SPEED=$(echo $CLOCK_SPEED | bc)

for size in 1 2 4 8 16 32
do
  echo "$size byte(s):"
  ./mem-test-$size $CLOCK_SPEED
  echo "================"
done

