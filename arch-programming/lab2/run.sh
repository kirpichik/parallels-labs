#!/bin/sh

rm result.csv prog.c test

for i in $(seq 10 $1)
do
  cat prog.prefix > prog.c

  for j in $(seq 1 $i)
  do
    echo "__asm__(\"nop\");" >> prog.c
  done

  cat prog.suffix >> prog.c

  echo "===== Compiling $i... ====="
  gcc -O2 prog.c -o prog
  echo "===== Running $i =========="

  RESULT=$(./prog $2)
  echo $RESULT
  echo "$i,$RESULT" >> result.csv

  echo "==========================="
done

