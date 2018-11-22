#!/bin/bash

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]; then
  echo "Args: <nops-min> <nops-max> <repeats>"
  exit 1
fi

rm result.csv

for i in $(seq $1 $2)
do
  cat prog.prefix.c > prog.c

  for j in $(seq 1 $i)
  do
    echo "        __asm__(\"nop\");" >> prog.c
  done

  cat prog.suffix.c >> prog.c

  echo "===== Compiling $i... ====="
  gcc -O2 prog.c -o prog
  echo "===== Running $i =========="

  RESULT=$(./prog $3)
  echo $RESULT
  echo "$i,$RESULT" >> result.csv

  echo "==========================="
done

