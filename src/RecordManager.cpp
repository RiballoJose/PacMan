#include "RecordManager.h"

template<> RecordManager* Ogre::Singleton<RecordManager>::msSingleton = 0;

RecordManager::RecordManager()
{
  _docName = "Records.txt";
}

RecordManager::~RecordManager()
{
  _records.clear();
}

void
RecordManager::saveRecords()
{
  std::ofstream file(_docName.c_str());
  std::map<int, Record>::iterator last_iter = _records.end();

  if(_records.size() > 0){
    last_iter--;
  }
	
  if (file.is_open()){
    for (std::map<int, Record>::iterator it = _records.begin(); 
	 it != _records.end(); ++it){
      file << it->first 
	   << ":" << it->second.getPlayer()
	   << ":" << it->second.getPunt();
      if(it != last_iter){
	file << '\n';
      }
    }
    file.close();
  } else {
    std::cout << "Error al escribir archivo" << std::endl;
  }
}

void
RecordManager::loadRecords()
{
  Record *rec;
  std::string str;
  Ogre::StringVector vct;
  std::ifstream file(_docName.c_str());

  if (file.is_open()){
    while (getline(file, str)){
      vct = Ogre::StringUtil::split(str.c_str(), ":");

      rec = new Record();
      rec->setPos(Ogre::StringConverter::parseInt(vct[0]));
      rec->setPlayer(vct[1]);
      rec->setPunt(Ogre::StringConverter::parseInt(vct[2]));
		
      addRecord(*rec);
    }
    file.close();
  }
}

void
RecordManager::addRecord(Record rec)
{
  Record aux;
  if(rec.getPos() != 0) {
    _records[rec.getPos()] = rec;
  } else if(_records.empty()){
    rec.setPos(1);
    _records[1] = rec;
  } else {
    int size = _records.size();
    for(int i = size; i > 0; i--){
      if(_records[i].getPunt() < rec.getPunt()){
	aux = _records[i];
	rec.setPos(i);
	_records[i] = rec;
				
	if(i < 6){
	  aux.setPos(i + 1);
	  _records[(i + 1)] = aux;
	}
      }
      else {
	if((i + 1) > size && (i + 1) <= 6){
	  rec.setPos(i + 1);
	  _records[(i + 1)] = rec;
	}
	break;
      }
    }
  }
}

Ogre::String
RecordManager::toString()
{
  //loadRecords();
  std::stringstream aux;
  for (std::map<int, Record>::iterator it = _records.begin(); 
       it != _records.end(); ++it){
    aux << it->second.toString() << "\n";
  }

  return aux.str();
}

Record
RecordManager::getRecord(int pos)
{
  return _records[pos];
}
RecordManager*
RecordManager::getSingletonPtr()
{
  return msSingleton;
}
RecordManager&
RecordManager::getSingleton()
{  
  assert(msSingleton);
  return *msSingleton;
}
