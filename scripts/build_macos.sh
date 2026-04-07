#!/bin/bash

#==============================================================================
# VneScene macOS Build Script
#==============================================================================
# Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# Author:    Ajeet Singh Yadav
# Created:   February 2026
#
# Autodoc:   yes
#
# This script builds VneScene for macOS with Xcode integration
#==============================================================================

set -e

JOBS=10
while [[ $# -gt 0 ]]; do
    case $1 in
        -j|--jobs) [[ -n "$2" && "$2" =~ ^[0-9]+$ ]] && { JOBS="$2"; shift 2; } || { echo "Invalid jobs: $2"; exit 1; } ;;
        -j*) JOBS="${1#-j}"; [[ "$JOBS" =~ ^[0-9]+$ ]] || { echo "Invalid jobs: $JOBS"; exit 1; }; shift ;;
        *) ARGS+=("$1"); shift ;;
    esac
done
set -- "${ARGS[@]}"

PLATFORM="Darwin"
COMPILER="clang"

usage() {
  echo "Usage: $0 [-t <build_type>] [-a <action>] [-l <lib_type>] [-clean] [-interactive] [-j <jobs>] [-xcode] [-xcode-only]"
  echo "  -t <build_type>  Debug|Release|RelWithDebInfo|MinSizeRel"
  echo "  -a <action>      configure|build|configure_and_build|test|xcode|xcode_build"
  echo "                   (configure enables VNE_SCENE_TESTS=ON and VNE_SCENE_EXAMPLES=ON; ctest runs unit + example smoke tests)"
  echo "  -l <lib_type>    static|shared (default: shared). Build dir: build/<lib_type>/..."
  echo "  -clean           Clean build directory first"
  echo "  -interactive     Interactive mode"
  echo "  -j <jobs>        Parallel jobs (default: 10)"
  echo "  -xcode           Also generate Xcode project"
  echo "  -xcode-only      Only generate Xcode project"
  exit 1
}

BUILD_TYPE="Debug"
ACTION="configure_and_build"
LIB_TYPE="shared"
CLEAN_BUILD=false
INTERACTIVE_MODE=false
GENERATE_XCODE=false
XCODE_ONLY=false

while [[ $# -gt 0 ]]; do
  case $1 in
    -t|--build-type) BUILD_TYPE="$2"; shift 2 ;;
    -a|--action) ACTION="$2"; shift 2 ;;
    -l|--lib-type) LIB_TYPE="$2"; shift 2 ;;
    -clean|--clean) CLEAN_BUILD=true; shift ;;
    -interactive|--interactive) INTERACTIVE_MODE=true; shift ;;
    -xcode|--xcode) GENERATE_XCODE=true; shift ;;
    -xcode-only|--xcode-only) XCODE_ONLY=true; GENERATE_XCODE=true; shift ;;
    -h|--help) usage ;;
    *) echo "Unknown option: $1"; usage ;;
  esac
done

[ "$XCODE_ONLY" = true ] && ACTION="xcode"
[ "$GENERATE_XCODE" = true ] && [ "$ACTION" = "configure_and_build" ] && ACTION="xcode_build"
[ "$ACTION" = "xcode" ] || [ "$ACTION" = "xcode_build" ] && GENERATE_XCODE=true

COMPILER_VERSION=$(clang --version | head -n 1 | awk '{print $4}' | sed 's/(.*)//')
[ "$COMPILER_VERSION" = "version" ] && COMPILER_VERSION=$(clang --version | head -n 1 | awk '{print $3}')

echo "$PLATFORM :: $COMPILER-${COMPILER_VERSION}"
PROJECT_ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)

# Build dir: build/<lib_type>/<build_type>/...
if [ "$GENERATE_XCODE" = true ]; then
  BUILD_DIR="$PROJECT_ROOT/build/${LIB_TYPE}/${BUILD_TYPE}/xcode-macos-$COMPILER-${COMPILER_VERSION}"
else
  BUILD_DIR="$PROJECT_ROOT/build/${LIB_TYPE}/${BUILD_TYPE}/build-macos-$COMPILER-${COMPILER_VERSION}"
fi

run_configure() {
  if [ "$GENERATE_XCODE" = true ]; then
    cmake -G Xcode \
      "-DCMAKE_BUILD_TYPE=$BUILD_TYPE" \
      "-DVNE_SCENE_LIB_TYPE=$LIB_TYPE" \
      -DCMAKE_C_COMPILER=clang \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15 \
      -DVNE_SCENE_TESTS=ON \
      -DVNE_SCENE_EXAMPLES=ON \
      "$PROJECT_ROOT"
  else
    cmake \
      "-DCMAKE_BUILD_TYPE=$BUILD_TYPE" \
      "-DVNE_SCENE_LIB_TYPE=$LIB_TYPE" \
      -DCMAKE_C_COMPILER=clang \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15 \
      -DVNE_SCENE_TESTS=ON \
      -DVNE_SCENE_EXAMPLES=ON \
      "$PROJECT_ROOT"
  fi
}

run_build() {
  if [ "$GENERATE_XCODE" = true ]; then
    xcodebuild -project VneScene.xcodeproj -configuration "$BUILD_TYPE" -parallelizeTargets -jobs "$JOBS"
  else
    make -j"$JOBS"
  fi
}

run_tests() {
  if [ "$GENERATE_XCODE" = true ]; then
    xcodebuild -project VneScene.xcodeproj -configuration "$BUILD_TYPE" -target RUN_TESTS
  else
    ctest --output-on-failure
  fi
}

clean_build() { rm -rf "$BUILD_DIR"; mkdir -p "$BUILD_DIR"; cd "$BUILD_DIR" || exit; }
ensure_build_dir() { [ ! -d "$BUILD_DIR" ] && mkdir -p "$BUILD_DIR"; cd "$BUILD_DIR" || exit; }

case $ACTION in
  configure) [ "$CLEAN_BUILD" = true ] && clean_build || ensure_build_dir; run_configure ;;
  build) [ "$CLEAN_BUILD" = true ] && clean_build || ensure_build_dir; run_configure; run_build ;;
  configure_and_build) [ "$CLEAN_BUILD" = true ] && clean_build || ensure_build_dir; run_configure; run_build ;;
  test) [ "$CLEAN_BUILD" = true ] && clean_build || ensure_build_dir; run_configure; run_build; run_tests ;;
  xcode) [ "$CLEAN_BUILD" = true ] && clean_build || ensure_build_dir; run_configure; echo "Xcode project: $BUILD_DIR (VneScene.xcodeproj)" ;;
  xcode_build) [ "$CLEAN_BUILD" = true ] && clean_build || ensure_build_dir; run_configure; run_build; echo "Xcode build done: $BUILD_DIR" ;;
  *) usage ;;
esac

echo ""
echo "=== Build completed successfully ==="
echo "Build directory: $BUILD_DIR"
