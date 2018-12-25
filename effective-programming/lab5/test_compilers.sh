#!/bin/bash

SOURCE_DEFAULT="5.c"
SOURCE_AUTO="5_2.c"
SOURCE_INTRIN="5_3_v1.c"
SOURCE_INTRIN_BEST="5_3.c"

LEVELS="3 fast"

GCC="gcc"
CLANG_CC=""

$GCC --version
if [ ! -z $CLANG_CC ]
then
  $CLANG_CC --version
fi

for level in $LEVELS
do
  echo "====== Testing level: O$level ======"

  echo "### GCC default test: ###"
  $GCC -O$level $SOURCE_DEFAULT -march=native -ftree-vectorize -fopt-info-vec -o compiler_default_O${level}_gcc
  ./compiler_default_O${level}_gcc

  echo "### GCC auto vectorisation test: ###"
  $GCC -O$level $SOURCE_AUTO -march=native -ftree-vectorize -fopt-info-vec -fopenmp -o compiler_auto_O${level}_gcc
  ./compiler_auto_O${level}_gcc

  echo "### GCC intrinsics test: ###"
  $GCC -O$level -std=c11 $SOURCE_INTRIN -march=native -o compiler_intrinsics_O${level}_gcc
  ./compiler_intrinsics_O${level}_gcc

  echo "### GCC intrinsics best test: ###"
  $GCC -O$level -std=c11 $SOURCE_INTRIN_BEST -march=native -o compiler_intrinsics_O${level}_best_gcc
  ./compiler_intrinsics_O${level}_best_gcc

done

echo "Press enter for cleanup or Ctrl-C to interrupt"
read

rm -f compiler_default_O* compiler_auto_O* compiler_intrinsics_O*
