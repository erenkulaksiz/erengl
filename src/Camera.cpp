#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ErenGL/Camera.h>

Camera::Camera()
{
  cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
  cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  std::cout << "Camera created" << std::endl;
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
  updateViewMatrix();
}

void Camera::handleInput()
{
  currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    cameraPos += (cameraSpeed * deltaTime) * cameraFront;
    updateViewMatrix();
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    cameraPos -= (cameraSpeed * deltaTime) * cameraFront;
    updateViewMatrix();
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime);
    updateViewMatrix();
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed * deltaTime);
    updateViewMatrix();
  }

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    cameraSpeed = 5.0f;
  }
  else
  {
    cameraSpeed = 2.5f;
  }
}

void Camera::handleMouse(GLdouble xpos, GLdouble ypos)
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

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cameraFront = glm::normalize(direction);
  updateViewMatrix();
}

void Camera::handleScroll(GLdouble yoffset)
{
  if (fov >= minFov && fov <= maxFov)
    fov -= yoffset * 1.5f;
  if (fov <= minFov)
    fov = minFov;
  if (fov >= maxFov)
    fov = maxFov;
  updateProjectionMatrix();
}

void Camera::updateViewMatrix()
{
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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
  cameraPos = pos;
  updateViewMatrix();
}