#include <Ghost.h>

Ghost::Ghost(std::string name, Ogre::SceneNode* node, Ogre::Entity* ent, int startZ, int startX, double speed, Ogre::Vector3 start)
{
  _name = name;
  _node = node;
  _ent = ent;
  _start.first = startZ;
  _start.second = startX;
  _speed = speed;
  _startPos = start;
  _canEat = false;
  _blinking = false;
  _dist = 0.0;
}

Ghost::~Ghost(){
  delete _node;
}


void Ghost::setSpeed(double speed){_speed = speed;}
void Ghost::setMove(Ogre::Vector3 move){_move = move;}
void Ghost::setEntity(Ogre::Entity* ent){_ent = ent;}
void Ghost::setCanEat(bool b){_canEat=b;}
void Ghost::setBlink()
{
  if(_blinking)
    _blinking=false;
  else
    _blinking=true;
}
void Ghost::setBlink(bool b){_blinking=b;}
void Ghost::setDist(double d){_dist = d;}
