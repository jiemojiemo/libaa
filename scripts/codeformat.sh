#!/usr/bin/env bash

find src include tests examples app -type f -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.h' | xargs clang-format -i {} --style=file
