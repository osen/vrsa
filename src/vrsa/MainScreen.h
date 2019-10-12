#include <hydra/hydra.h>

using namespace hydra;

struct MainScreen : public Component
{
  void onInitialize();

private:
  std::sr1::observer_ptr<Model> world;

};
