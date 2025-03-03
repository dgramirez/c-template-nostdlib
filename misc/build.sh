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
$CC main-linux.c entry-x86_64.o -nostdlib \
	-Wall -Wno-unused-function -O0    -g \
	-D_DEBUG \
	-I "${INC}" \
	-o "${OUT}/template"

$CC libapp.c -shared -fPIC -nostdlib -fno-builtin \
	-Wall -Wno-unused-function -O0    -g \
	-I "${INC}" \
	-o "${OUT}/libapp.so"

$CC libapp_x11.c -shared -fPIC -nostdlib -gdwarf-4 -fno-builtin \
	-Wall -Wno-unused-function -O0    -g \
	-lX11 \
	-I "${INC}" \
	-o "${OUT}/libapp_x11.so"

rm -rf *.o

echo $ROOT

