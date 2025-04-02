#!/bin/bash

ROOT="$(dirname $(dirname $(realpath $0))..)"
SRC="${ROOT}/src"
INC="${ROOT}/include"

clean_obj() {
		cd ${SRC}
		rm -rf *.o *.obj *.lib *.exp *.pdb
}
clean_all() {
	clean_obj

	cd ${ROOT}
	rm -rf out
}

BUILD=
CC_PLT=
setup_args() {
	local ARG_VAL=${1}
	local CHANGED=0
	local VALUE=0
	local VALID=0

	if [[ "${ARG_VAL}" == "" ]]; then
		return 0
	fi

	if [[ "${ARG_VAL}" == "cproc" ]]; then
		if [[ ${CC_PLT} == "" ]]; then
			CC_PLT=cproc
			CHANGED=1
			return 0
		fi

		echo "Compiler Already Chosen!"
		echo "Please do not add another compiler to the arguments!"
		exit 0
	fi

	if [[ "${ARG_VAL}" == "tcc" ]]; then
		echo "WARNING:"
		echo "As of tcc 0.9.27, it does not compile..."
		echo "Maybe a community port or an updated version may work."
		echo

		if [[ ${CC_PLT} == "" ]]; then
			CC_PLT=tcc
			CHANGED=1
			return 0
		fi

		echo "Compiler Already Chosen!"
		echo "Please do not add another compiler to the arguments!"
		exit 0
	fi

	if [[ "${ARG_VAL}" == *"c"* ]]; then
		clean_all
		echo "Project has been cleaned!"
		exit 0
	fi

	if [[ "${ARG_VAL}" == *"a"* ]]; then
		if [[ ! ${BUILD} == "7" ]]; then
			BUILD=7
			CHANGED=1
		fi
		VALID=1
	fi

	if [[ "${ARG_VAL}" == *"d"* ]]; then
		VALUE=$(( BUILD & 1 ))
		if [[ ! "${VALUE}" == "1" ]]; then
			BUILD=$(( BUILD | 1 ))
			CHANGED=1
		fi
		VALID=1
	fi

	if [[ "${ARG_VAL}" == *"r"* ]]; then
		VALUE=$(( BUILD & 2 ))
		if [[ ! "${VALUE}" == "2" ]]; then
			BUILD=$(( BUILD | 2 ))
			CHANGED=1
		fi
		VALID=1
	fi

	if [[ "${ARG_VAL}" == *"x"* ]]; then
		VALUE=$(( BUILD & 4 ))
		if [[ ! "${VALUE}" == "4" ]]; then
			BUILD=$(( BUILD | 4 ))
			CHANGED=1
		fi
		VALID=1
	fi

	if [[ ! "${CHANGED}" == "0" ]]; then
		return 0;
	fi

	echo "Argument Error: ${ARG_VAL}"
	if [[ "${VALID}" == "1" ]]; then
		echo "Has been applied already! Please only apply once!"
		echo
	else
		echo "Invalid Argument!"
		echo
	fi

	echo "Valid Arguments:"
	echo "    - a        Build ALL configurations"
	echo "    - d        Include Debug Build"
	echo "    - r        Include Release Build"
	echo "    - x        Include Debug-Release Build"
	echo
	echo "    - cproc    Use the cproc compiler for the platform layer."
	echo "    - tcc      Use the tcc compiler for the platform layer"
	echo "               WARNING: As of 0.9.27, it does not compile..."
	echo
	echo "example #1: build.sh dx"
	echo "    - Builds in Debug and Debug-Release Configuration"
	echo "example #2: build.sh a"
	echo "    - Builds ALL configurations"
	echo "example #3: build.sh rd cproc"
	echo "    - Builds in Release and Debug - Order doesn't matter"
	echo "      for configurations and compilers (cproc, tcc)."
	echo "      eg. (rd == dr), (cproc rd == dr cproc)"
	echo "example #3: build.sh c"
	echo "    - Cleans the project by removing all object files from src"
	echo "      and removing the out directory. Cannot be used in"
	echo "      conjunction with drx. c will override!"
	exit 1
}

setup_args ${1}
setup_args ${2}
setup_args ${3}
setup_args ${4}
if [[ "${BUILD}" == "" ]]; then
	echo "No build specified: Building Debug Only"
	BUILD=1
fi

if [[ "${CC}" == "" ]]; then
	echo "No CC Environment Variable Detected. Defaulting to cc"
	echo "NOTE: CC is still required for shared library compilation."
	CC=cc
fi

if [[ "${CC_PLT}" == "" ]]; then
	CC_PLT=${CC}
fi

OUT="${ROOT}/out"
if [[ ! -d ${OUT} ]]; then
	mkdir ${OUT}
fi

OUT_CC=${OUT}/${CC_PLT}
if [[ ! -d ${OUT_CC} ]]; then
	mkdir ${OUT_CC}
fi

