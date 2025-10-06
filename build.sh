#!/bin/bash
g++ -o tmp/program src/*.cpp -lglfw -lGLEW -lGL -lGLU

mkdir build
./build/program
