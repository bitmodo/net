#!/bin/bash

COMMAND="$0"
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

usage() {
    echo "Usage: ${COMMAND} [valgrind] <build|test|coverage|setup> [options]"
    exit 1
}

reconfig=0
setup=0
valg=0

build=0
testing=0
coverage=0

if [[ ! -d "${DIR}/build" ]]; then
    reconfig=1
fi

while [ "$1" != "" ]; do
    case $1 in
        valgrind ) valg=1;;
        setup ) reconfig=1
                setup=1
                shift
                break;;
        build ) build=1
                shift
                break;;
        test ) testing=1
               shift
               break;;
        coverage ) coverage=1
                   shift
                   break;;
        * ) usage;;
    esac
    shift
done

if [ "$setup" = "0" ] && [ "$build" = "0" ] && [ "$testing" = "0" ] && [ "$coverage" = "0" ]; then
    usage
fi

if [ "$reconfig" = "1" ]; then
    if [[ -d "${DIR}/build" ]]; then
        rm -r "${DIR}/build"
    fi

    args="-Db_coverage=true"
    if [ "$valg" = "1" ] && [ "$testing" = "0" ]; then
        args="${args} -Dalways_valgrind=true"
    fi
    if [ "$setup" = "1" ]; then
        args="${args} $@"
    fi

    eval "meson setup ${args} ${DIR}/build"

    if [ "$setup" = "1" ]; then
        exit 0
    fi
fi

if [ "$testing" = "1" ]; then
    args="-C ${DIR}/build $@"
    if [ "$valg" = "1" ]; then
        args="${args} --setup valgrind"
    fi

    eval "meson test ${args}"
    exit 0
fi

if [ "$build" = "1" ]; then
    args="-C ${DIR}/build $@"

    eval "ninja ${args}"
    exit 0
fi

if [ "$coverage" = "1" ]; then
    args="-C ${DIR}/build $@"

    eval "ninja ${args} coverage"
    exit 0
fi
