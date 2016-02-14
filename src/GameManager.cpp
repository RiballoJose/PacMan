#include <Ogre.h>

#include "GameManager.h"
#include "GameState.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

template<> GameManager* Ogre::Singleton<GameManager>::msSingleton = 0;

GameManager::GameManager ()
{
  _root = 0;
}

GameManager::~GameManager ()
{
  while (!_states.empty()) {
    _states.top()->exit();
    _states.pop();
  }
  
  if (_root)
    delete _root;
}

void
GameManager::start
(GameState* state)
{
  // Creación del objeto Ogre::Root.
  _root = new Ogre::Root();
  initSDL();
  _overlaySystem = new Ogre::OverlaySystem();
  _pTrackManager=new TrackManager();
  _pSoundFXManager=new SoundFXManager();
  loadResources();

  if (!configure())
    return;    
  	
  _inputMgr = new InputManager;
  _inputMgr->initialise(_renderWindow);

  // Registro como key y mouse listener...
  _inputMgr->addKeyListener(this, "GameManager");

  // El GameManager es un FrameListener.
  _root->addFrameListener(this);

  // Transición al estado inicial.
  changeState(state);

  // Bucle de rendering.
  _root->startRendering();
}

void
GameManager::changeState
(GameState* state)
{
  // Limpieza del estado actual.
  if (!_states.empty()) {
    // exit() sobre el último estado.
    _states.top()->exit();
    // Elimina el último estado.
    _states.pop();
  }

  // Transición al nuevo estado.
  _states.push(state);
  // enter() sobre el nuevo estado.
  _states.top()->enter();
}

void
GameManager::pushState
(GameState* state)
{
  // Pausa del estado actual.
  if (!_states.empty())
    _states.top()->pause();
  
  // Transición al nuevo estado.
  _states.push(state);
  // enter() sobre el nuevo estado.
  _states.top()->enter();
}

void
GameManager::popState ()
{
  // Limpieza del estado actual.
  if (!_states.empty()) {
    _states.top()->exit();
    _states.pop();
  }
  
  // Vuelta al estado anterior.
  if (!_states.empty())
    _states.top()->resume();
}

void
GameManager::loadResources ()
{
  Ogre::ConfigFile cf;
  cf.load("resources.cfg");
  
  Ogre::ConfigFile::SectionIterator sI = cf.getSectionIterator();
  Ogre::String sectionstr, typestr, datastr;
  while (sI.hasMoreElements()) {
    sectionstr = sI.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap *settings = sI.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typestr = i->first;    datastr = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation
            (datastr, typestr, sectionstr);	
    }
  }
}

bool
GameManager::configure ()
{
  if (!_root->restoreConfig()) {
    if (!_root->showConfigDialog()) {
      return false;
    }
  }
  
  _renderWindow = _root->initialise(true, "Pac-Man");
  
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  
  return true;
}

GameManager*
GameManager::getSingletonPtr ()
{
  return msSingleton;
}

GameManager&
GameManager::getSingleton ()
{  
  assert(msSingleton);
  return *msSingleton;
}

// Las siguientes funciones miembro delegan
// el evento en el estado actual.
bool
GameManager::frameStarted
(const Ogre::FrameEvent& evt)
{
  _inputMgr->capture();
  return _states.top()->frameStarted(evt);
}

bool
GameManager::frameEnded
(const Ogre::FrameEvent& evt)
{
  return _states.top()->frameEnded(evt);
}

bool
GameManager::keyPressed 
(const OIS::KeyEvent &e)
{
  _states.top()->keyPressed(e);
  return true;
}

bool
GameManager::keyReleased
(const OIS::KeyEvent &e)
{
  _states.top()->keyReleased(e);
  return true;
}

bool GameManager::initSDL () {
    // Inicializando SDL...
  std::cout<<"Inicianzo SDL"<<std::endl;
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;
    // Llamar a  SDL_Quit al terminar.
    atexit(SDL_Quit);

    // Inicializando SDL mixer...
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 4096) < 0)
      return false;

    // Llamar a Mix_CloseAudio al terminar.
    atexit(Mix_CloseAudio);

    return true;
}
