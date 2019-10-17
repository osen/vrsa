#include <hydra/hydra.h>

#include <sr1/vector>

using namespace hydra;

struct Key;

struct OctaveConstruction
{
  std::sr1::zero_initialized<int> index;
  std::sr1::vector<std::sr1::observer_ptr<Key> > keys;
};

struct Octave : public Component
{
  void onInitialize(const OctaveConstruction& oc);
  void onTick();

  std::sr1::observer_ptr<Key> getKey(Ray ray);
  void selectKey(std::sr1::observer_ptr<Key> key);

private:
  std::sr1::vector<std::sr1::observer_ptr<Key> > keys;
  std::sr1::vector<std::sr1::observer_ptr<Sound> > sounds;
  std::sr1::zero_initialized<int> startIndex;

};
