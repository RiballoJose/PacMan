#include "PlayState.h"
#include "PauseState.h"
#include "MenuState.h"
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

using namespace Ogre::OverlayElementCommands;

void
PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _sceneMgr = _root->getSceneManager("SceneManager");
  _sceneMgr->addRenderQueueListener(GameManager::getSingletonPtr()->getOverlaySystem());
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();
  
  _currentLevel = 0;
  _pacSpeed = 2.5;
  _currentDir = _nextDir = _prevDir = _prevCol = _prevRow = 0;
  _nPacDots = _score = 0;
  _canEat = false;
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _light = _sceneMgr->createLight("Light");
  
  LoadLevels();
  createScene();
  createOverlay();

  _endLevel = _exitGame = false;
}
void 
PlayState::createOverlay()
{
  _ovJuego = _overlayManager->getByName("Juego");
  _ovPunt = _overlayManager->getOverlayElement("Puntuacion");
  _ovVida = _overlayManager->getOverlayElement("Vida");
  _ovScore = _overlayManager->getOverlayElement("Puntos");
  _ovVida->setCaption("Vidas");
  _ovPunt->setCaption("Puntos");
  _ovJuego->show();
}
void
PlayState::createScene()
{
  Ogre::Entity* ent = NULL;
  Ogre::SceneNode* nodo = NULL;
  std::stringstream bloq, material;
  bloq.str("");
  _pacmanDef = false;
  _perspective = 0;
  _camera->setPosition(Ogre::Vector3(0, 42, 7));
  _camera->lookAt(Ogre::Vector3(0, -50, 0));
  
  nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode("Escenario", Ogre::Vector3(-0.5,0.0,3.0));
  ent = _sceneMgr->createEntity("Base.mesh");
  nodo->attachObject(ent);

  int aux = -13;
  _lifes.reserve(3);
  for(int i = 0; i < 3; i++){
    bloq << "Life_" << i;
    nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode((bloq.str()),Ogre::Vector3(aux+i, 0.5, 19.5));
    ent = _sceneMgr->createEntity("Pacman.mesh");
    nodo->attachObject(ent);
    _lifes.push_back(nodo);
    bloq.str("");
  }
  aux = -14;
  _wallRows = new std::vector<int>();
  _wallCols = new std::vector<int>();
  int nGhost = 0;
  int id = 0;

  _level = new Graph;
  for(int f = _currentLevel*(31); f < _currentLevel*(31)+31; f++){
    for(int c = 0; c < _columnas; c++){
      bloq.str("");
      switch(_levels[f][c]){
      case 0://vacio
	bloq << "Void(" << f << "," << c << ")";	
	break;
      case 1://muros
	bloq << "Wall(" << f << "," << c << ")";
	nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	//ent = _sceneMgr->createEntity(bloq.str(), "Cube.mesh");
	//ent->setMaterialName("offset.material");
	ent = _sceneMgr->createEntity(bloq.str(), "Muro_tex.mesh");
	nodo->setScale(0.5, 0.5, 0.5);
	nodo->attachObject(ent);
	_wallRows->push_back(f);
	_wallCols->push_back(c);
	break;
      case 2://Pac-dots segun wikipedia es la comida
	bloq << "Pac-dot(" << f << "," << c << ")";
	nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	ent = _sceneMgr->createEntity(bloq.str(), "Bola.mesh");
	nodo->setScale(0.5, 0.5, 0.5);
	nodo->attachObject(ent);
	_nPacDots++;
	break;
      case 3://comefantasmas
	bloq << "Power-Pellet(" << f << "," << c << ")";
	nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	ent = _sceneMgr->createEntity(bloq.str(), "Bola.mesh");
	nodo->setScale(1.0, 1.0, 1.0);
	nodo->attachObject(ent);
	break;
      case 4://zona de fantasmas
	switch(nGhost){
	case 0://rojo
	  bloq << "Blinky";
	  ent = _sceneMgr->createEntity(bloq.str(), "Fantasma.mesh");
	  ent->setMaterialName("F_rojo_mat");
	  _blinky = new Ghost(_sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12))),
			      ent, f-(((f-_currentLevel*31))-12), c-aux, 0.8, Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	  _blinky->getNode()->setScale(1.0, 1.0, 1.0);
	  _blinky->getNode()->attachObject(ent);
	  break;
	case 1://rosa
	  bloq << "Pinky";
	  ent = _sceneMgr->createEntity(bloq.str(), "Fantasma.mesh");
	  ent->setMaterialName("F_rosa_mat");
	  _pinky = new Ghost(_sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12))),
			     ent, f-(((f-_currentLevel*31))-12), c-aux, 1.0, Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	  _pinky->getNode()->setScale(1.0, 1.0, 1.0);
	  _pinky->getNode()->attachObject(ent);
	  break;
	case 2://azul
	  bloq << "Inky";
	  ent = _sceneMgr->createEntity(bloq.str(), "Fantasma.mesh");
	  ent->setMaterialName("F_azul_mat");
	  _inky = new Ghost(_sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12))),
			    ent, f-(((f-_currentLevel*31))-12), c-aux, 1.0, Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	  _inky->getNode()->setScale(1.0, 1.0, 1.0);
	  _inky->getNode()->attachObject(ent);
	  break;
	case 3://naranja
	  bloq << "Clyde";
	  ent = _sceneMgr->createEntity(bloq.str(), "Fantasma.mesh");
	  ent->setMaterialName("F_naranja_mat");
	  _clyde = new Ghost(_sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12))),
			     ent, f-(((f-_currentLevel*31))-12), c-aux, 1.0, Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	  _clyde->getNode()->setScale(1.0, 1.0, 1.0);
	  _clyde->getNode()->attachObject(ent);
	  break;
	default:
	  bloq << "GhostZone(" << f << "," << c << ")";
	  break;
	}
	nGhost++;
	break;
      case 5://donde empieza el pacman->cuidado:tenemos dos 5s
	if(!_pacmanDef){
	  bloq << "Pacman";
	  _pacman = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	  ent = _sceneMgr->createEntity(bloq.str(), "Pacman.mesh");
	  _pacman->setScale(0.8, 0.8, 0.8);
	  _pacman->translate(0.5,0,0.0);
	  _pacman->attachObject(ent);
	  _startPos = _pacman->getPosition();
	  _startRow = f-_pacman->getPosition().z;
	  _startCol = (c-_pacman->getPosition().x)+1;
	  _currentRow = _startRow;
	  _currentCol = _startCol;
	  _pacmanDef = true;
	}
	break;
      case 6:
	bloq << "Transporter(" << f << "," << c << ")";
	nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	ent = _sceneMgr->createEntity(bloq.str(), "Muro.mesh");
	nodo->setScale(0.2, 0.2, 0.2);
	nodo->attachObject(ent);
	nodo->setVisible(false);
	break;
      case 7://Ghost door
	bloq << "Wall(" << f << "," << c << ")";
	nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-_currentLevel*31))-12)));
	ent = _sceneMgr->createEntity(bloq.str(), "Muro.mesh");
	nodo->setScale(0.5, 0.5, 0.5);
	nodo->attachObject(ent);
	nodo->setVisible(false);
	_wallRows->push_back(f);
	_wallCols->push_back(c);
	break;
      default:
	break;
      }
      if(_levels[f][c]!=1 and _levels[f][c]!=9 and
	 ((((c > 0 and _levels[f][c-1]!=1) and ((f > _currentLevel*(31) and _levels[f-1][c]!=1) or
						(f < (_currentLevel+1)*(31) and _levels[f+1][c]!=1))) or
	   ((c < _columnas and _levels[f][c+1]!=1) and ((f > _currentLevel*(31) and _levels[f-1][c]!=1) or
							(f < (_currentLevel+1)*(31) and _levels[f+1][c]!=1))) or
	   ((f > _currentLevel*(31) and _levels[f-1][c]!=1) and ((c > 0 and _levels[f][c-1]!=1) or
								 (c < _columnas and _levels[f][c+1]!=1))) or
	   ((f+1 < (_currentLevel+1)*(31) and _levels[f+1][c]!=1) and ((c > 0 and _levels[f][c-1]!=1) or
								     (c < _columnas and _levels[f][c+1]!=1)))))){
	_level->addVertex(new GraphVertex(Node(id,f,c, bloq.str(), Ogre::Vector3(f, 0, c))));
      }
      id++;
      aux+=1;
    }
    aux = -14;
  }
  calculateAdjs();
  _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
  _sceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));
  _sceneMgr->setShadowTextureCount(2);
  _sceneMgr->setShadowTextureSize(512);
  
  _light->setType(Ogre::Light::LT_SPOTLIGHT);
  _light->setDirection(Ogre::Vector3(0,-1,0));
  _light->setSpotlightInnerAngle(Ogre::Degree(25.0f));
  _light->setSpotlightOuterAngle(Ogre::Degree(200.0f));
  _light->setPosition(0, 150, 0);
  _light->setSpecularColour(1, 1, 1);
  _light->setDiffuseColour(1, 1, 1);
  _light->setSpotlightFalloff(5.0f);
  _light->setCastShadows(true);
}

