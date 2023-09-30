#ifndef GUI_H
#define GUI_H
#include <GLFW/glfw3.h>
#include <ErenGL/Mesh.h>
#include <vector>

class Gui
{
public:
  Gui();
  void newFrame();
  void render();
  void drawGui(std::vector<Mesh *> *meshes, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, float fps, float deltaTime);
  void shutdown();
  void setMouseInteractions(bool value);
  bool mouseInteractions;

private:
  unsigned int selectedMeshIndex = -1;
  float positionVec3[3] = {0.0f, 0.0f, 0.0f};
  float rotationVec3[3] = {0.0f, 0.0f, 0.0f};
  float scaleVec3[3] = {1.0f, 1.0f, 1.0f};
};

#endif
