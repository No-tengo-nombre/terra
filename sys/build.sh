#!/bin/bash

# DISCLAIMER: Tools are meant to be run from the root directory of the project, not from the
#             `sys` directory.

params=""
example=""
platform=""
generator=""
force_generator=""
cores=""
release_mode=0
examples=0
testing=0
verbose=0
python_install=0

while getopts "hcxvisetrdG:R:P:j:" arg
do
    case "$arg" in
        c)
            sys/clean.sh
        ;;
        x)
            params+=" -DCMAKE_EXPORT_COMPILE_COMMANDS=1"
        ;;
        i)
            params+=" -DTERRA_INSTALL=ON"
        ;;
        s)
            params+=" -DBUILD_SHARED_LIBS=OFF"
        ;;
        h)
            params+=" -DBUILD_SHARED_LIBS=ON"
        ;;
        e)
            examples=1
        ;;
        t)
            testing=1
        ;;
        r)
            release_mode=1
        ;;
        d)
            release_mode=0
        ;;
        v)
            verbose=1
        ;;
        G)
            force_generator="${OPTARG}"
        ;;
        R)
            example="${OPTARG}"
        ;;
        P)
            platform="${OPTARG}"
        ;;
        j)
            cores="${OPTARG}"
        ;;
    esac
done

if [ $testing -eq 1 ]; then
    params+=" -DTERRA_BUILD_TESTS=ON"
else
    params+=" -DTERRA_BUILD_TESTS=OFF"
fi
if [ $examples -eq 1 ]; then
    params+=" -DTERRA_BUILD_EXAMPLES=ON"
else
    params+=" -DTERRA_BUILD_EXAMPLES=OFF"
fi

if [ $release_mode -eq "1" ]; then
    params+=" -DCMAKE_BUILD_TYPE=Release"
else
    params+=" -DCMAKE_BUILD_TYPE=Debug"
fi

if [[ "${force_generator}" != "" ]]; then
    generator=$force_generator
fi

echo -e "\n\nBuilding with parameters \x1b[32;20m'${params}'\x1b[0m"
if [[ "${generator}" == "" ]]; then
    cmake -S . -B ./build/ $params
else
    cmake -S . -B ./build/ -G "${generator}" $params
fi

if [ $release_mode -eq 1 ]; then
    cmake --build ./build/ --config Release -j"${cores}"
    build_type="Release"
else
    cmake --build ./build/ -j"${cores}"
    build_type="Debug"
fi

if [ $testing -eq 1 ]; then
    echo -e "\n\n\x1b[32;20mRunning tests\x1b[0m"
    if [ $verbose -eq 1 ]; then
        sys/test.sh -C $build_type -v
    else
        sys/test.sh -C $build_type
    fi
fi

if [[ "${example}" != "" ]]; then
    echo -e "\n\nRunning example '\x1b[32;20m${example}\x1b[0m'"
    if [ $release_mode -eq 1 ]; then
        ./bin/ex.${example}/ex.${example}
    else
        ./bin/debug/ex.${example}/ex.${example}
    fi
fi
