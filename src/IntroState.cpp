#include "IntroState.h"
#include "MenuState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void
IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();


  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  
  _camera = _sceneMgr->createCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(0, 0, 0));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

  
  RecordManager::getSingletonPtr()->loadRecords();
  createScene();
  _exitGame = false;
}
void
IntroState::exit()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
}

void
IntroState::createScene()
{
    Ogre::Entity *ent = NULL;
    Ogre::SceneNode *nodo = NULL;
    

    _camera->setPosition(Ogre::Vector3(0, 0, 10));
    _camera->lookAt(Ogre::Vector3(0, 2, 0));
    
    nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(
        "Background", Ogre::Vector3(0, 2.5, -2));
    ent = _sceneMgr->createEntity("Background.mesh");
    nodo->attachObject(ent);
    
    nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(
        "Titulo", Ogre::Vector3(2.75, 2, 0));
    ent = _sceneMgr->createEntity("Titulo.mesh");
    nodo->attachObject(ent);

    nodo = _sceneMgr->getRootSceneNode()->createChildSceneNode(
    "Pulsar", Ogre::Vector3(-2.0, 0, 0));
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
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_RETURN or e.key == OIS::KC_SPACE) {
    changeState(MenuState::getSingletonPtr());
  }
}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
