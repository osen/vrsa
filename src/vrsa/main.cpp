#include <hydra/hydra.h>

using namespace hydra;

struct IntroScreen : public Component
{
  void onInitialize()
  {
    logo = Texture::load("images/BULogo");
    Environment::getCamera()->setClearColor(Vector4(1, 1, 1, 1));
    timeout = 5;
  }

  void onTick()
  {
    timeout -= Environment::getDeltaTime();

    if(timeout <= 0)
    {
      getEntity()->kill();
    }
  }

  void onGui()
  {
    Gui::texture(Vector2(100, 100), logo.get());
  }

private:
  std::sr1::observer_ptr<Texture> logo;
  std::sr1::zero_initialized<float> timeout;

};

int main(int argc, char *argv[])
{
  Environment::initialize<IntroScreen>(argc, argv);

  return 0;
}
