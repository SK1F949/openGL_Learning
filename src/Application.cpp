#include "IndexBuffer.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "includes.h"
#include "Shader.h"
#include "Filereader.h"
// У МЕНЯ ЗАДАЧА СДЕЛАТЬ МАЙНКРАФТ

int screen_width = 720;
int screen_height = 720;

float move_speed;

float pos_x;
float pos_y;

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    pos_y += move_speed;
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    pos_y -= move_speed;
    
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    pos_x -= move_speed;
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    pos_x += move_speed;
}

int main(void) {
  Filereader config;

  if (!config.loadFromFile("data/configs/globals.cfg")) {
      std::cerr << "Failed to load config file!" << std::endl;
      return 1;
  }

  move_speed = config.getFloat("walk_speed", 0.01f);

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
        -0.5f, -0.5f, 0.0f,0.0f, // 0
        0.5f,  -0.5f, 1.0f,0.0f,// 1
        0.5f,  0.5f, 1.0f,1.0f, // 2
        -0.5f, 0.5f, 0.0f,1.0f,// 3
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb,layout);

    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/base.glsl");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
    shader.SetUniform4f("u_Pos", 0.0f, 0.0f, 0.0f, 1.0f);

    std::string texture_path = config.getString("test_texture_path", "res/textures/vasil.png");
    Texture texture(texture_path);
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.02f;

    while (!glfwWindowShouldClose(window)) {
      processInput(window);
      renderer.Clear();

      shader.Bind();

      shader.SetUniform4f("u_Pos", pos_x, pos_y, 0.0f, 1.0f);
      renderer.Draw(va, ib, shader);

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