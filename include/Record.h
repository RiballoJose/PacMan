#ifndef Record_h
#define Record_h

#include <Ogre.h>

class Record{
 public:
  Record();

  void setPunt(int sc);
  void setPos(int pos);
  void setPlayer(Ogre::String gm);

  int getPunt() const;
  int getPos() const;
  Ogre::String getPlayer() const;

  Ogre::String toString();
 private:
  Ogre::String _player;
  int _punt, _pos;
};

#endif
