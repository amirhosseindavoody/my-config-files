#!/bin/bash

set -euo pipefail

BUILD_DIR="build/src"

format() {
    # Format C++ files using clang-format and CMake format:
    clang-format -i --style=Google $(find ./src -name '*.h' -o -name '*.cpp' -o -name '*.c' -o -name '*.hpp' -o -name '*.cc')
    find . \( -path ./build -o -path ./.pixi \) -prune -false -o \( -name 'CMakeLists.txt' -o -name '*.cmake' \) -print | xargs cmake-format -i
}

iwuy() {
    iwyu_tool.py -p build src/
}

clean() {
    rm -rf build
}

build_debug() {
    cmake --preset debug
    cmake --build --preset debug
}

build_release() {
    cmake --preset release
    cmake --build --preset release
}

test() {
    local test_name="$1"
    if [[ -z "$test_name" ]]; then
        echo "Usage: test {<test_name>|--all} "
        exit 1
    elif [[ "$test_name" == "--all" ]]; then
        ctest -C Debug --test-dir $BUILD_DIR
        return
    fi
    ctest -C Debug -R "$test_name" --test-dir $BUILD_DIR
    # Uncomment the next line for verbose output
    # ctest -C Debug -R "$test_name" --verbose --test-dir $BUILD_DIR
}

coverage() {
    cmake --preset debug -DCOVERAGE=ON
    cmake --build --preset debug
    ctest -C Debug --test-dir $BUILD_DIR

    # Merge profile data
    llvm-profdata merge -sparse default.profraw -o coverage.profdata

    # Generate HTML coverage report
    llvm-cov show ./build/src/optimizer/nelder_mead_test \
        -instr-profile=coverage.profdata \
        -format=html \
        -output-dir=coverage_html \
        -ignore-filename-regex='(tests?|external|build)/'
}


make_tag() {
    build_debug

    set -x

    rm -rf release

    mkdir -p release

    # Copy the files that you want to the release directory.
    # For example, the entire src directory.
    cp -r ./build/src/main ./release/main

    version="$(date +%Y.%m.%d_%H.%M.%S)"
    release_filename="release_$version.tar.gz"
    tar -czf "$release_filename" release

    gh release create "v$version" "$release_filename" --title "v$version" --notes "Automated release created on $(date)"

    rm "$release_filename"
    rm -rf release

    set +x
}

release() {
    if [[ -z "$1" ]]; then
        echo "Usage: $0 release <release_dir>"
        exit 1
    fi

    release_dir="$1"
    echo "Creating release in $release_dir"

    latest_tag=$(gh release list --limit 1 --json tagName --jq '.[0].tagName')
    echo "Latest release tag: $latest_tag"

    gh release download "$latest_tag" --pattern "release_*.tar.gz" --dir "$release_dir" --skip-existing

    # Extract the downloaded tarball
    tarball=$(find "$release_dir" -maxdepth 1 -name 'release_*.tar.gz' -print0 | xargs -0 ls -t | head -n1)

    tar -xzf "$tarball" -C "$release_dir"

    rm "$tarball"

    release_area="$release_dir/$latest_tag"
    if [[ -d "$release_area" ]]; then
        rm -rf "$release_area"
    fi
    mv "$release_dir/release" "$release_area"

    echo -e "\033[1mCreated release to $release_area\033[0m"

    cd "$release_area"
    pixi shell-hook --manifest-path "$release_area/pixi.toml" --shell bash > activate.sh
    chmod g+rw -R .pixi
    echo -e "\033[1mRun \`source activate.sh\` to activate the environment\033[0m"

    cd "$release_dir"
    ln -fs "./$latest_tag" "latest"
    echo -e "\033[1mCreated symlink pointing to '$latest_tag': $release_dir/latest\033[0m"
}

if [[ "$1" == "format" ]]; then
    format
    exit 0
elif [[ "$1" == "iwuy" ]]; then
    iwuy
    exit 0
elif [[ "$1" == "clean" ]]; then
    clean
    exit 0
elif [[ "$1" == "build-debug" ]]; then
    build_debug
    exit 0
elif [[ "$1" == "build-release" ]]; then
    build_release
    exit 0
elif [[ "$1" == "test" ]]; then
    build_debug
    test "$2"
    exit 0
elif [[ "$1" == "test-all" ]]; then
    build_debug
    test --all
    exit 0
elif [[ "$1" == "coverage" ]]; then
    coverage
    exit 0
elif [[ "$1" == "make-tag" ]]; then
    make_tag
    exit 0
elif [[ "$1" == "release" ]]; then
    release "${2:-}"
    exit 0
else
    echo "Usage: $0 {"
    echo "    run-debug|run-release|build-debug|build-release|"
    echo "    coverage|make-tag|release|just-run|format|iwuy|clean|"
    echo "    test <test_name>|test-all"
    echo "}"
    exit 1
fi
