#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <ErenGL/Camera.h>
#include <ErenGL/Mesh.h>
#include <ErenGL/Gui.h>

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float fps;
float deltaTime;

typedef std::vector<Mesh> MeshArray_t;

Camera camera;

float cubeVertices[] = {
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
};

unsigned int cubeIndices[] = {
    0,
    1,
    2, // Front
    2,
    3,
    0,
    4,
    5,
    6, // Back
    6,
    7,
    4,
    0,
    3,
    7, // Left
    7,
    4,
    0,
    1,
    2,
    6, // Right
    6,
    5,
    1,
    3,
    2,
    6, // Top
    6,
    7,
    3,
    0,
    1,
    5, // Bottom
    5,
    4,
    0,
};

// triangle

float triangleVertices[] = {
    -0.5f,
    -0.5f,
    0.0f,
    0.5f,
    -0.5f,
    0.0f,
    0.0f,
    0.5f,
    0.0f,
};

unsigned int triangleIndices[] = {
    0,
    1,
    2,
};

const char *vertex_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;"
    "out vec3 FragPos;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 projection;"
    "void main() {"
    "  FragPos = aPos;"
    "  gl_Position = projection * view * model * vec4(aPos, 1.0);"
    "}";

const char *fragment_shader =
    "#version 330 core\n"
    "out vec4 FragColor;"
    "in vec3 FragPos;"
    "void main() {"
    "  FragColor = vec4(1.0, 0.5, 0.0, 1.0);"
    "}";

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ErenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  camera.setWindow(window);
  camera.setScreenSize(SCR_WIDTH, SCR_HEIGHT);
  camera.setCameraPos(glm::vec3(0.0f, 0.0f, 3.0f));

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  GLuint cubeVBO, cubeVAO, cubeEBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glGenBuffers(1, &cubeEBO);

  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader, NULL);
  glCompileShader(vs);

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader, NULL);
  glCompileShader(fs);

  GLuint shader_programme = glCreateProgram();
  glAttachShader(shader_programme, fs);
  glAttachShader(shader_programme, vs);
  glLinkProgram(shader_programme);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                 {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
    camera.setScreenSize(width, height);
    camera.updateProjectionMatrix(); });
  glfwSetCursorPosCallback(window, [](GLFWwindow *window, GLdouble x, GLdouble y)
                           { camera.handleMouse(x, y); });
  glfwSetScrollCallback(window, [](GLFWwindow *window, GLdouble xoffset, GLdouble yoffset)
                        { camera.handleScroll(yoffset); });

  std::vector<glm::vec3> cubePositions = {
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.0f, 0.0f, 0.0f),
  };

  std::vector<glm::vec3> trianglePositions = {
      glm::vec3(0.0f, 4.0f, 0.0f),
  };

  /*
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  */

  Gui gui;
  MeshArray_t meshes;

  for (const glm::vec3 &position : cubePositions)
  {
    Mesh mesh(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), cubeVertices, cubeIndices, sizeof(cubeVertices), sizeof(cubeIndices));
    meshes.push_back(mesh);
  }

  for (const glm::vec3 &position : trianglePositions)
  {
    Mesh mesh(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), triangleVertices, triangleIndices, sizeof(triangleVertices), sizeof(triangleIndices));
    meshes.push_back(mesh);
  }

  while (!glfwWindowShouldClose(glfwGetCurrentContext()))
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - fps;
    fps = currentFrame;

    camera.handleInput();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gui.drawStatistics(1.0f / deltaTime, 0);
    gui.render();

    glUseProgram(shader_programme);

    for (Mesh &mesh : meshes)
    {
      mesh.setup();
      mesh.render(shader_programme, camera);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
      std::cerr << "OpenGL error: " << error << std::endl;
    }
  }

  for (Mesh &mesh : meshes)
  {
    mesh.cleanup();
  }

  gui.shutdown();

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeEBO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteProgram(shader_programme);
  glDisable(GL_DEPTH_TEST);

  glfwTerminate();
  return 0;
}