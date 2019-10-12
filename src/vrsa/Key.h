#include <hydra/hydra.h>

using namespace hydra;

struct Key : public Component
{
  void onInitialize();

  void setType(int type);
  void setSelected(int selected);
  int getType();

private:
  std::sr1::zero_initialized<int> type;
  std::sr1::zero_initialized<int> selected;

  void refresh();

};
