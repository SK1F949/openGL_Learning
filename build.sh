#!/bin/bash
g++ -o build/program src/*.cpp -lglfw -lGLEW -lGL -lGLU

mkdir build
./build/program
