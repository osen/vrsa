#include "hydra.h"

#include "lodepng.h"

#include "resources/button_texture.h"

#include <iostream>

namespace hydra
{

Texture::Texture() { }

Texture::Texture(Vector4 rgba)
{
  // One pixel doesnt need linear interpolation
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  internal = Environment::getContext()->createTexture();
  internal->setSize(1, 1);
  internal->setPixel(0, 0, rgba);
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
  unsigned char* pixelData = NULL;

  if(split.at(0) == "internal")
  {
    int res = 0;

    if(path == "internal/textures/button")
    {
      res = lodepng_decode32(&pixelData,
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
    int error = lodepng_decode32_file(&pixelData, &width, &height,
      std::string(Environment::getAssetsDirectory() + "/" + path + ".png").c_str());

    if(error != 0)
    {
      error = lodepng_decode32_file(&pixelData, &width, &height, path.c_str());

      if(error != 0)
      {
        std::cout << "Failed to load png " << path << " code: " << error << std::endl;
        std::cout << lodepng_error_text(error) << std::endl;
        throw Exception("Failed to load image");
      }
    }
  }

  // Already default
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  rtn->internal = Environment::getContext()->createTexture();
  rtn->internal->setSize(width, height);

  for(size_t y = 0; y < height; y++)
  {
    for(size_t x = 0; x < width; x++)
    {
      size_t idx = width * 4 * y + x * 4;

      rend::vec4 rgba(
        (float)pixelData[idx] / 255.0f,
        (float)pixelData[idx+1] / 255.0f,
        (float)pixelData[idx+2] / 255.0f,
        (float)pixelData[idx+3] / 255.0f
      );

      rtn->internal->setPixel(x, (height - 1) - y, rgba);
    }
  }

  free(pixelData);

  return rtn.get();
}

std::string Texture::getPath()
{
  return path;
}

Vector4 Texture::getPixel(int x, int y)
{
  return internal->getPixel(x, y) * 255.0f;
}

int Texture::getWidth()
{
  return internal->getSize().x;
}

int Texture::getHeight()
{
  return internal->getSize().y;
}

std::sr1::shared_ptr<rend::TextureAdapter> Texture::getInternal()
{
  return internal;
}

}
