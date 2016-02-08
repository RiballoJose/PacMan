#include "EndState.h"
#include "MenuState.h"
#include "RecordManager.h"

template<> EndState* Ogre::Singleton<EndState>::msSingleton = 0;

char EndState::_name[3] = { 'A', 'A', 'A' };

void EndState::enter (){
    _root = Ogre::Root::getSingletonPtr();

    std::cout << "En endState " << '\n';
    /* Se recupera el gestor de escena y la cámara. */
    _sceneMgr = _root->getSceneManager("SceneManager");
    _camera = _sceneMgr->getCamera("CamaraPrincipal");
    _viewport = _root->getAutoCreatedWindow()->getViewport(0);
    _overlayManager = Ogre::OverlayManager::getSingletonPtr();

    _pos = 0;
    _endGame = false;

    _punteros = new Ogre::OverlayElement*[3];

    _rec = new Record();

    createOverlay();
}//Fin enter

void EndState::createOverlay(){
    std::stringstream st;

    _principal = _overlayManager->getByName("EnSt");
    _o_name = _overlayManager->getOverlayElement("NameText");
    _o_score = _overlayManager->getOverlayElement("ScoreText");

    for(int i = 0; i < 3; i++){
        st << "imgPunt" << (i + 1);
        _punteros[i] = _overlayManager->getOverlayElement(st.str());
        st.str("");
    }//Fin for

    _punteros[0]->show();
    _punteros[1]->hide();
    _punteros[2]->hide();
    _principal->show();
}//Fin createOverlay

void EndState::exit (){
    /* Ocultamos overlay */
    _principal->hide();
    _punteros[_pos]->hide();
    _rec->setPunt(Ogre::StringConverter::parseInt(_score));
    _rec->setPlayer(Ogre::String(_name));

    delete [] _punteros;

    _name[0] = 'A'; _name[1] = 'A'; _name[2] = 'A';
    RecordManager::getSingletonPtr()->addRecord(*_rec);
}//Fin exit

void EndState::pause (){}
void EndState::resume (){}

bool EndState::frameStarted(const Ogre::FrameEvent& evt){
    if(_principal->isVisible()){
        _o_name->setCaption(_name);
        _o_score->setCaption(_score);
    }//Fin if
    return true;
}//Fin frameStarted

bool EndState::frameEnded(const Ogre::FrameEvent& evt){  
	if(_endGame){
        popState();
    }//Fin if
  
    return true;
}//Fin frameEnded

void EndState::keyPressed(const OIS::KeyEvent &e){
    char aux;

    switch(e.key){
        case OIS::KC_RIGHT:
            _punteros[_pos]->hide();

            _pos++;
            if(_pos > 2){
                _pos = 0;
            }//Fin if

            _punteros[_pos]->show();
            break;
        case OIS::KC_LEFT:
            _punteros[_pos]->hide();

            _pos--;
            if(_pos < 0){
                _pos = 2;
            }//Fin if

            _punteros[_pos]->show();
            break;
        case OIS::KC_DOWN:
            aux = _name[_pos] - 1;
            if(aux < 'A'){
                _name[_pos] = 'Z';
            } else {
                _name[_pos] = aux;
            }//Fin if-else
            break;
        case OIS::KC_UP:
            aux = _name[_pos] + 1;
            if(aux > 'Z'){
                _name[_pos] = 'A';
            } else {
                _name[_pos] = aux;
            }//Fin if-else
            break;
        case OIS::KC_SPACE:
            /* Guardamos record */
            _endGame = true;
            break;
        default:
            break;
    }//Fin switch
}//Fin keyPressed

void EndState::addScore(int score){
    std::stringstream sc;

    if(score < 100){
        sc << "0";
        if(score < 10){
            sc << "0";
        }//Fin if
    }//Fin if

    sc << score;

    _score = sc.str();
}//Fin addScore

void EndState::keyReleased(const OIS::KeyEvent &e){}

EndState* EndState::getSingletonPtr (){
    return msSingleton;
}//Fin getSingletonPtr
EndState& EndState::getSingleton (){ 
    assert(msSingleton);
    return *msSingleton;
}//Fin getSingleton
