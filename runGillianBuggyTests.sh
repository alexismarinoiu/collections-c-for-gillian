#!/bin/bash

set -e

./clean.sh

TESTS_DIR="tests-with-bugs"
LIB_HEADERS_DIR="lib-with-bugs/include"
LIB_SRC_DIR="lib-with-bugs"
UTILS_HEADERS_DIR="test-utils"
UTILS_SRC_DIR="test-utils"

echo "Running buggy tests using Gillian-C..."
gillian-c bulk-wpst ${TESTS_DIR} -I ${LIB_HEADERS_DIR} -I ${UTILS_HEADERS_DIR} \
    -S ${LIB_SRC_DIR} -S ${UTILS_SRC_DIR} --ignore-undef
