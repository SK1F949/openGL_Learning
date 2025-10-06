#include "IndexBuffer.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "includes.h"
#include "Shader.h"
#include "Filereader.h"
#include "vendor/glm/ext/matrix_clip_space.hpp"
#include "vendor/glm/ext/matrix_transform.hpp"
#include "vendor/glm/ext/vector_float3.hpp"
//#include "vendor/imgui/imgui.h"
//#include "vendor/imgui/imgui_impl_glfw.h"
//#include "vendor/imgui/imgui_impl_opengl3.h"
// У МЕНЯ ЗАДАЧА СДЕЛАТЬ МАЙНКРАФТ

int screen_width = 960;
int screen_height = 540;

float move_speed;

float pos_x;
float pos_y;
float pos_z = 1.0f;

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

  if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    pos_z -= move_speed;
  if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    pos_z += move_speed;
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
        -50.0f, -50.0f, 0.0f,0.0f, // 0
        50.0f, -50.0f, 1.0f,0.0f,// 1
        50.0f, 50.0f, 1.0f,1.0f, // 2
        -50.0f, 50.0f, 0.0f,1.0f,// 3
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

    glm::mat4 proj = glm::ortho(0.0f,(screen_width)+.0f, 0.0f, (screen_height)+.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));

    Shader shader("res/shaders/base.glsl");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
    shader.SetUniform4f("u_Pos", 0.0f, 0.0f, 0.0f, -0.3f);

    std::string texture_path = config.getString("test_texture_path", "res/textures/vasil.png");
    Texture texture(texture_path);
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    //ImGui::CreateContext();
    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    //ImGui::StyleColorsDark();

    glm::vec3 translationA(200,200,0);
    glm::vec3 translationB(400,200,0);

    float r = 0.0f;
    float increment = 0.02f;

    while (!glfwWindowShouldClose(window)) {
      processInput(window);
      renderer.Clear();

      //ImGui_ImplOpenGL3_NewFrame();
      //ImGui_ImplGlfw_NewFrame();
      //ImGui::NewFrame();

      {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
        glm::mat4 mvp = proj * view * model;
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniform4f("u_Pos", 0.1f, 0.0f, 0.0f, 0.0f);

        renderer.Draw(va, ib, shader);
      }

      {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
        glm::mat4 mvp = proj * view * model;
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);
      }

      if (r > 1.0f)
        increment = -0.02f;
      else if (r < 0.0f)
        increment = 0.02f;
      r += increment;

      /*{
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::End();
        }
      
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  //ImGui_ImplOpenGL3_Shutdown();
  //ImGui_ImplGlfw_Shutdown();
  //ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}