#ifndef GUI_H
#define GUI_H
#include <GLFW/glfw3.h>

class Gui
{
public:
  Gui();
  void newFrame();
  void render();
  void drawStatistics(float fps, GLuint vertices);
  void shutdown();
};

#endif
