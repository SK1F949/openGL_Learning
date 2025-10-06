#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "includes.h"

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}

static bool GLLogCall(const char *function, const char *file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL error] " << error << std::endl;
    return false;
  }
  return true;
}

class Renderer{
public:
  void Clear()const;
  void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};