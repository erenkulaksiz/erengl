#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ErenGL/Shader.h>

std::string get_file_contents(const char *filename)
{
  std::FILE *fp = std::fopen(filename, "rb");
  if (fp)
  {
    std::string contents;
    std::fseek(fp, 0, SEEK_END);
    contents.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&contents[0], 1, contents.size(), fp);
    std::fclose(fp);
    return (contents);
  }
  throw(errno);
}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
  std::string vertexCode = get_file_contents(vertexPath);
  std::string fragmentCode = get_file_contents(fragmentPath);

  const char *vertexSource = vertexCode.c_str();
  const char *fragmentSource = fragmentCode.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);
  compileErrors(ID, "PROGRAM");

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

GLuint Shader::GetId()
{
  return ID;
}

void Shader::SetModel(glm::mat4 model)
{
  glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::SetView(glm::mat4 view)
{
  glUniformMatrix4fv(glGetUniformLocation(ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void Shader::SetProjection(glm::mat4 projection)
{
  glUniformMatrix4fv(glGetUniformLocation(ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::SetVec4(const char *name, glm::vec4 value)
{
  glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}

void Shader::SetColor(glm::vec4 color)
{
  glUniform4fv(glGetUniformLocation(ID, "color"), 1, glm::value_ptr(color));
}

void Shader::SetCameraPos(glm::vec3 pos)
{
  glUniform3fv(glGetUniformLocation(ID, "camPos"), 1, glm::value_ptr(pos));
}

void Shader::SetLightColor(glm::vec4 color)
{
  glUniform4fv(glGetUniformLocation(ID, "lightColor"), 1, glm::value_ptr(color));
}

void Shader::SetLightPos(glm::vec3 pos)
{
  glUniform3fv(glGetUniformLocation(ID, "lightPos"), 1, glm::value_ptr(pos));
}

void Shader::SetBillboardPos(glm::vec3 pos)
{
  glUniform3fv(glGetUniformLocation(ID, "billboardPos"), 1, glm::value_ptr(pos));
}

void Shader::Activate()
{
  glUseProgram(ID);
}

void Shader::Delete()
{
  glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char *type)
{
  GLint hasCompiled;
  char infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n"
                << infoLog << std::endl;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "SHADER_LINKING_ERROR for:" << type << "\n"
                << infoLog << std::endl;
    }
  }
}