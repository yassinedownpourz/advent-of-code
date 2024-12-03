#!/bin/zsh
set -e
CFLAGS=(
    -Wformat=2 -Wno-unused-parameter -Wshadow \
    -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
    -Wredundant-decls -Wnested-externs -Wmissing-include-dirs)
cc "${CFLAGS[@]}" day03.c -o day03