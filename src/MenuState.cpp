#include "MenuState.h"
#include "PlayState.h"

template<> MenuState* Ogre::Singleton<MenuState>::msSingleton = 0;

void
MenuState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();


  _sceneMgr = _root->getSceneManager("SceneManager");

  _camera = _sceneMgr->createCamera("MenuCamera");
  _camera->setPosition(Ogre::Vector3(0, 0, 0));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

  createScene();
  _exitGame = false;
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
    Ogre::SceneNode *nodo = NULL;
    

    _camera->setPosition(Ogre::Vector3(0, 0, 10));
    _camera->lookAt(Ogre::Vector3(0, 2, 0));
    
    /*nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(
        "BackMenu", Ogre::Vector3(0, 0, -2));
    ent = _sceneMgr->createEntity("BMenu.mesh");
    nodo->attachObject(ent);
    
    nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(
        "Titulo", Ogre::Vector3(-0.5, 2, 0));
    ent = _sceneMgr->createEntity("Titulo.mesh");
    nodo->attachObject(ent);*/

    nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(
    "Pulsar", Ogre::Vector3(0, 0, 0));
    ent = _sceneMgr->createEntity("Pulsar.mesh");
    nodo->attachObject(ent);
    nodo->yaw(Ogre::Degree(-90), Ogre::Node::TS_LOCAL);

    /* Iluminacion */
    Ogre::Light *light = _sceneMgr->createLight("Light");
    light->setType(Ogre::Light::LT_POINT);
    light->setPosition(0, 5, 5);
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
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    changeState(PlayState::getSingletonPtr());
  }
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
