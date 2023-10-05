#include <ErenGL/Line.h>
#include <ErenGL/Shader.h>

Line::Line(glm::vec3 end)
{
  lineShader = new Shader("shaders/line.vert", "shaders/line.frag");

  vao = new VAO();
  vao->Bind();

  float lineVertices[6] = {
      0.0f, 0.0f, 0.0f,
      end.x, end.y, end.z};

  unsigned int lineIndices[2] = {
      0, 1};

  vbo = new VBO(lineVertices, sizeof(lineVertices));
  ebo = new EBO(lineIndices, sizeof(lineIndices));

  vao->LinkVBO(*vbo, 0);
  vao->Unbind();
  vbo->Unbind();
  ebo->Unbind();
}

void Line::setRotation(glm::vec3 _rotation)
{
  rotation = _rotation;
}

void Line::setPosition(glm::vec3 _position)
{
  position = _position;
}

void Line::setColor(glm::vec4 _color)
{
  color = _color;
}

void Line::render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
  vao->Bind();
  lineShader->Activate();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
  model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

  lineShader->SetModel(model);
  lineShader->SetView(viewMatrix);
  lineShader->SetProjection(projectionMatrix);
  lineShader->SetVec4("color", color);

  glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}

void Line::Delete()
{
  vao->Delete();
  vbo->Delete();
  ebo->Delete();
  lineShader->Delete();
}