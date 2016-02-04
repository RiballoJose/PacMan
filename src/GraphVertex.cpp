/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#include <GraphVertex.h>

GraphVertex::GraphVertex
(const Node& data)
  :_data(data)
{
}

GraphVertex::~GraphVertex ()
{
  std::vector<GraphEdge*>::iterator itEdge; 
  for (itEdge = _edges.begin();
       itEdge != _edges.end();
       ++itEdge)
    delete *itEdge;
  
  _edges.clear();
}

std::vector<GraphVertex*>
GraphVertex::adjacents () const
{
  std::vector<GraphVertex*> result;
  std::vector<GraphEdge*>::const_iterator it;

  for (it = _edges.begin();
       it != _edges.end();
       ++it)
    result.push_back((*it)->getDestination());

  return result;
}
