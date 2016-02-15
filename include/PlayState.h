/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: David Vallejo Fernández    David.Vallejo@uclm.es
 *
 * Código modificado a partir de Managing Game States with OGRE
 * http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
 * Inspirado en Managing Game States in C++
 * http://gamedevgeek.com/tutorials/managing-game-states-in-c/
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <cstdlib>

#include "GameState.h"
#include "EndState.h"
#include "Graph.h"
#include "Ghost.h"
#include "TrackManager.h"
#include "SoundFXManager.h"


class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:
  PlayState () {}

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();

  void LoadLevels();
  void nextLevel();
  void removeLevel();
  void createScene();
  void pacmanMove();
  void ghostMove(Ghost* ghost);
  bool colisionMap(int dir, Ogre::SceneNode* node);
  void died();
  bool hit(Ghost* ghost);
  void resetGhost(Ghost* ghost);
  void destroyAllAttachedMovableObjects(Ogre::SceneNode* node);
  void removeScene();
  void createOverlay();
  void calculateAdjs();
  void canEat(bool b);
  void eating();
  void flicker();
  void blinking();
  void animar();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Ogre::Light* _light;

  int** _levels;
  int _hits, _filas, _columnas, _currentDir, _nextDir, _prevDir,
    _currentLevel, _perspective, _nwalls, _nPacDots, _score;
  double _pacSpeed, _startRow, _currentRow, _startCol,
    _currentCol, _prevRow, _prevCol, _canDied;
  bool _exitGame,  _endLevel, _pacmanDef, _canEat;

  Ogre::Real _deltaT;
  Ogre::Vector3 _pacMove;
  
  std::vector<Ogre::SceneNode*> _lifes;
  Ogre::SceneNode* _pacman;
  Ogre::Vector3 _startPos;
  Ghost* _blinky;//fantasma rojo

  Ghost* _pinky;//rosa
  Ghost* _inky;//azul cian
  Ghost* _clyde;//naranja
  std::vector<int>* _wallRows;
  std::vector<int>* _wallCols;
  std::vector<std::string>* _anim;
  
  Ogre::OverlayManager* _overlayManager;
  Ogre::Overlay *_ovJuego;
  Ogre::OverlayElement *_ovPunt, *_ovVida, *_ovScore;

  TrackPtr _mainTrack;
  SoundFXPtr _simpleEffect;
  TrackManager* _pTrackManager;
  SoundFXManager* _pSoundFXManager;

  Ogre::AnimationState *_animState;

  Graph* _level;

  
};

#endif
