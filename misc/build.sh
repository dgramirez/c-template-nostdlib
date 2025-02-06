#!/bin/bash

ROOT="$(dirname $(dirname $(realpath $0))..)"
SRC="${ROOT}/src"
INC="${ROOT}/include"
OUT="${ROOT}/out"

if [[ "${CC}" == "" ]]; then
	echo "No CC Environment Variable Detected. Defaulting to cc"
	CC=cc
fi

if [[ ! -d ${OUT} ]]; then
	mkdir ${OUT}
fi

cd "${SRC}"
fasm entry-x86_64.fasm -d PLATFORM=LINUX
cc main-linux.c entry-x86_64.o -nostdlib \
	-Wall -Wno-unused-function -O0    -g \
	-I "${INC}" \
	-o "${OUT}/template"

rm -rf *.o

echo $ROOT

