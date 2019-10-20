#include "hydra.h"

#include <iostream>

#include <cstring>

namespace hydra
{

static const char *characters = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

void Font::generateGlyphs()
{
  int x = 0;

  for(size_t i = 0; i < strlen(characters); i++)
  {
    Glyph g = {0};
    g.c = characters[i];
    g.texture = texture;
    g.y = 0;
    g.height = texture->getHeight();

    while(1)
    {
      if(x >= texture->getWidth())
      {
        //std::cout << "Glyph: " << g.c << std::endl;
        throw Exception("Glyph not found in image");
      }

      if(dirtyColumn(x) == true)
      {
        g.x = x;
        break;
      }

      x++;
    }

    while(1)
    {
      if(x >= texture->getWidth())
      {
        throw Exception("Glyph exceeded size of image");
      }

      if(dirtyColumn(x) == 0)
      {
        g.width = x - g.x;
        break;
      }

      x++;
    }

    glyphs.push_back(g);

    if(g.c == '-')
    {
      g.c = ' ';
      g.height = 1;
      glyphs.push_back(g);
    }
  }

  float tx = 1.0f / texture->getWidth();
  float ty = 1.0f / texture->getHeight();

  for(size_t i = 0; i < glyphs.size(); i++)
  {
    glyphs.at(i).mesh = std::make_shared<Mesh>();
    Vertex a;
    //a.position = Vector3(-0.5f, -0.5f, 0);
    a.position = Vector3(0, 0, 0);

    a.texCoord = Vector3(glyphs.at(i).x * tx,
      (glyphs.at(i).y + glyphs.at(i).height) * ty, 0);
    a.texCoord.y = 1.0f - a.texCoord.y;

    Vertex b;
    //b.position = Vector3(-0.5f, 0.5f, 0);
    b.position = Vector3(0, 1, 0);

    b.texCoord = Vector3(glyphs.at(i).x * tx,
      glyphs.at(i).y * ty, 0);
    b.texCoord.y = 1.0f - b.texCoord.y;

    Vertex c;
    //c.position = Vector3(0.5f, 0.5f, 0);
    c.position = Vector3(1, 1, 0);

    c.texCoord = Vector3((glyphs.at(i).x + glyphs.at(i).width) * tx,
      glyphs.at(i).y * ty, 0);
    c.texCoord.y = 1.0f - c.texCoord.y;

    glyphs.at(i).mesh->addFace(a, b, c);
    Vertex d;
    //d.position = Vector3(0.5f, -0.5f, 0);
    d.position = Vector3(1, 0, 0);

    d.texCoord = Vector3((glyphs.at(i).x + glyphs.at(i).width) * tx,
      (glyphs.at(i).y + glyphs.at(i).height) * ty, 0);
    d.texCoord.y = 1.0f - d.texCoord.y;

    glyphs.at(i).mesh->addFace(c, d, a);

    glyphs.at(i).mesh->generateVbos();
  }
}

bool Font::dirtyColumn(int x)
{
  for(int y = 0; y < texture->getHeight(); y++)
  {
    Vector4 col = texture->getPixel(x, y);

    if(col.w != 0) return true;
  }

  return false;
}

Font* Font::load(std::string path)
{
  for(size_t i = 0; i < Environment::instance->fonts.size(); i++)
  {
    if(Environment::instance->fonts.at(i)->path == path)
    {
      return Environment::instance->fonts.at(i).get();
    }
  }

  std::shared_ptr<Font> rtn = std::make_shared<Font>();
  Environment::instance->fonts.push_back(rtn);
  rtn->path = path;

  rtn->mesh = std::make_shared<Mesh>();
  Vertex a;
  a.position = Vector3(-0.5f, -0.5f, 0);
  a.texCoord = Vector3(0.0f, 0.0f, 0);
  Vertex b;
  b.position = Vector3(-0.5f, 0.5f, 0);
  b.texCoord = Vector3(0.0f, 1.0f, 0);
  Vertex c;
  c.position = Vector3(0.5f, 0.5f, 0);
  c.texCoord = Vector3(0.1f, 1.0f, 0);
  rtn->mesh->addFace(a, b, c);
  Vertex d;
  d.position = Vector3(0.5f, -0.5f, 0);
  d.texCoord = Vector3(0.1f, 0.0f, 0);
  rtn->mesh->addFace(c, d, a);

  rtn->mesh->generateVbos();

  rtn->texture = Texture::load(path);
  rtn->generateGlyphs();

  return rtn.get();
}

Glyph Font::getGlyph(char c)
{
  size_t i = 0;
  Glyph rtn = {0};

  for(i = 0; i < glyphs.size(); i++)
  {
    if(glyphs.at(i).c == c)
    {
      return glyphs.at(i);
    }
  }

  throw std::exception();
}

int Font::getHeight()
{
  Glyph g = getGlyph('A');

  return g.height;
}

int Font::getWidth(std::string sample)
{
  int rtn = 0;
  int space = 0;

  for(size_t i = 0; i < sample.length(); i++)
  {
    Glyph g = getGlyph(sample.at(i));

    rtn += space + g.width;
    space = 1;
  }

  return rtn;
}

}
