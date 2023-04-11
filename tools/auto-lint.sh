#!/bin/sh

# This is injected into find -exec sh -c ... {} \;, so it makes sense for $0
# not to expand.
# shellcheck disable=SC2016
C_LINT_CMD='echo Linting $0.; clang-format -Werror --i $0'

echo "~~~~~~ AUTO LINTER ~~~~~~"

lint_dir() {
    find "$1" -type f '(' \
        -name "*.cpp" \
        -o -name "*.c" \
        -o -name "*.hpp" \
        -o -name "*.h" \
    ')' \
        -exec sh -c "$C_LINT_CMD" {} \;
}

cd "${MESON_SOURCE_ROOT}" || exit 1;
lint_dir src
lint_dir include
lint_dir tools

echo "~~~~~ THE CODEBASE HAS CHANGED ~~~~~"
