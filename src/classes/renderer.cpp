#include "../include/renderer.h"
#include "../include/shader.h"
#include "../include/includes.h"

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}