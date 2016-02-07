#include "Player.h"

Player::Player(int vida, int punt){
  _vida = vida;
  _punt = punt;
}

void Player::setVida (int v){_vida = v;}

int Player::getVida(){return _vida;}

void Player::setPunt (int p){_punt = p;}

int Player::getPunt(){return _punt;}

