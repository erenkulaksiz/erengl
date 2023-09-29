#ifndef SHADER_H
#define SHADER_H
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char *filename);

class Shader
{
public:
  GLuint ID;
  Shader(const char *vertexPath, const char *fragmentPath);
  void Activate();
  void Delete();
  void SetModel(glm::mat4 model);
  void SetView(glm::mat4 view);
  void SetProjection(glm::mat4 projection);
  void SetVec4(const char *name, glm::vec4 value);
  GLuint GetId();

private:
  void compileErrors(unsigned int shader, const char *type);
};

#endif