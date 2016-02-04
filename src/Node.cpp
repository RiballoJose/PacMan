/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#include <Node.h>

Node::Node ()
{
}

Node::Node
(const int& id, const int& z,const int& x, const string& type, const Ogre::Vector3& position)
{
  _id = id;
  _z = z;
  _x = x;
  _type = type;
  _position = position;
}

Node::~Node ()
{
}

Node::operator
std::string() const
{
  std::stringstream r;
  r << "[Node: " << _id << " Type: " << _type << " (" 
    << _position.x << ", " << _position.y << ", " 
    << _position.z << ")]";
  return r.str();
}
