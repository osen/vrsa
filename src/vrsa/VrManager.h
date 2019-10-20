#include <hydra/hydra.h>

using namespace hydra;

struct VrManager : public Component
{
  void onInitialize(
    std::sr1::observer_ptr<Camera> leftCamera,
    std::sr1::observer_ptr<Camera> rightCamera);

  void onTick();
  void onGui();

private:
  std::sr1::observer_ptr<Font> font;
  std::sr1::observer_ptr<Camera> leftCamera;
  std::sr1::observer_ptr<Camera> rightCamera;

};
