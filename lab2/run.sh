#!/bin/sh

OS=$(uname)
CLOCK_SPEED=$1

if [ $OS == "Linux" ]; then
  CLOCK_SPEED=$(lscpu | grep 'CPU MHz' | awk '{print $3}')
  CLOCK_SPEED=$(expr "$CLOCK_SPEED*1000*1000")
  CLOCK_SPEED=$(echo $CLOCK_SPEED | bc)
elif [ $OS == "Darwin" ]; then
  CLOCK_SPEED=$(sysctl -n hw.cpufrequency)
elif [ -z "$1" ]; then
  echo "Cannot identify clock speed, argument required."
  exit 1
fi

for size in 1 2 4 8 16 32
do
  echo "$size byte(s):"
  ./mem-test-$size $CLOCK_SPEED
  echo "================"
done

