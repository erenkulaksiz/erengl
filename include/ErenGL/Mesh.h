#ifndef MESH_H
#define MESH_H
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <functional>
#include <ErenGL/Mesh.h>
#include <ErenGL/Shader.h>
#include <ErenGL/VAO.h>
#include <ErenGL/VBO.h>
#include <ErenGL/EBO.h>
#include <ErenGL/Texture.h>

class Camera;

typedef std::function<void(glm::vec3)> PositionChangeCallback;
typedef std::function<void(glm::vec3)> RotationChangeCallback;
typedef std::function<void(glm::vec3)> ScaleChangeCallback;

class Mesh
{
public:
  Mesh(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float *vertices, unsigned int *indices, size_t vertexCount, size_t indexCount, Shader &shader);
  Mesh();
  void render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Camera *_camera);
  void setPosition(glm::vec3 position);
  void setRotation(glm::vec3 rotation);
  void setScale(glm::vec3 scale);
  void setName(std::string name);
  void setIsSelected(bool selected);
  void setIsCamera(bool isCamera);
  void setColor(glm::vec4 color);
  void setIsLightSource(bool isLightSource);
  void setLightSourceColor(glm::vec4 color);
  bool getIsLightSource();
  glm::vec4 getColor();
  glm::vec4 getLightSourceColor();
  bool getIsCamera();
  bool getCameraDeleted();
  void setCamera(Camera &camera);
  Camera *getCamera();
  void deleteCamera();
  bool getIsSelected();
  int getVertexCount();
  int getIndexCount();
  std::string getName();
  glm::vec3 getPosition();
  glm::vec3 getRotation();
  glm::vec3 getScale();
  glm::vec3 getFront();
  glm::vec3 getUp();
  PositionChangeCallback positionChangeCallback = nullptr;
  RotationChangeCallback rotationChangeCallback = nullptr;
  ScaleChangeCallback scaleChangeCallback = nullptr;
  void setOnPositionChangeCallback(PositionChangeCallback callback);
  void setOnRotationChangeCallback(RotationChangeCallback callback);
  void setOnScaleChangeCallback(ScaleChangeCallback callback);
  void setTexture(Texture *texture);
  void getTexture(Texture *texture);
  void setLightPos(glm::vec3 lightPos);
  glm::vec3 getLightPos();

private:
  Texture *texture;
  VAO *vao;
  VBO *vbo;
  EBO *ebo;
  glm::vec3 lightPos;
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  glm::vec4 color;
  glm::vec4 lightColor;
  float *vertices;
  unsigned int *indices;
  size_t vertexCount;
  size_t indexCount;
  std::string name;
  bool selectedMesh = false; // Is user selected this mesh from menu?
  bool isCamera = false;     // Is this mesh a camera?
  bool cameraDeleted = false;
  bool hasTexture = false;    // is element has texture?
  bool isLightSource = false; // is element a light source?
  Shader *shader;
  Camera *camera;
};

#endif