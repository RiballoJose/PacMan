#ifndef RecordManager_h
#define RecordManager_h
#define MAX_RECORDS	9

#include <map>
#include <string>

#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreSingleton.h>

#include "Record.h"

class RecordManager: public Ogre::Singleton<RecordManager>{
 public:
  RecordManager();
  ~RecordManager();

  void saveRecords();
  void loadRecords();
	
  void addRecord(Record rec);
  Record getRecord(int pos);

  Ogre::String toString();
	
  static RecordManager& getSingleton ();
  static RecordManager* getSingletonPtr ();
 private:
  std::string _docName;
  std::map <int, Record> _records;
};

#endif
