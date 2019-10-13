#include <hydra/hydra.h>

#include <sr1/vector>

using namespace hydra;

struct Key;

struct Octave : public Component
{
  void onInitialize(int index);
  void onTick();

  std::sr1::observer_ptr<Key> getKey(Ray ray);
  void selectKey(std::sr1::observer_ptr<Key> key);

private:
  std::sr1::vector<std::sr1::observer_ptr<Key> > keys;
  std::sr1::vector<std::sr1::observer_ptr<Sound> > sounds;
  std::sr1::zero_initialized<int> index;

  void loadSounds();

};
