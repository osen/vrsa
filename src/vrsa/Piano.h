#include <hydra/hydra.h>

#include <sr1/vector>

using namespace hydra;

struct Key;

struct Piano : public Component
{
  void onInitialize();
  void onTick();

  std::sr1::observer_ptr<Key> getKey(Ray ray);

private:
  std::sr1::vector<std::sr1::observer_ptr<Key> > keys;

};
