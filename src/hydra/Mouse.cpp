#include "hydra.h"

#ifdef OPENGLD
  #include <GL/glut.h>
#else
  #include <GL/freeglut.h>
#endif

namespace hydra
{

int Mouse::x;
int Mouse::y;
bool Mouse::button0;
bool Mouse::button0Down;

void Mouse::motion(int x, int y)
{
  Mouse::x = x;
  Mouse::y = y;
}

int Mouse::getX()
{
  return x;
}

int Mouse::getY()
{
  return y;
}

bool Mouse::getButton(int button)
{
  if(button0)
  {
    button0 = false;

    return true;
  }

  return false;
}

bool Mouse::getButtonDown(int button)
{
  if(button0Down)
  {
    return true;
  }

  return false;
}

void Mouse::mouse(int button, int state, int x, int y)
{
  if(state == GLUT_DOWN)
  {
    button0 = true;
    button0Down = true;
  }
  else
  {
    button0 = false;
    button0Down = false;
  }

  Mouse::x = x;
  Mouse::y = y;
}

}
