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

std::array<bool, 10> Mouse::buttons;
std::array<bool, 10> Mouse::buttonsDown;

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
  if(buttons.at(button))
  {
    return true;
  }

  return false;
}

bool Mouse::getButtonDown(int button)
{
  if(buttonsDown.at(button))
  {
    buttonsDown.at(button) = false;

    return true;
  }

  return false;
}

void Mouse::mouse(int button, int state, int x, int y)
{
  if(state == GLUT_DOWN)
  {
    buttons.at(button) = true;
    buttonsDown.at(button) = true;
  }
  else
  {
    buttons.at(button) = false;
    buttonsDown.at(button) = false;
  }

  Mouse::x = x;
  Mouse::y = y;
}

}
