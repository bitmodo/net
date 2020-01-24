#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

if [[ ! -d build ]]; then
    meson setup -Dcriterion:samples=false -Dcriterion:tests=false "${DIR}/build"
fi

ninja -C "${DIR}/build" $@
