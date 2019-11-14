#include <sr1/zero_initialized>

#include <string>

struct KeyInfo
{
  std::sr1::zero_initialized<int> index;
  std::string audioPath;
  std::sr1::zero_initialized<int> type;
  std::sr1::zero_initialized<float> position;
  std::sr1::zero_initialized<int> octaveIndex;
};

struct KeyHelper
{
  static KeyInfo pianoLayout(int index);
  static KeyInfo octaveLayout(int startIndex, int index);

  static std::string idxToNote(int index);

  static std::string intervalToName(int interval);
  static std::string intervalToSName(int interval);
};
