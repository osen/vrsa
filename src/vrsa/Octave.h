#ifndef OCTAVE_H
#define OCTAVE_H

#include <hydra/hydra.h>

#include <sr1/vector>

using namespace hydra;

struct Key;

struct OctaveConstruction
{
  std::sr1::zero_initialized<int> index;
  //std::sr1::vector<std::sr1::observer_ptr<Key> > keys;
};

struct PlayItem
{
  std::sr1::zero_initialized<int> key;
  std::sr1::zero_initialized<int> offset;
};

struct Octave : public Component
{
  void onInitialize(const OctaveConstruction& oc);
  void onTick();

  std::sr1::observer_ptr<Key> getKey(Ray ray);
  void selectKey(std::sr1::observer_ptr<Key> key);
  void setReadOnly(bool readOnly);
  void playKey(int index);
  void playKey(int index, int offset);
  void setPlaylist(const std::sr1::vector<int>& playlist);
  void setPlaylist(const std::sr1::vector<PlayItem>& playlist);
  void setBackground(bool background);
  bool isPlaying();
  void setTestMode(bool testMode);

private:
  std::sr1::vector<std::sr1::observer_ptr<Key> > keys;
  std::sr1::vector<std::sr1::observer_ptr<Sound> > sounds;
  std::sr1::zero_initialized<int> startIndex;
  std::sr1::zero_initialized<bool> readOnly;

  std::sr1::vector<PlayItem> playlist;
  std::sr1::zero_initialized<float> background;

  bool keysPlaying();

};

#endif
