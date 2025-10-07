#include "Filereader.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "includes.h"
#include "vendor/glm/ext/matrix_clip_space.hpp"
#include "vendor/glm/ext/matrix_transform.hpp"
#include "vendor/glm/ext/vector_float3.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
//  У МЕНЯ ЗАДАЧА СДЕЛАТЬ МАЙНКРАФТ

int screen_width = 960;
int screen_height = 540;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;

float move_speed;

float pos_x;
float pos_y;
float pos_z = 1.0f;

void processInput(GLFWwindow *window, float deltaTime)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = move_speed * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos +=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  // Движение вверх/вниз (если нужно)
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraUp;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraUp;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // обратный порядок, так как координаты Y идут снизу вверх
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // Ограничиваем угол обзора по вертикали
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}

int main(void)
{
  Filereader config;

  if (!config.loadFromFile("data/configs/globals.cfg"))
  {
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
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // V-Sync

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  {
    // Vertex data for a cube (8 vertices, each with position and texture
    // coordinates)
    float vertices[] = {
        // Position (x, y, z),   Texture coordinates (u, v)
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // 0 - bottom-left
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // 1 - bottom-right
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // 2 - top-right
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // 3 - top-left

        // Back face
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 4 - bottom-left
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 5 - bottom-right
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // 6 - top-right
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // 7 - top-left

        // Top face
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // 8 - front-left
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // 9 - front-right
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f,  // 10 - back-right
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // 11 - back-left

        // Bottom face
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,  // 12 - front-left
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f,   // 13 - front-right
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // 14 - back-right
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 15 - back-left

        // Right face
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // 16 - bottom-front
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 17 - bottom-back
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // 18 - top-back
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // 19 - top-front

        // Left face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // 20 - bottom-front
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 21 - bottom-back
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // 22 - top-back
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f    // 23 - top-front
    };

    // Indices for the cube (12 triangles = 36 indices)
    unsigned int indices[] = {// Front face (2 triangles)
                              0, 1, 2, 2, 3, 0,
                              // Back face (2 triangles)
                              5, 4, 7, 7, 6, 5,
                              // Top face (2 triangles)
                              8, 9, 10, 10, 11, 8,
                              // Bottom face (2 triangles)
                              14, 13, 12, 12, 15, 14,
                              // Right face (2 triangles)
                              16, 17, 18, 18, 19, 16,
                              // Left face (2 triangles)
                              22, 21, 20, 20, 23, 22};

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(vertices, 24 * 5 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 36);

    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f,
        100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), // позиция камеры
                                 glm::vec3(0.0f, 0.0f, 0.0f), // куда смотрим
                                 glm::vec3(0.0f, 1.0f, 0.0f)  // вектор "вверх"
    );

    Shader shader("res/shaders/base.glsl");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
    shader.SetUniform4f("u_Pos", 0.0f, 0.0f, 0.0f, -0.3f);

    std::string texture_path =
        config.getString("test_texture_path", "res/textures/vasil.png");
    Texture texture(texture_path);
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    glCullFace(GL_BACK);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    glm::vec3 translationA(200, 200, 0);
    glm::vec3 translationB(400, 200, 0);

    float r = 0.0f;
    float increment = 0.05f;

    float rotation_angle = 0.0f;
    float rotation_speed = 1.0f;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(window, deltaTime);
      renderer.Clear();

      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Матрица вида (камера)
      glm::mat4 view =
          glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

      // Матрица модели (можно оставить как есть или изменить)
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::rotate(model, glm::radians(rotation_angle),
                          glm::vec3(0.5f, 1.0f, 0.0f));

      glm::mat4 mvp = proj * view * model;

      shader.Bind();
      shader.SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(va, ib, shader);

      if (r > 1.0f)
        increment = -0.05f;
      else if (r < 0.0f)
        increment = 0.05f;
      r += increment;
    }

    glfwTerminate();
    return 0;
  }
}