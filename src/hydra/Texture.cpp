#include "hydra.h"

#include "lodepng.h"

#include "resources/button_texture.h"

#include <iostream>

namespace hydra
{

Texture::Texture() { }

Texture::Texture(Vector4 rgba)
{
  GLuint newId = 0;
  glGenTextures(1, &newId);
  id = newId;
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, &rgba);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  this->width = 1;
  this->height = 1;
}

Texture* Texture::load(std::string path)
{
  if(path == "")
  {
    throw hydra::Exception("Invalid texture path");
  }

  for(size_t i = 0; i < Environment::instance->textures.size(); i++)
  {
    if(Environment::instance->textures.at(i)->path == path)
    {
      return Environment::instance->textures.at(i).get();
    }
  }

  std::shared_ptr<Texture> rtn = std::make_shared<Texture>();
  rtn->path = path;
  Environment::instance->textures.push_back(rtn);

  unsigned width = 0;
  unsigned height = 0;

  std::vector<std::string> split;
  Util::splitString(path, '/', split);
  unsigned char** pixelData = new unsigned char*(); *pixelData = NULL;

  if(split.at(0) == "internal")
  {
    int res = 0;

    if(path == "internal/textures/button")
    {
      res = lodepng_decode32(pixelData,
        &width, &height, button_texture, sizeof(button_texture));
    }
    else
    {
      std::cout << "Invalid texture path: " << path << std::endl;
      throw std::exception();
    }

    if(res != 0)
    {
      std::cout << "Failed to load png" << std::endl;
      throw std::exception();
    }
  }
  else
  {
    int error = lodepng_decode32_file(pixelData, &width, &height, std::string(Environment::getAssetsDirectory() + "/" + path + ".png").c_str());

    if(error != 0)
    {
      error = lodepng_decode32_file(pixelData, &width, &height, path.c_str());

      if(error != 0)
      {
        std::cout << "Failed to load png "<<path<<" code: " << error << std::endl;
        std::cout << lodepng_error_text(error) << std::endl;
        throw std::exception();
      }
    }
  }

  GLuint newId = 0;
  glGenTextures(1, &newId);
  rtn->id = newId;
  glBindTexture(GL_TEXTURE_2D, rtn->id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, *pixelData);

/*
  std::vector<GLfloat> pixels;

  for(size_t i = 0; i < width * height * 4; i++)
  {
    pixels.push_back(((float)(*pixelData)[i]) / 255.0f);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &pixels[0]);
*/
  //std::cout << width << " " << height << std::endl;
  /*
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  */

  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  //rtn->data.insert(rtn->data.end(), *pixelData,
  //  *pixelData + width * height * 4);

  for(size_t i = 0; i < width * height * 4; i++)
  {
    rtn->data.push_back((*pixelData)[i]);
  }

  free(*pixelData); delete pixelData;

  rtn->width = width;
  rtn->height = height;

  return rtn.get();
}

Texture::~Texture()
{
  GLuint oldId = id;
  glDeleteTextures(1, &oldId);
}

std::string Texture::getPath()
{
  return path;
}

Vector4 Texture::getPixel(int x, int y)
{
  Vector4 rtn;
  size_t i = (width * 4) * y + (x * 4);

  rtn.x = data.at(i);
  rtn.y = data.at(i+1);
  rtn.z = data.at(i+2);
  rtn.w = data.at(i+3);

  return rtn;
}

int Texture::getWidth()
{
  return width;
}

int Texture::getHeight()
{
  return height;
}

}
