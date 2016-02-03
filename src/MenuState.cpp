#include "MenuState.h"
#include "PlayState.h"
#include <iostream> 


template<> MenuState* Ogre::Singleton<MenuState>::msSingleton = 0;

void
MenuState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();


  _sceneMgr = _root->getSceneManager("SceneManager");

  _camera = _sceneMgr->getCamera("IntroCamera");

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

  createScene();
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
  //_simpleEffect=
  //_pSoundFXManager->load("FXMovPointer.ogg");
  /* Iniciamos la variable de las opciones */
  _sel = 0;
  _opciones = new Ogre::SceneNode*[4];

  /* Posicion y vision de la camara */
  _camera->setPosition(Ogre::Vector3(3, 2, 10));
  _camera->lookAt(Ogre::Vector3(3, 2, 0));

  /* Creacion de las opciones del menu */
  for(int i = 0; i < 4; i++){
    /* Cogemos el nombre de la que sera la opcion a crear */
    nombre_act << nombres[i];

    /* La agregamos al scenenode del menu */
    nodoAux = _nodo->createChildSceneNode(nombre_act.str(), Ogre::Vector3(0, (3 - i), 0));

    /* Creamos el nombre del mesh */
    nombre_act << ".mesh";

    /* Lo agregamos al nodo creado */
    ent = _sceneMgr->createEntity(nombre_act.str());
    nodoAux->attachObject(ent);

    /* Guardamos valores y reiniciamos variables de apoyo */
    _opciones[i] = nodoAux;
    nombre_act.str("");
  }//Fin for

  nodoAux = _sceneMgr->getRootSceneNode()->createChildSceneNode("Titulo", Ogre::Vector3(6, 4.5  , 0));
  ent = _sceneMgr->createEntity("Titulo.mesh");
  nodoAux->attachObject(ent);

  /* Puntero */
  nodoAux = _nodo->createChildSceneNode("Puntero", Ogre::Vector3(-0.6, 3.2, 0));
  ent = _sceneMgr->createEntity("Puntero.mesh");
  nodoAux->attachObject(ent);
    
  /*Background*/ 
  nodoAux = _sceneMgr->getRootSceneNode()->createChildSceneNode("BMenu", Ogre::Vector3(-0.5, 0, -9));
  ent = _sceneMgr->createEntity("Backmenu.mesh");
  nodoAux->pitch(Ogre::Degree(90));
  nodoAux->attachObject(ent);
    

  /* Iluminacion */
  Ogre::Light *light = _sceneMgr->createLight("Light");
  light->setType(Ogre::Light::LT_POINT);
  light->setPosition(0, 4, 5);
  light->setSpecularColour(0.9, 0.9, 0.9); 
  light->setDiffuseColour(0.9, 0.9, 0.9);
}//Fin createScene

bool
MenuState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  return true;
}

bool
MenuState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
MenuState::keyPressed
(const OIS::KeyEvent &e)
{
  /* Valores maximos y minimos */
  Ogre::Real min = 0.2;
  Ogre::Real max = 3.2;

  /* Cogemos el scenenode del puntero */
  Ogre::Node* p = _nodo->getChild("Puntero");
  /* Posicion actual del puntero */
  Ogre::Vector3 pos = p->getPosition();

  switch(e.key){
      
  case OIS::KC_RETURN:
    switch(_sel){
    case 0:
      changeState(PlayState::getSingletonPtr());
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      _exitGame = true;
    }
    break;
  case OIS::KC_UP:
    if(!_op){
      //std::cout << "Hola 1" << std::endl;
      if((pos[1] + 1) <= max){
	_sel -= 1;
	p->setPosition(pos[0], (pos[1] + 1), pos[2]);
      } else {
	_sel = 3;
	p->setPosition(pos[0], min, pos[2]);
      }//Fin if_else
    }//Fin if
    break;
  case OIS::KC_DOWN:
    if(!_op){
      if((pos[1] - 1) > 0.09){
	_sel += 1;
	p->setPosition(pos[0], (pos[1] - 1), pos[2]);
      } else {
	_sel = 0;
	p->setPosition(pos[0], max, pos[2]);
      }//Fin if_else
    }//Fin if
    break;
  case OIS::KC_SPACE:
    switch(_sel){
    case 0:
      changeState(PlayState::getSingletonPtr());
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      _exitGame = true;
    }
    break;
  case OIS::KC_ESCAPE:
    _exitGame = true;
          
  default:
    /* Cualquier otra tecla no hace nada */
    break;
  }//Fin switch
  // Transición al siguiente estado.
  // Espacio --> PlayState
}

void
MenuState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void
MenuState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
MenuState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
MenuState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
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
