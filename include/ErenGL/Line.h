#ifndef LINE_H
#define LINE_H
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ErenGL/Mesh.h>
#include <vector>

class Line
{
public:
  Line(glm::vec3 end);
  ~Line();
  void Delete();
  void setPosition(glm::vec3 _position);
  void setRotation(glm::vec3 _rotation);
  void setColor(glm::vec4 _color);
  void render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
  glm::vec3 position = glm::vec3(0, 0, 0);
  glm::vec3 rotation = glm::vec3(0, 0, 0);
  VAO *vao;
  VBO *vbo;
  EBO *ebo;
  Shader *lineShader;
  glm::vec4 color = glm::vec4(1, 1, 1, 1);
};

#endif
