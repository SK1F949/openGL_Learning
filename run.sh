#!/bin/bash

SOURCE_FILES="src/main.cpp \
                src/classes/draw_data.cpp \
                src/classes/game_object.cpp \
                src/classes/object_transform.cpp \
                src/classes/test_object.cpp \
                src/classes/camera.cpp \
                src/classes/file_reader.cpp \
                src/classes/index_buffer.cpp \
                src/classes/renderer.cpp \
                src/classes/shader.cpp \
                src/classes/texture.cpp \
                src/classes/vertex_array.cpp \
                src/classes/vertex_buffer.cpp" 

g++ -g -o build/linux/program $SOURCE_FILES \
    -Isrc/include \
    -Ivendor/glm \
    -Ivendor/stb_image \
    -lglfw -lGLEW -lGL -lGLU -ldl

./build/linux/program