#include <hydra/hydra.h>

#include <sr1/vector>

using namespace hydra;

struct Key;

struct Piano : public Component
{
  void onInitialize();
  void onTick();

  std::sr1::observer_ptr<Key> getKey(Ray ray);
  void selectKey(std::sr1::observer_ptr<Key> key);

private:
  std::sr1::vector<std::sr1::observer_ptr<Key> > keys;
  std::sr1::observer_ptr<Entity> octaveButton;
  std::sr1::vector<std::sr1::observer_ptr<Sound> > sounds;
  std::sr1::zero_initialized<int> octaveIndex;

  //void preloadKeys();
  //void loadSounds();
  void updateOctaveButton(int octaveStartIndex, std::sr1::observer_ptr<Key> key);

};
