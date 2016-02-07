#include <Ghost.h>

Ghost::Ghost(Ogre::SceneNode* node, Ogre::Entity* ent, int startZ, int startX, double speed, Ogre::Vector3 start)
{
  _node = node;
  _ent = ent;
  _start.first = startZ;
  _start.second = startX;
  _speed = speed;
  _startPos = start;
}

Ghost::~Ghost(){
  delete _node;
}


void Ghost::setSpeed(double speed){_speed = speed;}
void Ghost::setMove(Ogre::Vector3 move){_move = move;}
void Ghost::setEntity(Ogre::Entity* ent){_ent = ent;}
