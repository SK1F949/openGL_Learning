#pragma once

#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"
#include "includes.h"
#include "game_object.h"
#include "draw_data.h"

static void GLClearError();
static bool GLLogCall();

class DrawData;

class Renderer
{
public:
  void Clear() const;
  void Draw(DrawData &data, mat4 &mvp) const;
};