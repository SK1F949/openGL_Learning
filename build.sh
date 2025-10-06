#!/bin/bash
g++ -o tmp/program src/*.cpp -lglfw -lGLEW -lGL -lGLU

./build/program
