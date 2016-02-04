/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <OgreVector3.h>

using namespace std;

class Node
{
 public:
  Node ();
  Node (const int& id, const int& z, const int& x, const string& type, const Ogre::Vector3& position);
  ~Node ();

  int getID () const { return _id; }
  int getX () const { return _x; }
  int getZ () const { return _z; }
  string getType () const { return _type; }
  Ogre::Vector3 getPosition () const { return _position; }
  operator std::string() const;
    
 private:
  int _id;
  int _x;
  int _z;
  string _type;
  Ogre::Vector3 _position;
};

#endif
