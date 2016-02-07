#include "Player.h"

Player::Player(int vida, int punt){
  _vida = vida;
  _punt = punt;
}

void Player::setVida (int vida){_vida = vida;}

int Player::getVida(){return _vida;}

void Player::setPunt (int punt){_punt = punt;}

int Player::getPunt(){return _punt;}

