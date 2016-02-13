#ifndef Ghost_H
#define Ghost_H

#include <Ogre.h>

class Ghost{
 public:
  Ghost (Ogre::SceneNode* node, Ogre::Entity* ent, int f, int c, double speed, Ogre::Vector3 start);
  ~Ghost ();

  std::pair<int,int> getStart() const {return _start;}
  Ogre::Vector3 getMove() const{return _move;}
  Ogre::SceneNode* getNode() const{return _node;}
  Ogre::Entity* getEntity() const{return _ent;}
  Ogre::Vector3 getStartPos() const {return _startPos;}
  double getSpeed() const {return _speed;}
  bool canEat() const{return _canEat;}
  void setSpeed(double speed);
  void setMove(Ogre::Vector3 move);
  void setEntity(Ogre::Entity* ent);
  void setCanEat(bool b);

 private:
  Ogre::Vector3 _move;
  Ogre::Vector3 _startPos;
  Ogre::SceneNode* _node;
  Ogre::Entity* _ent;
  std::pair<int, int> _start;
  double _speed;
  bool _canEat;
};

#endif
