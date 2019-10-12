#include <hydra/hydra.h>

using namespace hydra;

struct Key : public Component
{
  void onInitialize();

  void setType(int type);

private:
  std::sr1::zero_initialized<int> type;

};
