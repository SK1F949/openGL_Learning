#include "../include/vertex_buffer.h"
#include "../include/vertex_buffer_layout.h"
#include "../include/vertex_array.h"

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
  glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer &vb,
                            const VertexBufferLayout &layout)
{
  Bind();
  vb.Bind();
  const auto &elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto &element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, element.normalized,
                          layout.GetStride(), (const void *)(size_t)offset);
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
}

void VertexArray::Bind() const
{
  glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
  glBindVertexArray(0);
}

int VertexArray::GetRendererID() const
{
  return m_RendererID;
}