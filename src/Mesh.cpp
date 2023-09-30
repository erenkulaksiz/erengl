#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ErenGL/Mesh.h>
#include <ErenGL/Shader.h>
#include <ErenGL/Camera.h>

Mesh::Mesh(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, float *_vertices, unsigned int *_indices, size_t _vertexCount, size_t _indexCount, Shader &_shader)
    : position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), scale(glm::vec3(1)), vertices(nullptr), indices(nullptr), vertexCount(0), indexCount(0), shader(0)
{
  vertices = _vertices;
  indices = _indices;
  vertexCount = _vertexCount;
  indexCount = _indexCount;
  position = _position;
  rotation = _rotation;
  scale = _scale;
  shader = &_shader;
}

Mesh::Mesh() : position(glm::vec3(0, 0, 0)), rotation(glm::vec3(0, 0, 0)), scale(glm::vec3(1)), vertices(nullptr), indices(nullptr), vertexCount(0), indexCount(0), shader(0)
{
  this->vertices = vertices;
  this->indices = indices;
  this->vertexCount = vertexCount;
  this->indexCount = indexCount;
  this->position = position;
  this->rotation = rotation;
  this->scale = scale;
  this->shader = shader;
}

void Mesh::setCamera(Camera &_camera)
{
  camera = &_camera;
}

Camera *Mesh::getCamera()
{
  return camera;
}

void Mesh::deleteCamera()
{
  if (!camera)
    return;
  camera = nullptr;
  cameraDeleted = true;
}

bool Mesh::getCameraDeleted()
{
  return cameraDeleted;
}

void Mesh::setOnPositionChangeCallback(PositionChangeCallback callback)
{
  positionChangeCallback = callback;
}

void Mesh::setOnRotationChangeCallback(RotationChangeCallback callback)
{
  rotationChangeCallback = callback;
}

void Mesh::setOnScaleChangeCallback(ScaleChangeCallback callback)
{
  scaleChangeCallback = callback;
}

void Mesh::setName(std::string _name)
{
  name = _name;
  std::cout << "Mesh name set: " << name << std::endl;
}

void Mesh::setPosition(glm::vec3 _position)
{
  position = _position;

  if (positionChangeCallback != nullptr)
  {
    positionChangeCallback(position);
  }
}

void Mesh::setRotation(glm::vec3 _rotation)
{
  rotation = _rotation;

  if (rotationChangeCallback != nullptr)
  {
    rotationChangeCallback(rotation);
  }
}

void Mesh::setScale(glm::vec3 _scale)
{
  scale = _scale;

  if (scaleChangeCallback != nullptr)
  {
    scaleChangeCallback(scale);
  }
}

glm::vec3 Mesh::getPosition()
{
  return position;
}

glm::vec3 Mesh::getRotation()
{
  return rotation;
}

glm::vec3 Mesh::getScale()
{
  return scale;
}

int Mesh::getVertexCount()
{
  return vertexCount;
}

int Mesh::getIndexCount()
{
  return indexCount;
}

std::string Mesh::getName()
{
  return name;
}

glm::vec3 Mesh::getFront()
{
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);

  glm::vec4 front = model * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
  return glm::vec3(front.x, front.y, front.z);
}

glm::vec3 Mesh::getUp()
{
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);

  glm::vec4 up = model * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
  return glm::vec3(up.x, up.y, up.z);
}

void Mesh::render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
  if (isCamera)
  {
    return;
  }
  vao->Bind();
  shader->Activate();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);

  shader->SetModel(model);
  shader->SetView(viewMatrix);
  shader->SetProjection(projectionMatrix);

  if (texture)
    texture->Bind();
  else
    shader->SetVec4("color", glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));

  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Mesh::setIsSelected(bool _selectedMesh)
{
  selectedMesh = _selectedMesh;
}

bool Mesh::getIsSelected()
{
  return selectedMesh;
}

void Mesh::setIsCamera(bool _isCamera)
{
  isCamera = _isCamera;
}

bool Mesh::getIsCamera()
{
  return isCamera;
}

void Mesh::setVAO(VAO *_vao)
{
  vao = _vao;
}

void Mesh::setVBO(VBO *_vbo)
{
  vbo = _vbo;
}

void Mesh::setEBO(EBO *_ebo)
{
  ebo = _ebo;
}

void Mesh::setTexture(Texture *_texture)
{
  texture = _texture;
}

void Mesh::getVAO(VAO *_vao)
{
  _vao = vao;
}