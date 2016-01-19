#include "Player.h"

Player::Player(int vida){
  _vida = vida;
}

void Player::setVida (int vida){_vida = vida;}

int Player::getVida(){return _vida;}

