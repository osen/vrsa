#include "Octave.h"
#include "Key.h"

void Octave::onInitialize(int index)
{
  std::cout << "Octave Index: " << index << std::endl;
}

void Octave::onTick()
{

}

std::sr1::observer_ptr<Key> Octave::getKey(Ray ray)
{
  return std::sr1::observer_ptr<Key>();
}

void Octave::selectKey(std::sr1::observer_ptr<Key> key)
{

}

void Octave::loadSounds()
{

}

