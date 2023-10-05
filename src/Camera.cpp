#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ErenGL/Camera.h>
#include <ErenGL/Mesh.h>
#include <ErenGL/Gui.h>

Camera::Camera()
{
  Mesh *mesh = new Mesh();
  mesh->setPosition(glm::vec3(0, 0, 5));
  mesh->setRotation(glm::vec3(0, 0, 90));
  this->mesh = mesh;

  cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  view = glm::lookAt(mesh->getPosition(), mesh->getPosition() + cameraFront, cameraUp);
  projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  std::cout << "Camera created" << std::endl;

  this->mesh->setOnPositionChangeCallback([this](glm::vec3 newPosition)
                                          { this->updateMatrix(); });

  this->mesh->setOnRotationChangeCallback([this](glm::vec3 newRotation)
                                          { this->updateMatrix(); });

  this->mesh->setOnScaleChangeCallback([this](glm::vec3 newScale)
                                       { this->updateMatrix(); });
}

Camera::~Camera()
{
  mesh = nullptr;
}

Mesh *Camera::getMesh()
{
  return mesh;
}

void Camera::setActiveCamera(bool active)
{
  activeCamera = active;
}

bool Camera::isActiveCamera()
{
  return activeCamera;
}

void Camera::updateMatrix()
{
  updateProjectionMatrix();
  updateViewMatrix();
}

void Camera::setWindow(GLFWwindow *_window)
{
  window = _window;
}

void Camera::setScreenSize(int width, int height)
{
  SCR_WIDTH = width;
  SCR_HEIGHT = height;
  std::cout << "Camera screen size set: " << width << "x" << height << std::endl;
}

void Camera::setFov(float _fov)
{
  fov = _fov;
  updateMatrix();
}

void Camera::handleInput(float deltaTime)
{
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    if (isMouseVisible())
    {
      return;
    }
    mesh->setPosition(mesh->getPosition() + (cameraSpeed * deltaTime) * cameraFront);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    if (isMouseVisible())
    {
      return;
    }
    mesh->setPosition(mesh->getPosition() - (cameraSpeed * deltaTime) * cameraFront);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    if (isMouseVisible())
    {
      return;
    }
    mesh->setPosition(mesh->getPosition() - glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime));
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    if (isMouseVisible())
    {
      return;
    }
    mesh->setPosition(mesh->getPosition() + glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime));
  }

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
  {
    if (glfwGetTime() - lastTimeMouseVisible < 0.5f)
      return;

    lastTimeMouseVisible = glfwGetTime();

    setMouseVisible(!mouseVisible);
    glfwSetInputMode(window, GLFW_CURSOR, mouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    cameraSpeed = 5.0f;
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    cameraSpeed = 1.0f;
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
  {
    cameraSpeed = 2.5f;
  }
}

bool Camera::isMouseVisible()
{
  return mouseVisible;
}

void Camera::setMouseVisible(bool visible)
{
  mouseVisible = visible;
}

void Camera::handleMouse(GLdouble xpos, GLdouble ypos)
{
  if (isMouseVisible())
  {
    firstMouse = true;
    return;
  }

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

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  if (yaw > 360.0f)
    yaw = 0.0f;
  if (yaw < 0.0f)
    yaw = 360.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cameraFront = glm::normalize(direction);
  mesh->setRotation(glm::vec3(pitch, yaw, 90.0f - yaw));

  updateMatrix();
}

void Camera::handleScroll(GLdouble yoffset)
{
  if (isMouseVisible())
    return;

  if (fov >= minFov && fov <= maxFov)
    fov -= yoffset * 1.5f;
  if (fov <= minFov)
    fov = minFov;
  if (fov >= maxFov)
    fov = maxFov;

  updateMatrix();
}

void Camera::updateViewMatrix()
{
  view = glm::lookAt(mesh->getPosition(), mesh->getPosition() + cameraFront, cameraUp);
}

void Camera::updateProjectionMatrix()
{
  projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

glm::mat4 Camera::getViewMatrix()
{
  return view;
}

glm::mat4 Camera::getProjectionMatrix()
{
  return projection;
}

void Camera::setCameraPos(glm::vec3 pos)
{
  mesh->setPosition(pos);
}