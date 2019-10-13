#include <hydra/hydra.h>

using namespace hydra;

struct Key : public Component
{
  void onInitialize();

  void setSound(std::sr1::observer_ptr<Sound> sound);
  void setType(int type);
  void setSelected(int selected);
  int getType();
  void play();

private:
  std::sr1::zero_initialized<int> type;
  std::sr1::zero_initialized<int> selected;
  std::sr1::observer_ptr<Sound> sound;

  void refresh();

};
