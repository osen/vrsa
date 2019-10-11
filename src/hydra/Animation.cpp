#include "hydra.h"

#include <iostream>
#include <fstream>

namespace hydra
{

void Animation::setFps(double fps)
{
  this->fps = fps;
}

void Animation::apply(Part* part, double frame)
{
  Vector3 ta;
  Vector3 ra;
  Vector3 tb;
  Vector3 rb;
  double f2 = (int)frame + 1;

  if(f2 >= frames.size())
  {
    f2 = 0;
  }

  for(size_t pi = 0; pi < frames.at(frame).transforms.size(); pi++)
  {
    PartTransform& p = frames.at(frame).transforms.at(pi);

    if(p.partName == part->name)
    {
      ta = p.position;
      ra = p.rotation;
      break;
    }
  }

  for(size_t pi = 0; pi < frames.at(f2).transforms.size(); pi++)
  {
    PartTransform& p = frames.at(f2).transforms.at(pi);

    if(p.partName == part->name)
    {
      tb = p.position;
      rb = p.rotation;
      break;
    }
  }

  ta.x = (part->size.x / 100.0f) * ta.x;
  ta.y = (part->size.y / 100.0f) * ta.y;
  ta.z = (part->size.z / 100.0f) * ta.z;

  tb.x = (part->size.x / 100.0f) * tb.x;
  tb.y = (part->size.y / 100.0f) * tb.y;
  tb.z = (part->size.z / 100.0f) * tb.z;

  double fd = frame - (int)frame;
  Vector3 td = tb - ta;
  Vector3 rd = rb - ra;
  td *= fd;
  rd *= fd;

  Vector3 t(ta.x + td.x, ta.y + td.y, ta.z + td.z);
  //t.x = (part->size.x / 100.0f) * t.x;
  //t.y = (part->size.y / 100.0f) * t.y;
  //t.z = (part->size.z / 100.0f) * t.z;

  glTranslatef(t.x, t.y, t.z);
  glRotatef(ra.z + rd.z, 0, 0, 1);
  glRotatef(ra.y + rd.y, 0, 1, 0);
  glRotatef(ra.x + rd.x, 1, 0, 0);
}

Animation* Animation::load(std::string path)
{
  for(size_t i = 0; i < Environment::instance->animations.size(); i++)
  {
    if(Environment::instance->animations.at(i)->path == path)
    {
      return Environment::instance->animations.at(i).get();
    }
  }

  std::shared_ptr<Animation> rtn = std::make_shared<Animation>();
  Environment::instance->animations.push_back(rtn);

  rtn->path = path;
  rtn->fps = 1;
  path += ".anm";
  std::ifstream file(std::string(Environment::getAssetsDirectory() + "/" + path).c_str());

  if(!file.is_open())
  {
    throw std::exception();
  }

  while(!file.eof())
  {
    std::string line;
    getline(file, line);
    std::vector<std::string> splitLine;
    Util::splitStringWhitespace(line, splitLine);

    if(splitLine.size() < 1) continue;

    if(splitLine.at(0) == "f")
    {
      rtn->frames.push_back(Frame());
    }
    else if(splitLine.at(0) == "t")
    {
      PartTransform pt;
      pt.partName = splitLine.at(1);
      pt.position.x = atof(splitLine.at(2).c_str());
      pt.position.y = atof(splitLine.at(3).c_str());
      pt.position.z = atof(splitLine.at(4).c_str());
      pt.rotation.x = atof(splitLine.at(5).c_str());
      pt.rotation.y = atof(splitLine.at(6).c_str());
      pt.rotation.z = atof(splitLine.at(7).c_str());
      rtn->frames.at(rtn->frames.size() - 1).transforms.push_back(pt);
    }
  }

  return rtn.get();
}

}
