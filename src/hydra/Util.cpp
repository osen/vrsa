#include "hydra.h"

#include <iostream>

namespace hydra
{

void Util::splitStringWhitespace(std::string& input, std::vector<std::string>& output)
{
  std::string current;
  size_t slot = 0;

  for(size_t i = 0; i < input.length(); i++)
  {
    if(input.at(i) == ' ' || input.at(i) == '\t')
    {
      if(current.length() > 0)
      {
        if(output.size() > slot)
        {
          output.at(slot) = current;
        }
        else
        {
          output.push_back(current);
        }

        slot++;
        current = "";
      }
    }
    else if(input.at(i) == '\r' || input.at(i) == '\n')
    {

    }
    else
    {
      current += input.at(i);
    }
  }

  if(current.length() > 0)
  {
    if(output.size() > slot)
    {
      output.at(slot) = current;
    }
    else
    {
      output.push_back(current);
    }

    slot++;
  }

  while(output.size() > slot)
  {
    output.erase(output.begin() + (output.size() - 1));
  }
}

void Util::splitString(std::string& input, char splitter, std::vector<std::string>& output)
{
  std::string current;
  size_t slot = 0;

  for(size_t i = 0; i < input.length(); i++)
  {
    if(input.at(i) == splitter)
    {
      if(output.size() > slot)
      {
        output.at(slot) = current;
      }
      else
      {
        output.push_back(current);
      }

      slot++;
      current = "";
    }
    else
    {
      current += input.at(i);
    }
  }

  if(current.length() > 0)
  {
    if(output.size() > slot)
    {
      output.at(slot) = current;
    }
    else
    {
      output.push_back(current);
    }

    slot++;
  }

  while(output.size() > slot)
  {
    output.erase(output.begin() + (output.size() - 1));
  }
}

bool Util::rectIntersect(Vector4 a, Vector4 b)
{
  bool xi = false;
  bool yi = false;

  if(a.x < b.x)
  {
    if(a.x + a.z > b.x)
    {
      xi = true;
    }
  }
  else
  {
    if(b.x + b.z > a.x)
    {
      xi = true;
    }
  }

  if(!xi) return false;

  if(a.y < b.y)
  {
    if(a.y + a.w > b.y)
    {
      yi = true;
    }
  }
  else
  {
    if(b.y + b.w > a.y)
    {
      yi = true;
    }
  }

  if(!yi) return false;

  return true;
}

int Util::randBetween(int min, int max)
{
  return rand() % (max + 1 - min) + min;
}

}
