#include "hydra.h"

#include <AL/al.h>

#include "stb_vorbis.h"

#include <iostream>
#include <vector>
#include <string>

namespace hydra
{

Sound* Sound::load(std::string path)
{
  for(size_t i = 0; i < Environment::instance->sounds.size(); i++)
  {
    if(Environment::instance->sounds.at(i)->path == path)
    {
      return Environment::instance->sounds.at(i).get();
    }
  }

  std::shared_ptr<Sound> rtn = std::make_shared<Sound>();
  rtn->path = path;
  Environment::instance->sounds.push_back(rtn);

  ALenum format = 0;
  ALsizei freq = 0;
  std::vector<char> bufferData;

  alGenBuffers(1, &rtn->id);

  path += ".ogg";
  loadOgg(std::string(Environment::getAssetsDirectory() + "/" + path).c_str(), bufferData, format, freq);

  alBufferData(rtn->id, format, &bufferData[0],
    static_cast<ALsizei>(bufferData.size()), freq);

  return rtn.get();
}

Sound::~Sound()
{
  alDeleteBuffers(1, &id);
}

void Sound::play()
{
  ALuint sid = 0;
  alGenSources(1, &sid);
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  alSource3f(sid, AL_POSITION, 0.0f, 0.0f, 0.0f);
  alSourcei(sid, AL_BUFFER, id);
  alSourcePlay(sid);

  hydra::Environment::instance->audioSources.push_back(sid);
}

std::string Sound::getPath()
{
  return path;
}

void Sound::play(float vol, float varMin, float varMax)
{
  //For better rand resolution
  varMin*=1000.0f;
  varMax*=1000.0f;
  float variance = (std::rand() % ((int)varMin + 1 - (int)varMax) + (int)varMin)/1000.0f;
    //return std::rand() % (max + 1 - min) + min;
  ALuint sid = 0;
  alGenSources(1, &sid);
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  alSource3f(sid, AL_POSITION, 0.0f, 0.0f, 0.0f);
  alSourcei(sid, AL_BUFFER, id);
  alSourcef(sid, AL_PITCH, variance);
  alSourcef(sid, AL_GAIN, vol);
  alSourcePlay(sid);

  hydra::Environment::instance->audioSources.push_back(sid);
}

void Sound::loadOgg(const std::string& fileName, std::vector<char>& buffer,
  ALenum &format, ALsizei &freq)
{
  int channels = 0;
  int sampleRate = 0;
  short *output = NULL;

  size_t samples = stb_vorbis_decode_filename(
    fileName.c_str(), &channels, &sampleRate, &output);

  if(samples == -1)
  {
    throw Exception("Failed to open file '" + fileName + "' for decoding");
  }

  // Record the format required by OpenAL
  if(channels == 1)
  {
    format = AL_FORMAT_MONO16;
  }
  else
  {
    format = AL_FORMAT_STEREO16;
  }

  // Record the sample rate required by OpenAL
  freq = sampleRate;

  buffer.resize(samples * 2);
  memcpy(&buffer.at(0), output, buffer.size());

  // Clean up the read data
  free(output);
}

}
