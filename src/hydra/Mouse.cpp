#include "hydra.h"

#include <SDL2/SDL.h>

namespace hydra
{

int Mouse::x;
int Mouse::y;

std::array<bool, 10> Mouse::buttons;
std::array<bool, 10> Mouse::buttonsDown;

void Mouse::clearButtons()
{
/*
  for(std::array<bool, 10>::iterator it = buttons.begin();
    it != buttons.end(); it++)
  {
    (*it) = false;
  }
*/

  for(std::array<bool, 10>::iterator it = buttonsDown.begin();
    it != buttonsDown.end(); it++)
  {
    (*it) = false;
  }
}

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
    //buttonsDown.at(button) = false;

    return true;
  }

  return false;
}

void Mouse::mouse(int button, int state, int x, int y)
{
  button -= 1;
  if(state == SDL_PRESSED)
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