OUT_DBG=${OUT_CC}/debug
OUT_REL=${OUT_CC}/release
OUT_DBR=${OUT_CC}/debug-release

cd "${SRC}"
fasm entry-x86_64.fasm -d PLATFORM=LINUX

COMPILE=$(( BUILD & 1 ))
if [[ "${COMPILE}" == "1" ]]; then
	echo "//////////////////////////"
	echo "// Configuration: Debug //"
	echo "//////////////////////////"
	if [[ ! -d ${OUT_DBG} ]]; then
		mkdir ${OUT_DBG}
	fi

	echo "Platform Compilation Test: Debug"
	$CC_PLT entry-x86_64-linux.c entry-x86_64.o -nostdlib \
		    -Wall -Wno-unused-function -O0    -g \
		    -D_DEBUG \
		    -I "${INC}" \
		    -o "${OUT_DBG}/template-compile-only"
	if [[ ! ${?} == "0" ]]; then
		exit ${?}
	fi

	echo "Platform Compilation: Debug"
	$CC entry-x86_64-linux.c entry-x86_64.o -nostdlib \
		-Wall -Wno-unused-function -O0    -g \
		-D_DEBUG -DUSING_LIBC -lc \
		-I "${INC}" \
		-o "${OUT_DBG}/template"
	if [[ ! ${?} == "0" ]]; then
		exit ${?}
	fi

	echo "Application Compilation: Debug"
	$CC ./app/libapp.c -shared -fPIC -nostdlib -fno-builtin \
		-Wall -Wno-unused-function -O0    -g \
		-D_DEBUG \
		-I "${INC}" \
		-o "${OUT_DBG}/libapp.so"
	if [[ ! ${?} == "0" ]]; then
		exit ${?}
	fi

	echo
fi

COMPILE=$(( BUILD & 2 ))
if [[ "${COMPILE}" == "2" ]]; then
	echo "////////////////////////////"
	echo "// Configuration: Release //"
	echo "////////////////////////////"

	if [[ ! -d ${OUT_REL} ]]; then
		mkdir ${OUT_REL}
	fi

	echo "Platform Compilation Test: Release"
	$CC_PLT entry-x86_64-linux.c entry-x86_64.o -nostdlib \
		    -Wall -Wno-unused-function -Ofast \
		    -D_RELEASE \
		    -I "${INC}" \
		    -o "${OUT_REL}/template-compile-only"
	if [[ ! ${?} == "0" ]]; then
		exit ${?}
	fi

	echo "Platform Compilation: Release"
	$CC entry-x86_64-linux.c entry-x86_64.o -nostdlib \
		-Wall -Wno-unused-function -Ofast \
		-D_RELEASE -DUSING_LIBC -lc \
		-I "${INC}" \
		-o "${OUT_REL}/template"
	if [[ ! ${?} == "0" ]]; then
		exit ${?}
	fi

	echo "Application Compilation: Release"
	$CC ./app/libapp.c -shared -fPIC -nostdlib -fno-builtin \
		-Wall -Wno-unused-function -Ofast \
		-D_RELEASE \
		-I "${INC}" \
		-o "${OUT_REL}/libapp.so"
	if [[ ! ${?} == "0" ]]; then
		exit ${?}
	fi

	echo
fi

COMPILE=$(( BUILD & 4 ))
if [[ "${COMPILE}" == "4" ]]; then
	echo "//////////////////////////////////"
	echo "// Configuration: Debug-Release //"
	echo "//////////////////////////////////"
	if [[ ! -d ${OUT_DBR} ]]; then
		mkdir ${OUT_DBR}
	fi

	echo "Platform Compilation Test: Debug-Release"
	$CC_PLT entry-x86_64-linux.c entry-x86_64.o -nostdlib \
		    -Wall -Wno-unused-function -Ofast -g \
		    -D_DEBUG -D_RELEASE \
		    -I "${INC}" \
		    -o "${OUT_DBR}/template-compile-only"
	if [[ ! ${?} == "0" ]]; then
		exit ${?}
	fi

	echo "Platform Compilation: Debug-Release"
	$CC entry-x86_64-linux.c entry-x86_64.o -nostdlib \
		-Wall -Wno-unused-function -Ofast \
		-D_DEBUG -D_RELEASE -DUSING_LIBC -lc -g \
		-I "${INC}" \
		-o "${OUT_DBR}/template"
	if [[ ! ${?} == "0" ]]; then
		exit ${?}
	fi

	echo "Application Compilation: Debug-Release"
	$CC ./app/libapp.c -shared -fPIC -nostdlib -fno-builtin \
		-Wall -Wno-unused-function -Ofast -g \
		-D_DEBUG -D_RELEASE \
		-I "${INC}" \
		-o "${OUT_DBR}/libapp.so"
	if [[ ! ${?} == "0" ]]; then
		exit ${?}
	fi

	clean_obj
	echo

fi

