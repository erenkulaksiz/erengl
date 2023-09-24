#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Camera.h>

Camera::Camera()
{
  this->cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
  this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  this->view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  this->projection = glm::perspective(glm::radians(fov), (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT, 0.1f, 100.0f);
  std::cout << "Camera created" << std::endl;
}

void Camera::setWindow(GLFWwindow *window)
{
  this->window = window;
}

void Camera::setScreenSize(int width, int height)
{
  this->SCR_WIDTH = width;
  this->SCR_HEIGHT = height;
  std::cout << "Camera screen size set: " << width << "x" << height << std::endl;
}

void Camera::setFov(float fov)
{
  this->fov = fov;
}

void Camera::handleInput()
{
  this->currentFrame = glfwGetTime();
  this->deltaTime = this->currentFrame - this->lastFrame;
  this->lastFrame = this->currentFrame;

  if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
  {
    this->cameraPos += (this->cameraSpeed * this->deltaTime) * this->cameraFront;
    this->updateViewMatrix();
  }

  if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
  {
    this->cameraPos -= (this->cameraSpeed * this->deltaTime) * this->cameraFront;
    this->updateViewMatrix();
  }

  if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
  {
    this->cameraPos -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * (this->cameraSpeed * this->deltaTime);
    this->updateViewMatrix();
  }

  if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
  {
    this->cameraPos += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * (this->cameraSpeed * this->deltaTime);
    this->updateViewMatrix();
  }

  if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    this->setMouseVisible(!this->isMouseVisible());
    glfwSetInputMode(window, GLFW_CURSOR, this->isMouseVisible() ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
  }
}

void Camera::updateViewMatrix()
{
  this->view = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

void Camera::updateProjectionMatrix()
{
  this->projection = glm::perspective(glm::radians(fov), (float)this->SCR_WIDTH / (float)this->SCR_HEIGHT, 0.1f, 100.0f);
}

void Camera::setMouseVisible(bool visible)
{
  this->mouseVisible = visible;
}

bool Camera::isMouseVisible()
{
  return this->mouseVisible;
}

glm::mat4 Camera::getViewMatrix()
{
  return this->view;
}

glm::mat4 Camera::getProjectionMatrix()
{
  return this->projection;
}

void Camera::handleMouse(GLdouble xpos, GLdouble ypos)
{
  if (this->mouseVisible)
    return;

  if (this->firstMouse)
  {
    this->lastX = xpos;
    this->lastY = ypos;
    this->firstMouse = false;
  }

  float xoffset = xpos - this->lastX;
  float yoffset = this->lastY - ypos;
  this->lastX = xpos;
  this->lastY = ypos;

  xoffset *= this->sensitivity;
  yoffset *= this->sensitivity;

  this->yaw += xoffset;
  this->pitch += yoffset;

  if (this->pitch > 89.0f)
    this->pitch = 89.0f;
  if (this->pitch < -89.0f)
    this->pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  direction.y = sin(glm::radians(this->pitch));
  direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

  this->cameraFront = glm::normalize(direction);
  this->updateViewMatrix();
}

void Camera::setCameraPos(glm::vec3 pos)
{
  this->cameraPos = pos;
  this->updateViewMatrix();
}