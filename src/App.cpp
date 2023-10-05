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
#include <stb/stb_image.h>
#include <ErenGL/Texture.h>
#include <ErenGL/Camera.h>
#include <ErenGL/Mesh.h>
#include <ErenGL/Gui.h>
#include <ErenGL/Shader.h>
#include <ErenGL/VAO.h>
#include <ErenGL/VBO.h>
#include <ErenGL/EBO.h>
#include <ErenGL/Line.h>
#include <ErenGL/App.h>

GLfloat cubeVertices[] = {
    //     COORDINATES     /        COLORS      /   TexCoord  /   normals
    // front face
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // Front bottom-left
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  // Front bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,   // Front top-right
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  // Front top-left

    // Back face
    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Back bottom-left
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Back bottom-right
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // Back top-right
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Back top-left

    // Left face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Left bottom-left
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,  // Left top-left
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,   // Left top-right
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // Left bottom-right

    // Right face
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Right bottom-left
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,  // Right top-left
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   // Right top-right
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Right bottom-right

    // Top face
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Top bottom-left
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // Top bottom-right
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Top top-right
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // Top top-left

    // Bottom face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // Bottom bottom-left
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,  // Bottom bottom-right
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,   // Bottom top-right
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // Bottom top-left
};

GLuint cubeIndices[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,

    // Back face
    4, 5, 6,
    6, 7, 4,

    // Left face
    8, 9, 10,
    10, 11, 8,

    // Right face
    12, 13, 14,
    14, 15, 12,

    // Top face
    16, 17, 18,
    18, 19, 16,

    // Bottom face
    20, 21, 22,
    22, 23, 20};

GLfloat planeVertices[] =
    {
        //     COORDINATES     /        COLORS      /   TexCoord  /   normals
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Lower left corner
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Upper left corner
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // Upper right corner
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // Lower right corner
};

GLuint planeIndices[] =
    {
        0, 2, 1,
        0, 3, 2};

void App::setupGLFW()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ErenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetWindowUserPointer(window, this);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetCursorPosCallback(window, mouse_position_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

App::App()
{
  setupGLFW();
  camera = new Camera();

  Shader texturedShader("shaders/default_light.vert", "shaders/default_light.frag");
  Shader billboardShader("shaders/billboard.vert", "shaders/billboard.frag");
  Shader bulldogErenShader("shaders/textured.vert", "shaders/textured.frag");
  Texture defaultTexture("resources/default3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  Texture bulldogErenTexture("resources/default2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  defaultTexture.texUnit(texturedShader, "tex0", 0);
  bulldogErenTexture.texUnit(bulldogErenShader, "tex0", 0);

  Shader lightShader("shaders/light.vert", "shaders/light.frag");

  Texture defaultBillboardTexture("resources/default3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
  defaultBillboardTexture.texUnit(billboardShader, "tex0", 0);

  gui = new Gui();

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
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(2.0f, 4.0f, 2.0f),
      glm::vec3(2.0f, 0.0f, 0.0f),
  };

  glEnable(GL_DEPTH_TEST);

  xline = new Line(glm::vec3(1.0f, 0.0f, 0.0f));
  yline = new Line(glm::vec3(0.0f, 1.0f, 0.0f));
  zline = new Line(glm::vec3(0.0f, 0.0f, 1.0f));
  originXLine = new Line(glm::vec3(5.0f, 0.0f, 0.0f));
  originYLine = new Line(glm::vec3(0.0f, 5.0f, 0.0f));
  originZLine = new Line(glm::vec3(0.0f, 0.0f, 5.0f));

  xline->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  yline->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  zline->setColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
  originXLine->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  originYLine->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  originZLine->setColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

  for (int index = 0; index < cubePositions.size(); ++index)
  {
    const glm::vec3 &position = cubePositions[index];
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    if (index == 1)
    {
      rotation = glm::vec3(0.0f, 45.0f, 0.0f);
    }
    Mesh *mesh = new Mesh(position, rotation, glm::vec3(1.0f), cubeVertices, cubeIndices, sizeof(cubeVertices), sizeof(cubeIndices), texturedShader);
    mesh->setName("cube " + std::to_string(index));
    mesh->setTexture(&defaultTexture);
    mesh->setLightSourceColor(glm::vec4(0.2f, 1.0f, 0.4f, 1.0f));
    meshes.push_back(mesh);
  }

  Mesh *mesh = new Mesh(glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), planeVertices, planeIndices, sizeof(planeVertices), sizeof(planeIndices), bulldogErenShader);
  mesh->setName("plane");
  mesh->setTexture(&bulldogErenTexture);
  meshes.push_back(mesh);

  Mesh *mesh_plane = new Mesh(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), planeVertices, planeIndices, sizeof(planeVertices), sizeof(planeIndices), texturedShader);
  mesh_plane->setName("plane_light");
  mesh_plane->setLightSourceColor(glm::vec4(0.2f, 1.0f, 0.4f, 1.0f));
  mesh_plane->setTexture(&defaultTexture);
  meshes.push_back(mesh_plane);

  Mesh *lightMesh = new Mesh(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), planeVertices, planeIndices, sizeof(planeVertices), sizeof(planeIndices), lightShader);
  lightMesh->setName("light_test");
  lightMesh->setLightSourceColor(glm::vec4(0.2f, 1.0f, 0.4f, 1.0f));
  lightMesh->setIsLightSource(true);
  lightMesh->setOnPositionChangeCallback([this](glm::vec3 position)
                                         { lightPos = position; });
  meshes.push_back(lightMesh);

  Mesh *mesh_billboard = new Mesh(glm::vec3(5.0f, 3.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), planeVertices, planeIndices, sizeof(planeVertices), sizeof(planeIndices), billboardShader);
  mesh_billboard->setName("mesh_billboard");
  mesh_billboard->setTexture(&defaultBillboardTexture);
  mesh_billboard->setIsBillboard(true);
  meshes.push_back(mesh_billboard);

  previousFrame = glfwGetTime();

  run();
}

