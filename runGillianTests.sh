#!/bin/bash

set -e

./clean.sh

TESTS_DIR="for-gillian-with-imports"
LIB_HEADERS_DIR="lib/include"
LIB_SRC_DIR="lib"
UTILS_HEADERS_DIR="test-utils"
UTILS_SRC_DIR="test-utils"

echo "Running tests using Gillian-C..."
gillian-c bulk-wpst ${TESTS_DIR} -I ${LIB_HEADERS_DIR} -I ${UTILS_HEADERS_DIR} \
    -S ${LIB_SRC_DIR} -S ${UTILS_SRC_DIR} --ignore-undef
