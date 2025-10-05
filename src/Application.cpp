#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "includes.h"
#include "Shader.h"
#include <GL/gl.h>

int screen_width = 1280;
int screen_height = 720;

int main(void) {
  GLFWwindow *window;

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(screen_width, screen_height, "test", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // V-Sync

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  {
    // Vertex data
    float vertices[] = {
        -0.5f, -0.5f, // 0
        0.5f,  -0.5f, // 1
        0.5f,  0.5f,  // 2
        -0.5f, 0.5f,  // 3
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexArray va;
    VertexBuffer vb(vertices, 4 * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb,layout);

    IndexBuffer ib(indices, 6);

    Shader shader("");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    float r = 0.0f;
    float increment = 0.02f;

    while (!glfwWindowShouldClose(window)) {
      glClear(GL_COLOR_BUFFER_BIT);

      shader.Bind();
      shader.SetUniform4f("u_Color", r, r, 1.0f, 1.0f);

      va.Bind();
      ib.Bind();

      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

      if (r > 1.0f)
        increment = -0.02f;
      else if (r < 0.0f)
        increment = 0.02f;

      r += increment;

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return 0;
}