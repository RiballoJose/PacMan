#include "InputManager.h"

template<> InputManager* Ogre::Singleton<InputManager>::msSingleton = 0;

InputManager::InputManager ():
  _inputSystem(0),
  _keyboard(0)
{
}

InputManager::~InputManager ()
{
  if (_inputSystem) {

    if (_keyboard) {
      _inputSystem->destroyInputObject(_keyboard);
      _keyboard = 0;
    }

    OIS::InputManager::destroyInputSystem(_inputSystem);

    _inputSystem = 0;

    // Limpiar todos los listeners.
    _keyListeners.clear();
    }
}

void
InputManager::initialise
(Ogre::RenderWindow *renderWindow)
{
  if(!_inputSystem) {
    // Setup basic variables
    OIS::ParamList paramList;    
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    renderWindow->getCustomAttribute("WINDOW", &windowHnd);
    // Fill parameter list
    windowHndStr << windowHnd;
    paramList.insert(std::make_pair(std::string( "WINDOW"),
				    windowHndStr.str()));

    // Create inputsystem
    _inputSystem = OIS::InputManager::
      createInputSystem(paramList);

    _keyboard = static_cast<OIS::Keyboard*>
      (_inputSystem->createInputObject(OIS::OISKeyboard, true));
    _keyboard->setEventCallback(this);

    // Get window size
    unsigned int width, height, depth;
    int left, top;
    renderWindow->getMetrics(width, height, depth, left, top);
  }
}

void
InputManager::capture ()
{
  
  if (_keyboard)
    _keyboard->capture();
}

void
InputManager::addKeyListener
(OIS::KeyListener *keyListener, const std::string& instanceName)
{
  if (_keyboard) {
    // Comprobar si el listener existe.
    itKeyListener = _keyListeners.find(instanceName);
    if (itKeyListener == _keyListeners.end()) {
      _keyListeners[instanceName] = keyListener;
    }
    else {
      // Elemento duplicado; no hacer nada.
    }
  }
}

void
InputManager::removeKeyListener
(const std::string& instanceName)
{
  // Comprobar si el listener existe.
  itKeyListener = _keyListeners.find(instanceName);
  if (itKeyListener != _keyListeners.end()) {
    _keyListeners.erase(itKeyListener);
  }
  else {
    // No hacer nada.
  }
}
void
InputManager::removeKeyListener
(OIS::KeyListener *keyListener)
{
  itKeyListener = _keyListeners.begin();
  itKeyListenerEnd = _keyListeners.end();
  for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) {
    if (itKeyListener->second == keyListener) {
      _keyListeners.erase(itKeyListener);
      break;
    }
  }
}
void
InputManager::removeAllListeners
()
{
  _keyListeners.clear();
}

void
InputManager::removeAllKeyListeners ()
{
  _keyListeners.clear();
}

void
InputManager::setWindowExtents 
(int width, int height)
{
}

OIS::Keyboard*
InputManager::getKeyboard ()
{
    return _keyboard;
}

bool
InputManager::keyPressed 
(const OIS::KeyEvent &e)
{
  itKeyListener = _keyListeners.begin();
  itKeyListenerEnd = _keyListeners.end();
  // Delega en los KeyListener añadidos.
  for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) {
    itKeyListener->second->keyPressed(e);
  }

  return true;
}

bool
InputManager::keyReleased
(const OIS::KeyEvent &e)
{
  itKeyListener = _keyListeners.begin();
  itKeyListenerEnd = _keyListeners.end();
  // Delega en los KeyListener añadidos.
  for (; itKeyListener != itKeyListenerEnd; ++itKeyListener) {
    itKeyListener->second->keyReleased( e );
  }

  return true;
}

InputManager*
InputManager::getSingletonPtr ()
{
  return msSingleton;
}

InputManager&
InputManager::getSingleton ()
{  
  assert(msSingleton);
  return *msSingleton;
}
