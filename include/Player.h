#ifndef PLAYER
#define PLAYER

class Player{
public:
  Player(int vida);
  ~Player();
  void crearPlayer(int vida);
  int getVida();
  void setVida(int v);

 
private:
  int _vida; 
};
#endif
