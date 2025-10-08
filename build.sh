#!/bin/bash

build_project() {
  mkdir -p build/windows build/linux
  rm -rf ./build/windows/* ./build/linux/*

  SOURCE_FILES="src/main.cpp \
                src/classes/filereader.cpp \
                src/classes/indexBuffer.cpp \
                src/classes/renderer.cpp \
                src/classes/shader.cpp \
                src/classes/texture.cpp \
                src/classes/vertexArray.cpp \
                src/classes/vertexBuffer.cpp" 

  echo "Compiling for Linux..."
  g++ -o build/linux/program $SOURCE_FILES -Isrc/include -lglfw -lGLEW -lGL -lGLU -ldl

  if [ $? -eq 0 ]; then
    echo "Linux compilation successful!"
  else
    echo "Linux compilation failed!"
    exit 1
  fi

  WIN_INCLUDE="./src/vendor/win_libs/include"
  WIN_LIB="./src/vendor/win_libs/lib"

  echo "Compiling for Windows..."
  x86_64-w64-mingw32-g++ -o build/windows/program.exe \
    $SOURCE_FILES \
    -I"src/include" \
    -I"$WIN_INCLUDE" \
    -L"$WIN_LIB" \
    -lglfw3 -lglew32 -lopengl32 -lglu32 \
    -lgdi32 -luser32 -lkernel32 -lshell32 \
    -static -static-libgcc -static-libstdc++

  if [ $? -eq 0 ]; then
    echo "Windows compilation successful!"
    cp ./src/vendor/win_libs/bin/*.dll build/windows/
  else
    echo "Windows compilation failed!"
  fi

  echo "Archiving source code..."
  7z a source.zip ./*

  cp res -r build/linux
  cp data -r build/linux
  cp read.me -r build/linux

  cp res -r build/windows
  cp data -r build/windows
  cp read.me -r build/windows
}

if [ "$1" = "-s" ]; then
  echo "Building project with -s flag..."
  build_project

  echo "Running Linux version..."
  ./build/linux/program
else
  build_project
  echo "Build completed. Use './build.sh -s' to build and run immediately."
fi