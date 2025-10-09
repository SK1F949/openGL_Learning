#include "file_reader.h"
#include "index_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"
#include "camera.h"
#include "game_object.h"
#include "includes.h"
#include "draw_data.h"
#include "test_object.h"

// global variables
int screen_width = 960;
int screen_height = 540;
bool firstMouse = true;
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
float vertices[] = {
    // Front face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // top-left

    // Back face
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-left
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // bottom-right
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // top-right
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-left

    // Top face
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // front-left
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // front-right
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f,  // back-right
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, // back-left

    // Bottom face
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,  // front-left
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f,   // front-right
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // back-right
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // back-left

    // Right face
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // bottom-front
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-back
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-back
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-front

    // Left face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-front
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-back
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-back
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f    // top-front
};
unsigned int indices[] = {
    // Front face
    0, 1, 2, 2, 3, 0,
    // Back face
    5, 4, 7, 7, 6, 5,
    // Top face
    8, 9, 10, 10, 11, 8,
    // Bottom face
    14, 13, 12, 12, 15, 14,
    // Right face
    16, 17, 18, 18, 19, 16,
    // Left face
    22, 21, 20, 20, 23, 22};

// objects
std::shared_ptr<Camera> camera = std::make_shared<Camera>();
std::shared_ptr<TestObject> obj = std::make_shared<TestObject>();
Filereader config;

void checkGLError(const std::string &location)
{
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR)
  {
    std::cerr << "OpenGL error at " << location << ": " << err << std::endl;
  }
}

// callback functions
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera->ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  screen_width = width;
  screen_height = height;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, float deltaTime)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // camera control
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->ProcessKeyboard(GLFW_KEY_W, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->ProcessKeyboard(GLFW_KEY_S, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->ProcessKeyboard(GLFW_KEY_A, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->ProcessKeyboard(GLFW_KEY_D, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera->ProcessKeyboard(GLFW_KEY_SPACE, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    camera->ProcessKeyboard(GLFW_KEY_C, deltaTime);

  float rotationAmount = 2.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    obj->ProcessKeyboard(GLFW_KEY_UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    obj->ProcessKeyboard(GLFW_KEY_DOWN, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    obj->ProcessKeyboard(GLFW_KEY_RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    obj->ProcessKeyboard(GLFW_KEY_LEFT, deltaTime);
}

int main(void)
{
  // initialize config
  if (!config.loadFromFile("data/configs/globals.cfg"))
  {
    std::cerr << "Failed to load config file!" << std::endl;
    return 1;
  }

  float fov = config.getFloat("fov", 60.0f);

  float bg_red = config.getFloat("background_red", 0.0f);
  float bg_green = config.getFloat("background_green", 0.0f);
  float bg_blue = config.getFloat("background_blue", 0.0f);

  // initialize glfw
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "test", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // V-Sync

  // callbacks
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  // setting camera
  camera->SetMovementSpeed(config.getFloat("walk_speed", 2.5f));
  obj->SetMovementSpeed(10);

  // setting OpenGL
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glCullFace(GL_BACK);

  // initialize renders and shaders
  VertexArray va;
  VertexBuffer vb(vertices, 24 * 5 * sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(3);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 36);
  checkGLError("setupCubeGeometry");

  Shader shader("res/shaders/base.glsl");
  shader.Bind();
  shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

  std::string texture_path = config.getString("test_texture_path", "res/textures/vasil.png");
  Texture texture(texture_path);
  texture.Bind();
  shader.SetUniform1i("u_Texture", 0);

  glClearColor(bg_red, bg_green, bg_blue, 1.0f);

  // setting input
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // output
  mat4 proj = perspective(
      radians(fov), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

  std::cout << "Initialization complete. Starting render loop..." << std::endl;

  // main while
  Renderer renderer;
  DrawData draw(va, ib, shader);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  while (!glfwWindowShouldClose(window))
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window, deltaTime);

    // clear
    renderer.Clear();

    // get matrix
    mat4 view = camera->GetViewMatrix();

    // rendering
    {
      obj->GetTransform().SetPosition(vec3(0.0f, 0.0f, -3.0f));

      mat4 model = obj->GetTransform().GetModelMatrix();
      mat4 mvp = proj * view * model;

      renderer.Draw(draw, mvp);
    }

    // swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}