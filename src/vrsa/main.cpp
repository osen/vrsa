#include "IntroScreen.h"

#include <hydra/hydra.h>

using namespace hydra;

int main(int argc, char *argv[])
{
  Environment::initialize<IntroScreen>(argc, argv);

  return 0;
}
