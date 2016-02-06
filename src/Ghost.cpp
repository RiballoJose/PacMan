#include <Ghost.h>

Ghost::Ghost(Ogre::SceneNode* node, int startZ, int startX, double speed)
{
  _node = node;
  _start.first = startZ;
  _start.second = startX;
  _speed = speed;
}

Ghost::~Ghost(){
  delete _node;
}


void Ghost::setSpeed(double speed){_speed = speed;}
void Ghost::setMove(Ogre::Vector3 move){_move = move;}
