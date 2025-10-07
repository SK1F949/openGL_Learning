#!/bin/bash
mkdir -p build

g++ -o build/program src/*.cpp -lglfw -lGLEW -lGL -lGLU

./build/program
