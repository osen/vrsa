#include "Fade.h"

void Fade::onInitialize(const Vector3& c, bool fadeOut)
{
  this->direction = fadeOut;
  color = c;

  if(fadeOut == true)
  {
    amount = 1;
  }
}

void Fade::onTick()
{
  if(direction == true)
  {
    amount -= Environment::getDeltaTime();

    if(amount <= 0)
    {
      amount = 0;
      getEntity()->kill();
    }
  }
  else
  {
    amount += Environment::getDeltaTime();

    if(amount >= 1)
    {
      amount = 1;
      getEntity()->kill();
    }
  }
}

void Fade::onGui()
{
  std::sr1::shared_ptr<Texture> pixel =
    std::sr1::make_shared<Texture>(Vector4(color.x, color.y, color.z, amount));

  Gui::texture(Vector4(0, 0,
    Environment::getScreenWidth(), Environment::getScreenHeight()), pixel.get());
}