void
PlayState::calculateAdjs(){
  std::vector<GraphVertex*> vertexes = _level->getVertexes();
  std::vector<GraphVertex*>::const_iterator it;
  GraphVertex* aux = NULL;
  bool wall = false;
  bool vertex = false;
  for (it = vertexes.begin(); it != vertexes.end(); ++it){
    for(int f = (*it)->getData().getZ()+1; f < (_currentLevel+1)*31 and (!wall and ! vertex); f++){
      if (_levels[f][(*it)->getData().getX()]==1){wall = true;}
      else if((aux = _level->getVertex(f, (*it)->getData().getX()))!=NULL){
	vertex = true;
	_level->addEdge((*it), aux);
      }
    }
    wall = vertex = false;
    for(int c = (*it)->getData().getX()+1; c < _columnas and (!wall and ! vertex); c++){
      if (_levels[(*it)->getData().getZ()][c]==1){wall = true;}
      else if((aux = _level->getVertex((*it)->getData().getZ(), c))!=NULL){
	vertex = true;
	_level->addEdge((*it), aux);
      }
    }
    wall = vertex = false;
  }
}

void
PlayState::exit ()
{
  for(int i = 0; i < _filas; i++){
    delete [] _levels[i];
  }
  delete [] _levels;

  _ovJuego->hide();
  
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
PlayState::pause()
{
}

void
PlayState::resume()
{
  if(_exitGame){changeState(MenuState::getSingletonPtr());}
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
  _pacMove.x = 0; _pacMove.y = 0; _pacMove.z = 0;
  _deltaT = evt.timeSinceLastFrame;
  _ovScore->setCaption(Ogre::StringConverter::toString(_score));
  if(!_exitGame and !_endLevel){
    pacmanMove();
    ghostMove(_blinky);
    ghostMove(_pinky);
    ghostMove(_inky);
    ghostMove(_clyde);
  }
  else if(!_exitGame){//nivel terminado
    nextLevel();
  }
  //else{return false;}
  return true;
}

void
PlayState::pacmanMove()
{
  if(hit(_blinky)){
    if(_blinky->canEat()){_blinky->setCanEat(false);resetGhost(_blinky);_score+=50;}
    else{died();}
  }if(hit(_inky)){
    if(_inky->canEat()){_inky->setCanEat(false);resetGhost(_inky);_score+=50;}
    else{died();}
  }if(hit(_pinky)){
    if(_pinky->canEat()){_pinky->setCanEat(false);resetGhost(_pinky);_score+=50;}
    else{died();}
  }if(hit(_clyde)){
    if(_clyde->canEat()){_clyde->setCanEat(false);resetGhost(_clyde);_score+=50;}
    else{died();}
  }
  else{    
    if(_currentDir == 1 and !colisionMap(0, _pacman)){_pacMove.x = 1;}
    else if(_currentDir == 3 and !colisionMap(1, _pacman)){_pacMove.x = -1;}
    else if(_currentDir == 4 and !colisionMap(2, _pacman)){_pacMove.z = -1;}
    else if(_currentDir == 2 and !colisionMap(3, _pacman)){_pacMove.z = 1;}
    _pacman->translate(_pacMove*_deltaT*_pacSpeed);
    _currentRow = (int)(_pacman->getPosition().z+_startRow+0.5);
    _currentCol = (int)(_pacman->getPosition().x+_startCol);
    if(_pacSpeed>2.5){_pacSpeed-=0.0015;}
    else{canEat(false);eating();}
    if (_levels[(int)_currentRow][(int)_currentCol]==2){
      Ogre::SceneNode* nodo = NULL;
      std::stringstream bloq;
      bloq << "Pac-dot(" << (int)_currentRow << "," << (int)_currentCol << ")";
      try{nodo = _sceneMgr->getSceneNode(bloq.str());}catch(...){}
      if(nodo){
	Ogre::AxisAlignedBox bboxPac = _pacman->_getWorldAABB();
	Ogre::AxisAlignedBox bboxDot = nodo->_getWorldAABB();
	if(bboxPac.intersects(bboxDot)){
	  nodo->removeAndDestroyAllChildren();
	  _sceneMgr->destroySceneNode(nodo);
	  _nPacDots--;
	  _score += 10;
	  _blinky->setSpeed(_blinky->getSpeed()+0.005);
	}
	if(_nPacDots<1){_endLevel = true;}
      }
    }
    else if (_levels[(int)_currentRow][(int)_currentCol]==3){
      Ogre::SceneNode* nodo = NULL;
      std::stringstream bloq;
      bloq << "Power-Pellet(" << (int)_currentRow << "," << (int)_currentCol << ")";
      try{nodo = _sceneMgr->getSceneNode(bloq.str());}catch(...){}
      if(nodo){
	Ogre::AxisAlignedBox bboxPac = _pacman->_getWorldAABB();
	Ogre::AxisAlignedBox bboxDot = nodo->_getWorldAABB();
	if(bboxPac.intersects(bboxDot)){
	  nodo->removeAndDestroyAllChildren();
	  _sceneMgr->destroySceneNode(nodo);
	  _pacSpeed = 3.5;
	  //_endLevel = true;//para probar a pasar de nivel rapido
	  canEat(true);
	  eating();
	}
      }
    }
    else if (_levels[(int)_currentRow][(int)_currentCol]==6){
      Ogre::SceneNode* nodo = NULL;
      std::stringstream bloq;
      bloq << "Transporter(" << (int)_currentRow << "," << (int)_currentCol << ")";
      nodo = _sceneMgr->getSceneNode(bloq.str());
      Ogre::AxisAlignedBox bboxPac = _pacman->_getWorldAABB();
      Ogre::AxisAlignedBox bboxDot = nodo->_getWorldAABB();
      if(bboxPac.intersects(bboxDot)){
	if((int)_currentCol > 25){_pacman->translate(-26,0,0);}
	else if((int)_currentCol<4){_pacman->translate(26,0,0);}
	else if((int)_currentRow<(_currentLevel-1)*31+4){_pacman->translate(0,0,29);}
	else{_pacman->translate(0,0,-29);}
      }
    }
  }
}
void
PlayState::canEat(bool b)
{
  _blinky->setCanEat(b);_pinky->setCanEat(b);_inky->setCanEat(b);_clyde->setCanEat(b);_canEat=b;
}
void
PlayState::eating()
{
  Ogre::Entity* pieza;
  if(_blinky->canEat()){
    pieza = static_cast<Ogre::Entity*>(_blinky->getNode()->getAttachedObject(0));
    pieza->setMaterialName("F_comer_mat");
  }
  else{
    pieza = static_cast<Ogre::Entity*>(_blinky->getNode()->getAttachedObject(0));
    pieza->setMaterialName("F_rojo_mat");
  }
  if(_inky->canEat()){
    pieza = static_cast<Ogre::Entity*>(_inky->getNode()->getAttachedObject(0));
    pieza->setMaterialName("F_comer_mat");
  }
  else{ 
    pieza = static_cast<Ogre::Entity*>(_inky->getNode()->getAttachedObject(0));
    pieza->setMaterialName("F_azul_mat");
  }
  if(_pinky->canEat()){
    pieza = static_cast<Ogre::Entity*>(_pinky->getNode()->getAttachedObject(0));
    pieza->setMaterialName("F_comer_mat");
  }
  else{
    pieza = static_cast<Ogre::Entity*>(_pinky->getNode()->getAttachedObject(0));
    pieza->setMaterialName("F_rosa_mat");
  }
  if(_clyde->canEat()){
    pieza = static_cast<Ogre::Entity*>(_clyde->getNode()->getAttachedObject(0));
    pieza->setMaterialName("F_comer_mat");
  }
  else{
    pieza = static_cast<Ogre::Entity*>(_clyde->getNode()->getAttachedObject(0));
    pieza->setMaterialName("F_naranja_mat");
  }
}
void
PlayState::died()
{
  if(_lifes.size()>1){
      _lifes.back()->removeAndDestroyAllChildren();
      _sceneMgr->destroySceneNode(_lifes.back());
      _lifes.pop_back();
      _pacman->setPosition(_startPos);
      _currentDir = _prevDir = 0;
    }
  else{_exitGame = true;}
}
bool
PlayState::hit(Ghost* ghost)
{
  Ogre::AxisAlignedBox bboxPac = _pacman->_getWorldAABB();
  Ogre::AxisAlignedBox bbox = ghost->getNode()->_getWorldAABB();
  if(bboxPac.intersects(bbox))
    return true;
  return false;
}
void
PlayState::resetGhost(Ghost* ghost)
{
  eating();
  ghost->getNode()->setPosition(ghost->getStartPos());
  ghost->setMove(Ogre::Vector3(0,0,0));
}
void
PlayState::ghostMove(Ghost* ghost)
{
  std::vector<GraphVertex*>vertexes;
  GraphVertex* vert;
  GraphVertex* next;
  //int dist = 10000;
  double intX, intZ;
  double decX = 0.0;
  double decZ = 0.0;
  
  decX = (std::modf(ghost->getNode()->getPosition().x, &intX));
  decZ = (std::modf(ghost->getNode()->getPosition().z, &intZ));
  if(((std::abs(decZ) <_deltaT) or (std::abs(decZ) >1-_deltaT)) and
     ((std::abs(decX) <_deltaT) or (std::abs(decX) >1-_deltaT)) and
     (vert = _level->getVertex(ghost->getStart().first+ghost->getNode()->getPosition().z+_deltaT,
			       ghost->getStart().second+ghost->getNode()->getPosition().x+_deltaT))!=NULL){
    vertexes = _level->getLinks(vert);
    next = vertexes.at(rand()%vertexes.size());
    /*if(_canEat)
      dist = -10000;
    for(std::vector<GraphVertex*>::const_iterator it = vertexes.begin(); it != vertexes.end(); ++it){
      if(true){
	if(std::abs(_pacman->getPosition().z - ((*it)->getData().getZ()-12))+std::abs(_pacman->getPosition().x - ((*it)->getData().getX()-14)) < dist){
	  dist = std::abs( _pacman->getPosition().z - ((*it)->getData().getZ()-12))+std::abs(_pacman->getPosition().x - ((*it)->getData().getX()-14));
	  next = (*it);
	}
      }
      else{
	if(std::abs(_pacman->getPosition().z - ((*it)->getData().getZ()-12))+std::abs(_pacman->getPosition().x - ((*it)->getData().getX()-14)) > dist){
	  dist = std::abs( _pacman->getPosition().z - ((*it)->getData().getZ()-12))+std::abs(_pacman->getPosition().x - ((*it)->getData().getX()-14));
	  next = (*it);
	}
      }
      }*/
    ghost->setMove(_level->getMove(vert, next));//vertexes.at(rand()%vertexes.size())));
    if(ghost->getMove().x==0){
      if(std::abs(decX) > 0 and std::abs(decX)<0.5)
	ghost->getNode()->setPosition(Ogre::Vector3(ghost->getNode()->getPosition().x-decX, 0.5, ghost->getNode()->getPosition().z));
      else if(decX >0.5)
	ghost->getNode()->setPosition(Ogre::Vector3(ghost->getNode()->getPosition().x-decX+1, 0.5, ghost->getNode()->getPosition().z));
      else if(decX <-0.5)
	ghost->getNode()->setPosition(Ogre::Vector3(ghost->getNode()->getPosition().x-decX-1, 0.5, ghost->getNode()->getPosition().z));
    }
    else if(ghost->getMove().z==0){
      if(std::abs(decZ) > 0 and std::abs(decZ)<0.5)
	ghost->getNode()->setPosition(Ogre::Vector3(ghost->getNode()->getPosition().x, 0.5,ghost->getNode()->getPosition().z-decZ));
      else if(decZ > 0.5)
	ghost->getNode()->setPosition(Ogre::Vector3(ghost->getNode()->getPosition().x, 0.5,ghost->getNode()->getPosition().z-decZ+1));
      else if(decZ < -0.5)
	ghost->getNode()->setPosition(Ogre::Vector3(ghost->getNode()->getPosition().x, 0.5,ghost->getNode()->getPosition().z-decZ-1));
    }
  }
  ghost->getNode()->translate(ghost->getMove()*_deltaT*ghost->getSpeed());
}

void
PlayState::nextLevel()
{
  _currentLevel = (_currentLevel+1);
  if(_currentLevel>3){_exitGame = true;}
  else{
    removeLevel();createScene();
    _currentDir=_prevDir=0;
  }
}
void
PlayState::removeLevel()
{
  _pacman->removeAndDestroyAllChildren();
  _level->getVertexes().clear();
  _level->getEdges().clear();
  destroyAllAttachedMovableObjects(_sceneMgr->getRootSceneNode());
  _sceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
  _endLevel = false;
}
void
PlayState::destroyAllAttachedMovableObjects(Ogre::SceneNode* node)
{
   if(!node) return;

   Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

   while (itObject.hasMoreElements()){
      node->getCreator()->destroyMovableObject(itObject.getNext());
   }

   Ogre::SceneNode::ChildNodeIterator itChild = node->getChildIterator();

   while (itChild.hasMoreElements()){
      Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
      destroyAllAttachedMovableObjects(pChildNode);
   }
}

bool
PlayState::colisionMap(int dir, Ogre::SceneNode* node)
{
  
  Ogre::AxisAlignedBox bboxPac = node->_getWorldAABB();
  Ogre::AxisAlignedBox bboxWall;
  Ogre::SceneNode* aux;
  std::stringstream wall;
  bool hit = false;
  for(int i = 0;!hit and (unsigned) i < _wallRows->size(); i++){
    wall.str("");
    wall << "Wall(" << _wallRows->at(i) << "," << _wallCols->at(i) << ")";
    aux = _sceneMgr->getSceneNode(wall.str());
    bboxWall = aux->_getWorldAABB();
    if(bboxPac.intersects(bboxWall)){
      switch(dir){
      case 0:
	if(_currentCol < _wallCols->at(i)){
	  node->translate(-0.125, 0.0, 0.0);
	}
	break;
      case 1:
	if(_currentCol > _wallCols->at(i)){
	  node->translate(0.125, 0.0, 0.0);
	}
	break;
      case 2:
	if(_currentRow > _wallRows->at(i)){
	  node->translate(0.0, 0.0, 0.125);
	}
	break;
      case 3:
	if(_currentRow < _wallRows->at(i)){
	  node->translate(0.0, 0.0, -0.125);
	}
	break;
      }
      hit = true;
      _hits++;
      if(_hits>1){_currentDir=0;_prevDir=0;}
      else{_currentDir = _prevDir;}
    }
  }
  return hit;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame){
    EndState::getSingletonPtr()->addScore(_score);
    pushState(EndState::getSingletonPtr());
    //return false;
  }

  if(_currentRow != _prevRow){_hits = 0;_prevRow = _currentRow;_prevDir = 0;}
  if(_currentCol != _prevCol){_hits = 0;_prevCol = _currentCol;_prevDir = 0;}
  return true;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
  switch(e.key){
  case OIS::KC_SPACE:
    pushState(PauseState::getSingletonPtr());
    break;
  case OIS::KC_ESCAPE://overlay?
    break;
  case OIS::KC_P:
    break;
  case OIS::KC_C:
      _perspective = (_perspective+1) % 2;
      switch(_perspective){
      case 0:
	//Vista aerea
	_camera->setPosition(Ogre::Vector3(0, 42, 7));
	_camera->lookAt(Ogre::Vector3(0, -50, 0));
	break;
      case 1:
	//vista 3D
	_camera->setPosition(Ogre::Vector3(0, 32, 37));
	_camera->lookAt(Ogre::Vector3(0, 0, 0));
	break;
      }
      break;
  case OIS::KC_RIGHT:
    _pacman->lookAt(Ogre::Vector3(0,0,999), _pacman->TS_WORLD);
    _prevDir = _currentDir;
    _currentDir = 1;
    _nextDir = 1;
    break;
  case OIS::KC_LEFT:
    _pacman->lookAt(Ogre::Vector3(0,0,-999), _pacman->TS_WORLD);
    _prevDir = _currentDir;
    _currentDir = 3;
    _nextDir = 3;
    break;
  case OIS::KC_UP:
    _pacman->lookAt(Ogre::Vector3(-999,0,0), _pacman->TS_WORLD);
    _prevDir = _currentDir;
    _currentDir = 4;
    _nextDir = 4;
    break;
  case OIS::KC_DOWN:
    _pacman->lookAt(Ogre::Vector3(999,0,0), _pacman->TS_WORLD);
    _prevDir = _currentDir;
    _currentDir = 2;
    _nextDir = 2;
    break;
  default:
    break;
  }
}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
  switch(e.key){
  case OIS::KC_ESCAPE:
    _exitGame = true;
    break;
  default:
    break;
  }
}

PlayState*
PlayState::getSingletonPtr ()
{
return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void
PlayState::LoadLevels()
{
  std::ifstream file("niveles.txt");
  int i = 0, j = 0;
  _nwalls = 0;
  std::string str;
  if(file.is_open()){
    _filas = 124; _columnas = 28;//cada nivel tiene 31 filas
    _levels = new int*[_filas];
    while(getline(file, str)){
      _levels[i] = new int[_columnas];
      for(std::string::iterator it = str.begin(); it != str.end(); ++it){
	_levels[i][j] = *it-48;//'0' = 48 en ascii
	if(*it-48==1){_nwalls++;}
	j++;
      }
      i++;
      j = 0;
    }
    file.close();
  }
}
