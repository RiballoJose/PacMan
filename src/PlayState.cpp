#include "PlayState.h"
#include "PauseState.h"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _currentLevel = 1;
  _pacSpeed = 3;
  _currentDir = _prevDir = _prevCol = _prevRow = 0;
		  
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);

  LoadLevels();
  _blqEstructura = _sceneMgr->getRootSceneNode()
    ->createChildSceneNode("Walls", Ogre::Vector3(0, 0, 0));
  createScene();

  _endPGame=_iniJuego= false;
  _endGame = _endLevel = _exitGame = false;
}
void
PlayState::createScene()
{
  Ogre::Entity* ent = NULL;
  Ogre::SceneNode* nodo = NULL;
  std::stringstream bloq, material;
  bloq.str("");
  //Vista aerea
  _perspective = 0;
  _camera->setPosition(Ogre::Vector3(0, 42, 7));
  _camera->lookAt(Ogre::Vector3(0, -50, 0));
  
  nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode("Escenario", Ogre::Vector3(0, 0, 0));
  ent = _sceneMgr->createEntity("Base.mesh");
  nodo->translate(-0.5,0.0,3.0);
  nodo->attachObject(ent);

  int aux = -13;
  for(int i = 0; i < 3; i++){
    bloq << "Life_" << i;
    nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode((bloq.str()),Ogre::Vector3(aux, 0.5, 19.5));
    ent = _sceneMgr->createEntity("Nave.mesh");
    nodo->setScale(0.075, 0.5, 0.375);
    nodo->attachObject(ent);
    _lifes.push_back(nodo);

    aux += 1; bloq.str("");
  }
  aux = -14;
  _wallRows = new std::vector<int>();
  _wallCols = new std::vector<int>();
  
  for(int f = (_currentLevel-1)*(31); f < (_currentLevel-1)*(31)+31; f++){
    for(int c = 0; c < _columnas; c++){
      bloq.str("");
      switch(_levels[f][c]){
      case 0://vacio
	bloq << "Void(" << f << "," << c << ")";
	break;
      case 1://muros
	bloq << "Wall(" << f << "," << c << ")";
	nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-(_currentLevel-1)*31))-12)));
	ent = _sceneMgr->createEntity(bloq.str(), "Muro.mesh");
	//ent->setMaterialName(material.str());
	nodo->setScale(0.5, 0.5, 0.5);
	nodo->attachObject(ent);
	_wallRows->push_back(f);
	_wallCols->push_back(c);
	break;
      case 2://Pac-dots segun wikipedia es la comida
	bloq << "Pac-dot(" << f << "," << c << ")";
	nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-(_currentLevel-1)*31))-12)));
	ent = _sceneMgr->createEntity(bloq.str(), "Bola.mesh");
	//ent->setMaterialName(material.str());
	nodo->setScale(0.5, 0.5, 0.5);
	nodo->attachObject(ent);
	break;
      case 3://comefantasmas
	bloq << "Power-Pellet(" << f << "," << c << ")";
	nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-(_currentLevel-1)*31))-12)));
	ent = _sceneMgr->createEntity(bloq.str(), "Bola.mesh");
	//ent->setMaterialName(material.str());
	nodo->setScale(1.0, 1.0, 1.0);
	nodo->attachObject(ent);
	break;
      case 4://zona de fantasmas
	bloq << "Home(" << f << "," << c << ")";
	break;
      case 5://donde empieza el pacman->cuidado:tenemos dos 5s
	if(!_pacman){
	  bloq << "Start(" << f << "," << c << ")";
	  _pacman = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-(_currentLevel-1)*31))-12)));
	  ent = _sceneMgr->createEntity(bloq.str(), "Nave.mesh");
	  _pacman->setScale(0.15, 0.5, 0.75);
	  _pacman->translate(0.5,0,0.0);
	  _pacman->attachObject(ent);
	  _startRow = f-_pacman->getPosition().z;
	  _startCol = (c-_pacman->getPosition().x)+1;
	  _currentRow = _startRow;
	  _currentCol = _startCol;
	}
	break;
      case 6:
	bloq << "Transporter(" << f << "," << c << ")";
	nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(bloq.str(), Ogre::Vector3(aux, 0.5, (((f-(_currentLevel-1)*31))-12)));
	ent = _sceneMgr->createEntity(bloq.str(), "Muro.mesh");
	//ent->setMaterialName(material.str());
	nodo->setScale(0.2, 0.2, 0.2);
	nodo->attachObject(ent);
	nodo->setVisible(false);
	break;
      default:
	break;
      }
      aux+=1;
    }
    aux = -14;
  }
  _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
  _sceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));
  _sceneMgr->setShadowTextureCount(2);
  _sceneMgr->setShadowTextureSize(512);

  Ogre::Light *light = _sceneMgr->createLight("Light");
  light->setType(Ogre::Light::LT_SPOTLIGHT);
  light->setDirection(Ogre::Vector3(0,-1,0));
  light->setSpotlightInnerAngle(Ogre::Degree(25.0f));
  light->setSpotlightOuterAngle(Ogre::Degree(200.0f));
  light->setPosition(0, 150, 0);
  light->setSpecularColour(1, 1, 1);
  light->setDiffuseColour(1, 1, 1);
  light->setSpotlightFalloff(5.0f);
  light->setCastShadows(true);
}

