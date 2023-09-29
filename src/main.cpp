#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <ErenGL/Camera.h>
#include <ErenGL/Mesh.h>
#include <ErenGL/Gui.h>
#include <ErenGL/Shader.h>
#include <ErenGL/VAO.h>
#include <ErenGL/VBO.h>
#include <ErenGL/EBO.h>
#include <ErenGL/Line.h>

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float fps;
float deltaTime;
std::vector<Mesh *> meshes;
Camera *camera = new Camera();
Line xline;
Line yline;
Line zline;

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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                 {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
    camera->setScreenSize(width, height);
    camera->updateProjectionMatrix(); });
  glfwSetCursorPosCallback(window, [](GLFWwindow *window, GLdouble x, GLdouble y)
                           { camera->handleMouse(x, y); });
  glfwSetScrollCallback(window, [](GLFWwindow *window, GLdouble xoffset, GLdouble yoffset)
                        { camera->handleScroll(yoffset); });

  Shader shader("shaders/default.vert", "shaders/default.frag");

  VAO VAO1;
  VAO1.Bind();

  VBO VBO1(cubeVertices, sizeof(cubeVertices));
  EBO EBO1(cubeIndices, sizeof(cubeIndices));

  VAO1.LinkVBO(VBO1, 0);
  VAO1.Unbind();
  VBO1.Unbind();
  EBO1.Unbind();

  Gui gui;

  camera->setActiveCamera(true);
  camera->setWindow(window);
  camera->setScreenSize(SCR_WIDTH, SCR_HEIGHT);
  camera->setCameraPos(glm::vec3(0.0f, 0.0f, 3.0f));
  camera->getMesh()->setName("debugCamera");
  camera->getMesh()->setIsCamera(true);
  camera->getMesh()->setCamera(*camera);
  meshes.push_back(camera->getMesh());

  Camera *camera2 = new Camera();
  camera2->setWindow(window);
  camera2->setScreenSize(SCR_WIDTH, SCR_HEIGHT);
  camera2->setCameraPos(glm::vec3(0.0f, 5.0f, 3.0f));
  camera2->getMesh()->setName("camera_test_2");
  camera2->getMesh()->setIsCamera(true);
  camera2->getMesh()->setCamera(*camera2);
  meshes.push_back(camera2->getMesh());

  std::vector<glm::vec3> cubePositions = {
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.0f, 0.0f, 0.0f),
      glm::vec3(4.0f, 1.0f, 0.0f),
  };

  glEnable(GL_DEPTH_TEST);

  xline.setup(glm::vec3(1.0f, 0.0f, 0.0f));
  yline.setup(glm::vec3(0.0f, 1.0f, 0.0f));
  zline.setup(glm::vec3(0.0f, 0.0f, 1.0f));

  for (int index = 0; index < cubePositions.size(); ++index)
  {
    const glm::vec3 &position = cubePositions[index];
    Mesh *mesh = new Mesh(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), cubeVertices, cubeIndices, sizeof(cubeVertices), sizeof(cubeIndices), shader);
    mesh->setName("cube " + std::to_string(index));
    meshes.push_back(mesh);
  }

  while (!glfwWindowShouldClose(glfwGetCurrentContext()))
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - fps;
    fps = currentFrame;

    if (!camera->isActiveCamera())
    {
      std::cout << "our main cam is not active" << std::endl;

      for (Mesh *mesh : meshes)
      {
        if (mesh->getIsCamera())
        {
          if (mesh->getCamera() && mesh->getCamera()->isActiveCamera())
          {
            camera = mesh->getCamera();
          }
        }
      }
    }

    if (camera->getMesh()->getCameraDeleted())
    {
      delete camera;
    }

    camera->handleInput();
    gui.setMouseInteractions(camera->isMouseVisible());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Mesh *mesh : meshes)
    {
      shader.Activate();
      VAO1.Bind();
      mesh->render(camera->getViewMatrix(), camera->getProjectionMatrix());
    }

    for (Mesh *mesh : meshes)
    {
      if (mesh->getIsSelected())
      {
        glDisable(GL_DEPTH_TEST);
        xline.setPosition(mesh->getPosition());
        xline.setRotation(mesh->getRotation());
        xline.setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        xline.render(camera->getViewMatrix(), camera->getProjectionMatrix());

        yline.setPosition(mesh->getPosition());
        yline.setRotation(mesh->getRotation());
        yline.setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        yline.render(camera->getViewMatrix(), camera->getProjectionMatrix());

        zline.setPosition(mesh->getPosition());
        zline.setRotation(mesh->getRotation());
        zline.setColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        zline.render(camera->getViewMatrix(), camera->getProjectionMatrix());
        glEnable(GL_DEPTH_TEST);
      }
    }

    gui.newFrame();
    gui.drawGui(&meshes, camera->getViewMatrix(), camera->getProjectionMatrix());
    gui.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  for (Mesh *mesh : meshes)
  {
    delete mesh;
  }

  xline.Delete();
  yline.Delete();
  zline.Delete();

  gui.shutdown();
  VAO1.Delete();
  VBO1.Delete();
  EBO1.Delete();
  shader.Delete();
  glDisable(GL_DEPTH_TEST);

  glfwTerminate();
  return 0;
}