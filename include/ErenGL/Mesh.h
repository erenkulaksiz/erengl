#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

class Mesh
{
public:
  Mesh(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float *vertices, unsigned int *indices, size_t vertexCount, size_t indexCount);
  void render(GLuint shaderProgram, Camera camera);
  void cleanup();
  void setup();
  void setPosition(glm::vec3 position);
  void setRotation(glm::vec3 rotation);
  void setScale(glm::vec3 scale);
  glm::vec3 getPosition();
  glm::vec3 getRotation();
  glm::vec3 getScale();

private:
  GLuint VAO, VBO, EBO;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  float *vertices;
  unsigned int *indices;
  size_t vertexCount;
  size_t indexCount;
};

#endif