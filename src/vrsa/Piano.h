#include <hydra/hydra.h>

#include <sr1/vector>

using namespace hydra;

struct Key;

struct Piano : public Component
{
  void onInitialize();

private:
  std::sr1::vector<std::sr1::observer_ptr<Key> > keys;

};
