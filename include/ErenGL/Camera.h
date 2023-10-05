#ifndef CAMERA_H
#define CAMERA_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ErenGL/Camera.h>
#include <ErenGL/Mesh.h>
#include <ErenGL/Gui.h>

typedef std::function<void(bool)> CameraActiveCallback;

class Camera
{
public:
  Camera();
  ~Camera();
  void setWindow(GLFWwindow *_window);
  void setScreenSize(int width, int height);
  void setFov(float _fov);
  void setActiveCamera(bool active);
  bool isActiveCamera();
  void updateViewMatrix();
  void updateProjectionMatrix();
  void updateMatrix();
  void handleMouse(GLdouble xpos, GLdouble ypos);
  void handleInput(float deltaTime);
  void handleScroll(double yoffset);
  void setMouseVisible(bool visible);
  void setCameraPos(glm::vec3 pos);
  bool isMouseVisible();
  void onPositionChange(glm::vec3 newPosition);
  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();
  Mesh *getMesh();

private:
  GLFWwindow *window;
  unsigned int SCR_WIDTH = 800;
  unsigned int SCR_HEIGHT = 600;
  GLdouble xpos;
  GLdouble ypos;
  float fov = 80.0f;
  float minFov = 1.0f;
  float maxFov = 120.0f;
  float cameraSpeed = 2.5f;
  float sensitivity = 0.1f;
  float yaw = 0.0f;
  float pitch = 0.0f;
  bool firstMouse = true;
  bool mouseVisible = false;
  bool activeCamera = false; // Is this camera active rendering camera?
  float lastTimeMouseVisible = 0.0f;
  float lastX = SCR_WIDTH / 2.0f;
  float lastY = SCR_HEIGHT / 2.0f;
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 cameraUp;
  glm::vec3 cameraFront;
  Mesh *mesh;
};

#endif
