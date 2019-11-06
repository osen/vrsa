#include "hydra.h"

namespace hydra
{

void SoundSource::onInitialize(const std::sr1::observer_ptr<Sound>& sound)
{
  this->sound = sound;

  ALuint sid = 0;
  alGenSources(1, &sid);
  id = sid;
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  updatePosition();
  alSourcei(id, AL_BUFFER, sound->id);
  alSourcePlay(id);
}

void SoundSource::onTick()
{
  updatePosition();

  ALint state = 0;
  alGetSourcei(id, AL_SOURCE_STATE, &state);

  if(state == AL_STOPPED)
  {
    kill();
  }
}

void SoundSource::updatePosition()
{
  Transform* t = Environment::getCamera()->getEntity()->getComponent<Transform>();
  vec3 v = getEntity()->getComponent<Transform>()->getPosition();
  v /= 10.0f;
  Vector4 res = Environment::getCamera()->getView() * vec4(v, 1.0f);
  alSource3f(id, AL_POSITION, res.x, res.y, res.z);
}

SoundSource::~SoundSource()
{
  ALuint sid = id;
  alDeleteSources(1, &sid);
}

}
