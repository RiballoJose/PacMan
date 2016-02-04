/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

#include <iostream>
#include <vector>
#include <GraphVertex.h>

using namespace std;

class GraphVertex;

class GraphEdge
{
 public:
  GraphEdge (GraphVertex* origin, GraphVertex* dest);
  ~GraphEdge ();

  void setOrigin (GraphVertex* origin) { _pOrigin = origin; }
  GraphVertex* getOrigin () const { return _pOrigin; }
  void setDestination (GraphVertex* dest) { _pDestination = dest; } 
  GraphVertex* getDestination () const { return _pDestination; }

 private:
  GraphVertex* _pOrigin;
  GraphVertex* _pDestination;
};

#endif
