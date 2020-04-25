#!/bin/bash

set -e

./clean.sh

ENABLE_STATS=$1

TESTS_DIR="for-gillian-with-imports"
LIB_HEADERS_DIR="lib/include"
LIB_SRC_DIR="lib"
UTILS_HEADERS_DIR="test-utils"
UTILS_SRC_DIR="test-utils"

echo "Running tests using Gillian-C..."
for filename in ${TESTS_DIR}/**/*.c; do
    [[ -f "${filename}" ]] || break
    echo ${filename}
    if [[ "${ENABLE_STATS}" = "--stats" ]]; then
        time gillian-c wpst ${filename} -I ${LIB_HEADERS_DIR} \
            -I ${UTILS_HEADERS_DIR} -S ${LIB_SRC_DIR} -S ${UTILS_SRC_DIR} \
            --ignore-undef -s --stats
    else
        time gillian-c wpst ${filename} -I ${LIB_HEADERS_DIR} \
            -I ${UTILS_HEADERS_DIR} -S ${LIB_SRC_DIR} -S ${UTILS_SRC_DIR} \
            --ignore-undef -s --parallel
    fi
done
