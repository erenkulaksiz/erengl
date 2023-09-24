#ifndef CAMERA_H
#define CAMERA_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
  Camera();
  void setWindow(GLFWwindow *window);
  void setScreenSize(int width, int height);
  void setFov(float fov);
  void updateViewMatrix();
  void updateProjectionMatrix();
  void setMouseVisible(bool visible);
  bool isMouseVisible();
  void handleMouse(GLdouble xpos, GLdouble ypos);
  void handleInput();
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();
  void setCameraPos(glm::vec3 pos);

private:
  GLFWwindow *window;
  unsigned int SCR_WIDTH = 800;
  unsigned int SCR_HEIGHT = 600;
  GLdouble xpos;
  GLdouble ypos;
  float fov = 80.0f;
  float cameraSpeed = 2.5f;
  float sensitivity = 0.1f;
  float yaw = 0.0f;
  float pitch = 0.0f;
  bool firstMouse = true;
  bool mouseVisible = false;
  float lastX = SCR_WIDTH / 2.0f;
  float lastY = SCR_HEIGHT / 2.0f;
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 cameraPos;
  glm::vec3 cameraUp;
  glm::vec3 cameraFront;
  float currentFrame;
  float deltaTime;
  float lastFrame;
};

#endif
