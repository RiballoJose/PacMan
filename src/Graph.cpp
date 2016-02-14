/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#include <Graph.h>

Graph::Graph ()
{
  // Reserva de memoria inicial.
  _vertexes.reserve(5);
  _edges.reserve(5);
}

Graph::~Graph ()
{
  // Liberar vértices.
  typename std::vector<GraphVertex*>::iterator itVert;
  for (itVert = _vertexes.begin();
       itVert != _vertexes.end();
       ++itVert)
    delete *itVert;

  // Liberar arcos.
  typename std::vector<GraphEdge*>::iterator itEdge;
  for (itEdge = _edges.begin();
       itEdge != _edges.end();
       ++itEdge)
    delete *itEdge;

  _vertexes.clear();
  _edges.clear();
}

void
Graph::addVertex
(GraphVertex* pVertex)
{
  _vertexes.push_back(pVertex);
}

void
Graph::addEdge
(GraphVertex* pOrigin, GraphVertex* pDestination, bool undirected)
{
  GraphEdge* pEdge = new GraphEdge(pOrigin, pDestination);
  _edges.push_back(pEdge);
  pOrigin->addEdge(pEdge);
    
  if (undirected) {
    GraphEdge* pEdge2 = new GraphEdge(pDestination, pOrigin);
  _edges.push_back(pEdge2);
    pDestination->addEdge(pEdge2);
  }
}

std::vector<GraphVertex*>
Graph::adjacents
(int z, int x) const
{
  std::vector<GraphVertex*> result;
  std::vector<GraphVertex*>::const_iterator it;

  for (it = _vertexes.begin(); it != _vertexes.end(); ++it)
    if ((*it)->getData().getZ() == z and (*it)->getData().getX() == x) 
      return (*it)->adjacents();

  return result;
}

GraphVertex*
Graph::getVertex
(int z, int x) const
{
  std::vector<GraphVertex*>::const_iterator it;

  for (it = _vertexes.begin();
       it != _vertexes.end();
       ++it)
    if ((*it)->getData().getZ() == z and (*it)->getData().getX() == x) 
      return (*it);

  return NULL;
}

std::vector<GraphVertex*>
Graph::getLinks(GraphVertex* pOrigin)
{
  std::vector<GraphVertex*> aux;
  std::vector<GraphEdge*>::const_iterator it;
  for(it= _edges.begin();it!=_edges.end();++it){
    if ((*it)->getOrigin()->getData().getZ() == pOrigin->getData().getZ() and
	(*it)->getOrigin()->getData().getX() == pOrigin->getData().getX()){
      aux.push_back((*it)->getDestination());
      /*std::cout << "Posible camino añadido: " << "(";
      std::cout << pOrigin->getData().getZ() << ", ";
      std::cout << pOrigin->getData().getX() << ")" << "to (";
      std::cout << (*it)->getDestination()->getData().getZ() << ", ";
      std::cout << (*it)->getDestination()->getData().getX() << ")" << '\n';*/
    }
  }
  return aux;
}
Ogre::Vector3
Graph::getMove(GraphVertex* pOrigin, GraphVertex* pDestination)
{
  Ogre::Vector3 move (0,0,0);
  /*std::cout << "Origen.z = " << pOrigin->getData().getZ() << "; Origen.x = " << pOrigin->getData().getX() << '\t';
    std::cout << "Destino.z = " << pDestination->getData().getZ() << "; Destino.x = " << pDestination->getData().getX() << '\n';*/
  move.y = std::abs(((pDestination->getData().getX() - pOrigin->getData().getX()))+
		    ((pDestination->getData().getZ() - pOrigin->getData().getZ())));
  if((pDestination->getData().getX() - pOrigin->getData().getX())>0)
    move.x=1;
  else if((pDestination->getData().getX() - pOrigin->getData().getX()) < 0)
    move.x=-1;
  else if((pDestination->getData().getZ() - pOrigin->getData().getZ()) > 0)
    move.z=1;
  else if((pDestination->getData().getZ() - pOrigin->getData().getZ()) < 0)
    move.z=-1;
  //move.x = ((pDestination->getData().getX() - pOrigin->getData().getX())%2);
  //move.z = ((pDestination->getData().getZ() - pOrigin->getData().getZ())%2);
  //std::cout << "Move.z = " << move.z << "; Move.x = " << move.x << '\n';
  return move;
}
/*GraphVertex*
Graph::nextStep(GraphVertex*pOrigin, GraphVertex*pDestination)
{
  
}*/
