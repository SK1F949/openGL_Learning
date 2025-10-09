#include "../include/draw_data.h"

DrawData::DrawData(VertexArray &va, IndexBuffer &ib, Shader &shader)
    : m_Va(va),
      m_Ib(ib),
      m_Shader(shader)
{
}