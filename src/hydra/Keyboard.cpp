#include "hydra.h"

namespace hydra
{

void Keyboard::keyboard(unsigned char key, int x, int y)
{
  for(size_t i = 0; i < Environment::instance->keys.size(); i++)
  {
    if(Environment::instance->keys.at(i) == key)
    {
      return;
    }
  }

  Environment::instance->keys.push_back(key);
  Environment::instance->downKeys.push_back(key);

  if(key == 'X')
  {
    Environment::exit();
  }
}

void Keyboard::keyboardSpecial(int key, int x, int y)
{
  // TODO
/*
  if      (key == GLUT_KEY_LEFT)  keyboard('a',x,y);
  else if (key == GLUT_KEY_RIGHT) keyboard('d',x,y);
  else if (key == GLUT_KEY_UP)    keyboard('w',x,y);
  else if (key == GLUT_KEY_DOWN)  keyboard('s',x,y);
*/
}

void Keyboard::keyboardSpecialUp(int key, int x, int y)
{
  // TODO
}

void Keyboard::keyboardUp(unsigned char key, int x, int y)
{
  for(size_t i = 0; i < Environment::instance->keys.size(); i++)
  {
    if(Environment::instance->keys.at(i) == key)
    {
      Environment::instance->keys.erase(Environment::instance->keys.begin() + i);
      break;
    }
  }
}

bool Keyboard::getKey(unsigned char key)
{
  for(size_t i = 0; i < Environment::instance->keys.size(); i++)
  {
    if(Environment::instance->keys.at(i) == key)
    {
      return true;
    }
  }

  return false;
}

bool Keyboard::getKeyDown(unsigned char key)
{
  for(size_t i = 0; i < Environment::instance->downKeys.size(); i++)
  {
    if(Environment::instance->downKeys.at(i) == key)
    {
      Environment::instance->downKeys.erase(Environment::instance->downKeys.begin() + i);
      return true;
    }
  }

  return false;
}

}