void
PlayState::exit ()
{
  for(int i = 0; i < _filas; i++){
    delete [] _levels[i];
  }
  delete [] _levels;
  
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
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
  _move.x = 0; _move.y = 0; _move.z = 0;
  _deltaT = evt.timeSinceLastFrame;
  if(!_endGame || !_endLevel){
    /*if(_perspective==2){
      _camera->setPosition(_pacman->getPosition()+Ogre::Vector3(0,0,0));
      switch(_currentDir){
      case 1:_camera->lookAt(Ogre::Vector3(200,0,0));break;
      case 2:_camera->lookAt(Ogre::Vector3(-200,0,0));break;
      case 3:_camera->lookAt(Ogre::Vector3(0,0,-200));break;
      case 4:_camera->lookAt(Ogre::Vector3(0,0,200));break;
      }
      }*/
    if(_currentDir == 1 and !colisionMap(0)){_move.x = 1;}
    else if(_currentDir == 2 and !colisionMap(1)){_move.x = -1;}
    else if(_currentDir == 3 and !colisionMap(2)){_move.z = -1;}
    else if(_currentDir == 4 and !colisionMap(3)){_move.z = 1;}
    _pacman->translate(_move*_deltaT*_pacSpeed);
    _currentRow = (int)(_pacman->getPosition().z+_startRow+0.5);
    _currentCol = (int)(_pacman->getPosition().x+_startCol);
    if(_pacSpeed>3){_pacSpeed-=0.001;}
    if (_levels[(int)_currentRow][(int)_currentCol]==2){
      Ogre::SceneNode* nodo = NULL;
      std::stringstream bloq;
      bloq << "Pac-dot(" << (int)_currentRow << "," << (int)_currentCol << ")";
      nodo = _sceneMgr->getSceneNode(bloq.str());
      Ogre::AxisAlignedBox bboxPac = _pacman->_getWorldAABB();
      Ogre::AxisAlignedBox bboxDot = nodo->_getWorldAABB();
      if(bboxPac.intersects(bboxDot)){nodo->setVisible(false);}
    }
    else if (_levels[(int)_currentRow][(int)_currentCol]==3){
      Ogre::SceneNode* nodo = NULL;
      std::stringstream bloq;
      bloq << "Power-Pellet(" << (int)_currentRow << "," << (int)_currentCol << ")";
      nodo = _sceneMgr->getSceneNode(bloq.str());
      Ogre::AxisAlignedBox bboxPac = _pacman->_getWorldAABB();
      Ogre::AxisAlignedBox bboxDot = nodo->_getWorldAABB();
      if(bboxPac.intersects(bboxDot)){//habria que cambiar fantasmas y poder comer
	nodo->setVisible(false);
	_pacSpeed = 5;
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
  return true;
}

bool
PlayState::colisionMap(int dir)
{
  
  Ogre::AxisAlignedBox bboxPac = _pacman->_getWorldAABB();
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
	  hit = true;
	  _pacman->translate(-0.075, 0.0, 0.0);
	}
	break;
      case 1:
	if(_currentCol > _wallCols->at(i)){
	  hit = true;
	  _pacman->translate(0.075, 0.0, 0.0);
	}
	break;
      case 2:
	if(_currentRow > _wallRows->at(i)){
	  hit = true;
	  _pacman->translate(0.0, 0.0, 0.075);
	}
	break;
      case 3:
	if(_currentRow < _wallRows->at(i)){
	  hit = true;
	  _pacman->translate(0.0, 0.0, -0.075);
	}
	break;
      }
      _currentDir = _prevDir;
      //_prevDir = 0;_currentDir = 0;
    }
  }
  return hit;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;

  if(_currentRow != _prevRow){_prevRow = _currentRow;_prevDir = 0;}
  if(_currentCol != _prevCol){_prevCol = _currentCol;_prevDir = 0;}
  return true;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
  switch(e.key){
  case OIS::KC_SPACE:
    break;
  case OIS::KC_P:
      pushState(PauseState::getSingletonPtr());
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
      case 2:
	_camera->setPosition(_pacman->getPosition());
	_camera->lookAt(Ogre::Vector3(0, 0, 0));
	break;
      }
      break;
  case OIS::KC_RIGHT:
    _prevDir = _currentDir;
    _currentDir = 1;
    break;
  case OIS::KC_LEFT:
    _prevDir = _currentDir;
    _currentDir = 2;
    break;
  case OIS::KC_UP:
    _prevDir = _currentDir;
    _currentDir = 3;
    break;
  case OIS::KC_DOWN:
    _prevDir = _currentDir;
    _currentDir = 4;
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

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
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
