#include "MenuState.h"
#include "PlayState.h"
#include <iostream> 


template<> MenuState* Ogre::Singleton<MenuState>::msSingleton = 0;

void
MenuState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();
  _sceneMgr = _root->getSceneManager("SceneManager");

  _sceneMgr->addRenderQueueListener(GameManager::getSingletonPtr()->getOverlaySystem());
  _overlayManager = Ogre::OverlayManager::getSingletonPtr();

  _camera = _sceneMgr->getCamera("IntroCamera");

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

  createScene();
  createOverlay();
  _exitGame = false; _op=false;
}
void
MenuState::exit()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
MenuState::pause ()
{
}

void
MenuState::resume ()
{
}

void
MenuState::createScene()
{
  Ogre::Entity *ent = NULL;
  _nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode("OpMenu", Ogre::Vector3(0, 0, 0));
  Ogre::SceneNode *nodoAux = NULL;

  std::stringstream nombre_act;
  Ogre::String nombres[] = {"Jugar", "Records", "Creditos", "Salir"};
  _sel = 0;
  _opciones = new Ogre::SceneNode*[4];
  _camera->setPosition(Ogre::Vector3(3, 2, 10));
  _camera->lookAt(Ogre::Vector3(3, 2, 0));
  
  for(int i = 0; i < 4; i++){
    nombre_act << nombres[i];
    nodoAux = _nodo->createChildSceneNode(nombre_act.str(), Ogre::Vector3(0, (3 - i), 0));
    nombre_act << ".mesh";
    ent = _sceneMgr->createEntity(nombre_act.str());
    nodoAux->attachObject(ent);
    _opciones[i] = nodoAux;
    nombre_act.str("");
  }

  nodoAux = _sceneMgr->getRootSceneNode()->createChildSceneNode("Titulo", Ogre::Vector3(6, 4.5  , 0));
  ent = _sceneMgr->createEntity("Titulo.mesh");
  nodoAux->attachObject(ent);
  nodoAux = _nodo->createChildSceneNode("Puntero", Ogre::Vector3(-0.6, 3.2, 0));
  ent = _sceneMgr->createEntity("Puntero.mesh");
  nodoAux->attachObject(ent);
  nodoAux = _sceneMgr->getRootSceneNode()->createChildSceneNode("BMenu", Ogre::Vector3(-0.5, 0, -9));
  ent = _sceneMgr->createEntity("Backmenu.mesh");
  nodoAux->pitch(Ogre::Degree(90));
  nodoAux->attachObject(ent);
  Ogre::Light *light = _sceneMgr->createLight("Light");
  light->setType(Ogre::Light::LT_POINT);
  light->setPosition(0, 4, 5);
  light->setSpecularColour(0.9, 0.9, 0.9); 
  light->setDiffuseColour(0.9, 0.9, 0.9);
  light->setCastShadows(false);
}

void 
MenuState::createOverlay()
{
  _ovCreditos = _overlayManager->getByName("Creditos");
  _ovRecords = _overlayManager->getByName("Records");
}

bool
MenuState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  if(_ovRecords->isVisible()){
    showRecords();
  }
  return true;
}

bool
MenuState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame){
    RecordManager::getSingletonPtr()->saveRecords();
    return false;
  }
  
  return true;
}

void MenuState::menuActions(){
  switch(_sel){
  case 0:
    changeState(PlayState::getSingletonPtr());
    break;
  case 1:
    _op = true;
    _ovRecords->show();
    break;
  case 2:
    _op = true;
    _ovCreditos->show();
    break;
  case 3:
    _exitGame = true;
    break;
  }
}
void MenuState::showRecords(){
  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("RecordsText");
  oe->setCaption(RecordManager::getSingletonPtr()->toString());
}
void
MenuState::keyPressed
(const OIS::KeyEvent &e)
{
  Ogre::Real min = 0.2;
  Ogre::Real max = 3.2;
  Ogre::Node* p = _nodo->getChild("Puntero");
  Ogre::Vector3 pos = p->getPosition();

  switch(e.key){
      
  case OIS::KC_RETURN:
    if(!_op){
      menuActions();
    }
    break;
  case OIS::KC_UP:
    if(!_op){
      if((pos[1] + 1) <= max){
	_sel -= 1;
	p->setPosition(pos[0], (pos[1] + 1), pos[2]);
      } else {
	_sel = 3;
	p->setPosition(pos[0], min, pos[2]);
      }
    }
    break;
  case OIS::KC_DOWN:
    if(!_op){
      if((pos[1] - 1) > 0.09){
	_sel += 1;
	p->setPosition(pos[0], (pos[1] - 1), pos[2]);
      } else {
	_sel = 0;
	p->setPosition(pos[0], max, pos[2]);
      }
    }
    break;
  case OIS::KC_SPACE:
    if(!_op){
      menuActions();
    }
    break;
  case OIS::KC_ESCAPE:
    if(_op){
      _op = false;
      if(_ovCreditos->isVisible()){
        _ovCreditos->hide();
      } else {
        if(_ovRecords->isVisible())
	     _ovRecords->hide();
      }
    }
    break;
          
  default:
    break;
  }
}

void
MenuState::keyReleased
(const OIS::KeyEvent &e )
{
}
MenuState*
MenuState::getSingletonPtr ()
{
  return msSingleton;
}

MenuState&
MenuState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
