#include "Record.h"

Record::Record(): _player(""), _punt(0), _pos(0){}

void Record::setPunt(int sc){
	_punt = sc;
}//Fin setPunt

void Record::setPos(int pos){
	_pos = pos;
}//Fin setPos

void Record::setPlayer(Ogre::String p){
	_player = p;
}//Fin setPlayer

int Record::getPunt() const{
	return _punt;
}//Fin getPunt

int Record::getPos() const{
	return _pos;
}//Fin getPos

Ogre::String Record::getPlayer() const{
	return _player;
}//Fin getPlayer

Ogre::String Record::toString(){
	std::stringstream aux;

	aux << _pos << ".\t\t\t\t\t\t"
	    << _player << "\t\t\t\t\t\t";

	if(_punt < 100){
        aux << "0";
        if(_punt < 10){
            aux << "0";
        }//Fin if
    }//Fin if

    aux << _punt;

	return aux.str().c_str();
}//Fin toString