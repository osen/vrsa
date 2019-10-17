#include "KeyHelper.h"

#include <hydra/hydra.h>

#include <sstream>

using namespace hydra;

KeyInfo KeyHelper::octaveLayout(int startIndex, int index)
{
  KeyInfo ski = pianoLayout(startIndex);
  KeyInfo ki = pianoLayout(startIndex + index);

  ki.position -= ski.position;
  ki.position *= 120.0f;
  ki.position /= 14.0f;

  //std::cout << ki.position << std::endl;

  return ki;
}

KeyInfo KeyHelper::pianoLayout(int index)
{
  int type = 0;
  int position = 0;
  int octaveIdx = 0;

  for(int i = 0; i < 104; i++)
  {
    if(type == 5)
    {
      position ++;
      type++;
    }

    if(type == 13)
    {
      octaveIdx++;
      position ++;
      type = 0;
    }

    if(i == index)
    {
      KeyInfo ki;

      if(type == 1 || type == 3 || type == 7 ||
        type == 9 || type == 11)
      {
        ki.type = 1;
      }

      ki.position = (float)position / 120.0f;
      ki.octaveIndex = octaveIdx;

      std::stringstream trunk;
      trunk << "audio/octave";
      trunk << octaveIdx;
      trunk << "/";
      std::stringstream base;
      base << octaveIdx;
      ki.audioPath = trunk.str() + idxToNote(i % 12) + base.str();

      //std::cout << ki.position << std::endl;
      return ki;
    }

    type++;
    position ++;
  }

  throw Exception("Invalid index specified");
}

std::string KeyHelper::idxToNote(int index)
{
  index = index % 12;

  if(index == 0) return "C";
  else if(index == 1) return "CS";
  else if(index == 2) return "D";
  else if(index == 3) return "DS";
  else if(index == 4) return "E";
  else if(index == 5) return "F";
  else if(index == 6) return "FS";
  else if(index == 7) return "G";
  else if(index == 8) return "GS";
  else if(index == 9) return "A";
  else if(index == 10) return "AS";
  else if(index == 11) return "B";

  throw Exception("Invalid index specified");
}
