#ifndef Ghost_H
#define Ghost_H

#include <Ogre.h>

class Ghost{
 public:
  Ghost (std::string name, Ogre::SceneNode* node, Ogre::Entity* ent, int f, int c, double speed, Ogre::Vector3 start);
  ~Ghost ();

  std::string name() const {return _name;}
  std::pair<int,int> getStart() const {return _start;}
  Ogre::Vector3 getMove() const{return _move;}
  Ogre::SceneNode* getNode() const{return _node;}
  Ogre::Entity* getEntity() const{return _ent;}
  Ogre::Vector3 getStartPos() const {return _startPos;}
  double getSpeed() const {return _speed;}
  double getDist() const {return _dist;}
  bool canEat() const{return _canEat;}
  bool blinking() const{return _blinking;}
  void setSpeed(double speed);
  void setMove(Ogre::Vector3 move);
  void setEntity(Ogre::Entity* ent);
  void setCanEat(bool b);
  void setBlink();
  void setBlink(bool b);
  void setDist(double d);

 private:
  std::string _name;
  Ogre::Vector3 _move;
  Ogre::Vector3 _startPos;
  Ogre::SceneNode* _node;
  Ogre::Entity* _ent;
  std::pair<int, int> _start;
  double _speed;
  bool _canEat;
  bool _blinking;
  double _dist;
};

#endif
