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

#ifndef MenuState_H
#define MenuState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "RecordManager.h"

class MenuState : public Ogre::Singleton<MenuState>, public GameState
{
 public:
  MenuState() {}

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);
  void showRecords ();
  static MenuState& getSingleton ();
  static MenuState* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  Ogre::OverlayManager* _overlayManager;
  Ogre::Overlay *_ovCreditos;



 private:

  int _sel;

  Ogre::SceneNode* _nodo;
  Ogre::SceneNode** _opciones;
  Ogre::Overlay* _ovRecords;

  void createScene();
  void createOverlay();
  void menuActions();
  bool _exitGame, _op;
};

#endif
