#!/bin/bash

shopt -s nullglob

declare -i LINT_FAILED=0

lint_dir() {
    for x in "$1"/*.{c,cpp,h,hpp}; do
        echo "Linting $x."
        clang-format -Werror --dry-run "$x"
        LINT_FAILED+=$?
        clang-tidy -p build "$x"
        LINT_FAILED+=$?
    done

    for x in "$1"/*.{sh,bash}; do
        echo "Linting shell-script $x."
        shellcheck "$x"
        LINT_FAILED+=$?
    done
}

cd "${MESON_SOURCE_ROOT}" || exit 1;
lint_dir src
lint_dir include
lint_dir tools

exit "$LINT_FAILED"
