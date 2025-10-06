#!/bin/bash
g++ -o tmp/program src/*.cpp -lglfw -lGLEW -lGL -lGLU

./tmp/program

rm ./tmp/*
