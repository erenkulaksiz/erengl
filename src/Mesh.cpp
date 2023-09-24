#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Mesh.h"
#include "Camera.h"

Mesh::Mesh(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float *vertices, unsigned int *indices, size_t vertexCount, size_t indexCount)
    : position(position), rotation(rotation), scale(scale)
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  std::cout << "Mesh created" << std::endl;
}

Mesh::~Mesh()
{
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
}

void Mesh::render(GLuint shaderProgram, Camera camera)
{
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);

  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

  GLint viewMatrixLocation = glGetUniformLocation(shaderProgram, "view");
  GLint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projection");

  glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
  glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::cleanup()
{
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
  VBO = 0;
  EBO = 0;
  VAO = 0;
}