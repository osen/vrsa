#include <hydra/hydra.h>
#include <sqlite3/sqlite3.h>

using namespace hydra;

struct Database : public Component
{
  void onInitialize();
  ~Database();

  void execute(const std::string& sql);
  int queryInt(const std::string& sql);
  std::string getHostname();

private:
  std::sr1::zero_initialized<sqlite3*> db;

  void prepare();

};
