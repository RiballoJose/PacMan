#ifndef Ghost_H
#define Ghost_H

#include <Ogre.h>

class Ghost{
 public:
  Ghost (Ogre::SceneNode* node, int startZ, int startX, double speed);
  ~Ghost ();

  std::pair<int,int> getStart() const {return _start;}
  Ogre::Vector3 getMove() const{return _move;}
  Ogre::SceneNode* getNode() const{return _node;}
  double getSpeed() const {return _speed;}
  void setSpeed(double speed);
  void setMove(Ogre::Vector3 move);

 private:
  Ogre::Vector3 _move;
  Ogre::SceneNode* _node;
  std::pair<int, int> _start;
  double _speed;
};

#endif
