#!/bin/bash
# Script to compile/optimize stdc++.h precompiled headers (PCH) for Clang and GCC.

# Exit on error
set -e

# Default settings
STD_VERSION="c++23"
OPT_LEVEL="O2"
COMPILER="clang++"

# Help message
show_help() {
    echo "Usage: ./build_pch.sh [options]"
    echo ""
    echo "Options:"
    echo "  -c, --compiler <compiler>  Specify compiler: clang++ or g++ (default: clang++)"
    echo "  -s, --std <version>        Specify C++ standard: c++17, c++20, c++23 (default: c++23)"
    echo "  -o, --opt <level>          Specify optimization: O0, O2, O3, Ofast (default: O2)"
    echo "  -h, --help                 Show this help message"
    exit 0
}

# Parse options
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -c|--compiler) COMPILER="$2"; shift ;;
        -s|--std) STD_VERSION="$2"; shift ;;
        -o|--opt) OPT_LEVEL="$2"; shift ;;
        -h|--help) show_help ;;
        *) echo "Unknown parameter: $1"; exit 1 ;;
    esac
    shift
done

# Resolve absolute paths
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
HEADER_PATH="$DIR/bits/stdc++.h"

if [ ! -f "$HEADER_PATH" ]; then
    echo "Error: Cannot find bits/stdc++.h in $DIR"
    exit 1
fi

echo "==> Building Precompiled Header..."
echo "    Compiler     : $COMPILER"
echo "    C++ Standard : $STD_VERSION"
echo "    Optimization : -$OPT_LEVEL"

if [[ "$COMPILER" == *"clang++"* ]]; then
    OUTPUT_PCH="$DIR/bits/stdc++.h.pch"
    echo "    Output PCH   : $OUTPUT_PCH"
    
    # Compile Clang PCH
    # We use -x c++-header to tell clang it's a header file
    "$COMPILER" -std="$STD_VERSION" -"$OPT_LEVEL" -x c++-header "$HEADER_PATH" -o "$OUTPUT_PCH"
    
    echo "==> Success! Clang PCH created."
    echo "    To use it, compile your code like this:"
    echo "    $COMPILER -std=$STD_VERSION -$OPT_LEVEL -I$DIR main.cpp"

elif [[ "$COMPILER" == *"g++"* ]]; then
    OUTPUT_PCH="$DIR/bits/stdc++.h.gch"
    echo "    Output PCH   : $OUTPUT_PCH"
    
    # Compile GCC PCH
    "$COMPILER" -std="$STD_VERSION" -"$OPT_LEVEL" -x c++-header "$HEADER_PATH" -o "$OUTPUT_PCH"
    
    echo "==> Success! GCC PCH created."
    echo "    To use it, compile your code like this:"
    echo "    $COMPILER -std=$STD_VERSION -$OPT_LEVEL -I$DIR main.cpp"
else
    echo "Error: Unsupported compiler $COMPILER. Use clang++ or g++."
    exit 1
fi
