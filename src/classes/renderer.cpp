#include "../include/renderer.h"
#include "../include/shader.h"
#include "../include/includes.h"
#include "../include/camera.h"
#include "../include/game_object.h"
#include "../include/draw_data.h"

void GLClearError();
bool GLLogCall();

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
}
void Renderer::Draw(DrawData &data, mat4 &mvp) const
{
    data.m_Shader.Bind();
    data.m_Shader.SetUniformMat4f("u_MVP", mvp);

    data.m_Va.Bind();
    data.m_Ib.Bind();

    glDrawElements(GL_TRIANGLES, data.m_Ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}