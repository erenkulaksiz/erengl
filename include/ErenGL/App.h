#ifndef APP_H
#define APP_H
#include <vector>
#include <ErenGL/Line.h>
#include <ErenGL/Camera.h>
#include <ErenGL/Mesh.h>
#include <ErenGL/Gui.h>

class App
{
public:
  App();
  ~App();
  void run();
  unsigned int SCR_WIDTH = 800;
  unsigned int SCR_HEIGHT = 600;
  std::vector<Mesh *> meshes;
  Camera *camera;
  GLFWwindow *window;
  Line *xline;
  Line *yline;
  Line *zline;
  Line *originXLine;
  Line *originYLine;
  Line *originZLine;
  Gui *gui;
  glm::vec3 lightPos = glm::vec3(2.0f, 2.0f, 2.0f);
  bool isPlaying = false;
  float previousFrame;
  float fps;
  float deltaTime;
  float currentFrame;
  static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
  {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app)
    {
      app->camera->handleScroll(yoffset);
    }
  }
  static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
  {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app)
    {
      app->SCR_WIDTH = width;
      app->SCR_HEIGHT = height;
      glViewport(0, 0, width, height);
      app->camera->setScreenSize(width, height);
      app->camera->updateProjectionMatrix();
    }
  }
  static void mouse_position_callback(GLFWwindow *window, double xpos, double ypos)
  {
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app)
    {
      app->camera->handleMouse(xpos, ypos);
    }
  }

private:
  void setupGLFW();
};

#endif