#pragma once

#include "../include/renderer.h"
#include "../include/shader.h"
#include "../include/includes.h"

class VertexArray;
class IndexBuffer;
class Shader;

class DrawData
{
public:
    VertexArray m_Va;
    IndexBuffer m_Ib;
    Shader m_Shader;

    DrawData(VertexArray &va, IndexBuffer &ib, Shader &shader);
};
