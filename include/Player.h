#ifndef PLAYER
#define PLAYER

class Player{
public:
  Player(int vida, int punt);
  ~Player();
  int getVida();
  void setVida(int v);
  int getPunt();
  int setPunt(int p);

 
private:
  int _vida;
  int _punt;
};
#endif
