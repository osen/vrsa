#include <hydra/hydra.h>

using namespace hydra;

struct Key : public Component
{
  void onInitialize(int index);
  void onTick();

  void setSound(std::sr1::observer_ptr<Sound> sound);
  void setType(int type);
  void setSelected(int selected);
  void play();

  std::sr1::observer_ptr<Sound> getSound();
  int getIndex();
  int getType();

private:
  std::sr1::zero_initialized<int> type;
  std::sr1::zero_initialized<int> selected;
  std::sr1::observer_ptr<Sound> sound;
  std::sr1::shared_ptr<Material> material;

  std::sr1::zero_initialized<int> index;
  std::sr1::zero_initialized<float> time;

  void refresh();

};
