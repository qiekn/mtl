#!/bin/bash

TARGET="mystl_test"

cmake --build build -j && ./build/test/${TARGET}

