/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#include <GraphEdge.h>

GraphEdge::GraphEdge
(GraphVertex* origin, GraphVertex* dest)
  :_pOrigin(origin), _pDestination(dest)
{
}

GraphEdge::~GraphEdge () 
{
}