void App::run()
{
  while (!glfwWindowShouldClose(glfwGetCurrentContext()))
  {
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

    camera->handleInput(deltaTime);
    gui->setMouseInteractions(camera->isMouseVisible());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Mesh *mesh : meshes)
    {
      mesh->setLightPos(lightPos);
      mesh->render(camera->getViewMatrix(), camera->getProjectionMatrix(), camera);
      if (mesh->getIsSelected())
      {
        glDisable(GL_DEPTH_TEST);
        xline->setPosition(mesh->getPosition());
        xline->setRotation(mesh->getRotation());
        xline->render(camera->getViewMatrix(), camera->getProjectionMatrix());

        yline->setPosition(mesh->getPosition());
        yline->setRotation(mesh->getRotation());
        yline->render(camera->getViewMatrix(), camera->getProjectionMatrix());

        zline->setPosition(mesh->getPosition());
        zline->setRotation(mesh->getRotation());
        zline->render(camera->getViewMatrix(), camera->getProjectionMatrix());
        glEnable(GL_DEPTH_TEST);
      }
    }

    originXLine->render(camera->getViewMatrix(), camera->getProjectionMatrix());
    originYLine->render(camera->getViewMatrix(), camera->getProjectionMatrix());
    originZLine->render(camera->getViewMatrix(), camera->getProjectionMatrix());

    currentFrame = glfwGetTime();
    deltaTime = currentFrame - previousFrame;
    previousFrame = currentFrame;
    fps = 1.0f / deltaTime;

    gui->newFrame();
    gui->drawGui(&meshes, camera->getViewMatrix(), camera->getProjectionMatrix(), fps, deltaTime, &isPlaying);
    gui->render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

App::~App()
{
  for (Mesh *mesh : meshes)
  {
    delete mesh;
  }

  xline->Delete();
  yline->Delete();
  zline->Delete();
  originXLine->Delete();
  originYLine->Delete();
  originZLine->Delete();

  gui->shutdown();

  glDisable(GL_DEPTH_TEST);

  glfwTerminate();
}