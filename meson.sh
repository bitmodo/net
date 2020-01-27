#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

if [[ ! -d build ]]; then
    meson setup "${DIR}/build"
fi

ninja -C "${DIR}/build" $@
