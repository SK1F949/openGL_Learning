#pragma once

#include "indexBuffer.h"
#include "shader.h"
#include "vertexArray.h"
#include "includes.h"

static void GLClearError();
static bool GLLogCall(const char *function, const char *file, int line);

class Renderer
{
public:
  void Clear() const;
  void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};