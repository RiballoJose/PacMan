/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <GraphVertex.h>
#include <GraphEdge.h>

class Graph
{
 public:
  Graph ();
  ~Graph ();
  
  void addVertex (GraphVertex* pVertex);
  void addEdge (GraphVertex* pOrigin, GraphVertex* pDestination,
		bool undirected = true);

  // Lista de vértices adyacentes a uno dado.
  std::vector<GraphVertex*> adjacents (int z, int x) const;

  GraphVertex* getVertex (int z, int x) const;
  std::vector<GraphVertex*> getVertexes () const { return _vertexes; }
  std::vector<GraphEdge*> getEdges () const { return _edges; }

  std::vector<GraphVertex*> getLinks(GraphVertex* pOrigin);
  Ogre::Vector3 getMove(GraphVertex* pOrigin, GraphVertex* pDestination);

 private:
  std::vector<GraphVertex*> _vertexes;
  std::vector<GraphEdge*> _edges;
};

#endif
