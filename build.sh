#!/bin/bash
g++ -o build/program src/*.cpp -lglfw -lGLEW -lGL -lGLU

mkdir -p build
./build/program
