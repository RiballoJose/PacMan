#include "RecordManager.h"

template<> RecordManager* Ogre::Singleton<RecordManager>::msSingleton = 0;

RecordManager::RecordManager(){
	_docName = "Records.txt";
}//Fin contructor

RecordManager::~RecordManager(){
	_records.clear();
}//Fin destructor

void RecordManager::saveRecords(){
	std::ofstream file(_docName.c_str());
	std::map<int, Record>::iterator last_iter = _records.end();

	if(_records.size() > 0){
		last_iter--;
	}//Fin if
	
    if (file.is_open()){
        for (std::map<int, Record>::iterator it = _records.begin(); 
            it != _records.end(); ++it){
            file << it->first 
        		 << ":" << it->second.getPlayer()
                 << ":" << it->second.getPunt();
            if(it != last_iter){
            	file << '\n';
            }//Fin if
        }//Fin for
        file.close();
    } else {
        std::cout << "Error al escribir archivo" << std::endl;
    }//Fin if-else
}//Fin saveRecords

void RecordManager::loadRecords(){
	Record *rec;
	std::string str;
	Ogre::StringVector vct;
    std::ifstream file(_docName.c_str());

    if (file.is_open()){
        while (getline(file, str)){
        	/* Hacemos split por ":" para obtener la posicion, el nombre del jugador y la puntuacion */
            vct = Ogre::StringUtil::split(str.c_str(), ":");

            rec = new Record();
            /* Agregamos la posicion al record */
        	rec->setPos(Ogre::StringConverter::parseInt(vct[0]));
        	/* Agregamos el nombre del jugador al record */
        	rec->setPlayer(vct[1]);
        	/* Agregamos la puntuacion al record */
        	rec->setPunt(Ogre::StringConverter::parseInt(vct[2]));

        	/* Agregamos el record al map */
        	addRecord(*rec);
        }//Fin while
        file.close();
    }//Fin if
}//Fin loadRecords

void RecordManager::addRecord(Record rec){
	/* Variable auxiliar */
	Record aux;

	/* Comprobamos si el record tiene ya posicion */
	if(rec.getPos() != 0) {
		_records[rec.getPos()] = rec;
	/* Comprobamos si el mapa esta vacio */
	} else if(_records.empty()){
		/* Si lo esta, metemos el record como primero */
		rec.setPos(1);
		_records[1] = rec;
	} else {
		/* Tomamos la medicion del size aqui porque cabe la posibilidad
		 * de que crezca en 1, y por tanto puede que varie el valor de la
		 * consulta de una iteracion del for a otra. Solo nos importa como
		 * esta al principio, por tanto la tomamos ahora. */
		int size = _records.size();
		/* Empezamos desde la ultima posicion */
		for(int i = size; i > 0; i--){
			/* Si el record para esa clave es menor que el que me pasan */
			if(_records[i].getPunt() < rec.getPunt()){
				/* Meto el nuevo record en esa clave */
				aux = _records[i];
				rec.setPos(i);
				_records[i] = rec;

				/* Si la posicion por la que lo cambio no es la maxima permitida */
				if(i < MAX_RECORDS){
					aux.setPos(i + 1);
					_records[(i + 1)] = aux;
				}//Fin if
			} else {
				/* Comprobamos que, aunque no supere al record[i], podamos meter
				 * mas records siempre y cuando no lo hayamos metido ya */
				if((i + 1) > size && (i + 1) <= MAX_RECORDS){
					rec.setPos(i + 1);
					_records[(i + 1)] = rec;
				}//Fin if
				/* Usamos un break para dejar de comprobar con records mejores */
				break;
			}//Fin if-else
		}//Fin for
	}//Fin if-else
}//Fin addRecord

Ogre::String RecordManager::toString(){
	std::stringstream aux;
	for (std::map<int, Record>::iterator it = _records.begin(); 
            it != _records.end(); ++it){
		aux << it->second.toString() << "\n";
	}//Fin for

	return aux.str();
}//Fin toString

Record RecordManager::getRecord(int pos){
	/* Devolvemos el record asociado a la clave */
	return _records[pos];
}//Fin getRecord

RecordManager* RecordManager::getSingletonPtr(){
    return msSingleton;
}//Fin getSingletonPtr

RecordManager& RecordManager::getSingleton(){  
    assert(msSingleton);
    return *msSingleton;
}//Fin getSingleton