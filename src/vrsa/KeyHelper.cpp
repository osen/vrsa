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

std::string KeyHelper::intervalToName(int interval)
{
  if(interval == 0) return "Perfect unison";
  else if(interval == 1) return "Minor second";
  else if(interval == 2) return "Major second";
  else if(interval == 3) return "Minor third";
  else if(interval == 4) return "Major third";
  else if(interval == 5) return "Perfect fourth";
  else if(interval == 6) return "Tritone";
  else if(interval == 7) return "Perfect fifth";
  else if(interval == 8) return "Minor sixth";
  else if(interval == 9) return "Major sixth";
  else if(interval == 10) return "Minor seventh";
  else if(interval == 11) return "Major seventh";
  else if(interval == 12) return "Perfect octave";

  throw Exception("Invalid interval specified");
}

std::string KeyHelper::intervalToSName(int interval)
{
  if(interval == 0) return "Perfect unison";
  else if(interval == 1) return "Minor 2nd";
  else if(interval == 2) return "Major 2nd";
  else if(interval == 3) return "Minor 3rd";
  else if(interval == 4) return "Major 3rd";
  else if(interval == 5) return "Perfect 4th";
  else if(interval == 6) return "Tritone";
  else if(interval == 7) return "Perfect 5th";
  else if(interval == 8) return "Minor 6th";
  else if(interval == 9) return "Major 6th";
  else if(interval == 10) return "Minor 7th";
  else if(interval == 11) return "Major 7th";
  else if(interval == 12) return "Perfect octave";

  throw Exception("Invalid interval specified");
}
