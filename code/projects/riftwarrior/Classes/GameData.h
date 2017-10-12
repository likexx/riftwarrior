//
//  GameData.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/5/12.
//
//

#ifndef __ike_tdgame__GameData__
#define __ike_tdgame__GameData__

#include <sqlite3.h>

#include "ActiveSkillSetting.h"
#include "BuildingSetting.h"
#include "EnemySetting.h"
#include "Item.h"
#include "Magic.h"
#include "MapConfig.h"
#include "MapNpcMessage.h"
#include "MapWinningCondition.h"
#include "NpcSetting.h"
#include "Skill.h"
#include <queue>

using namespace std;

class DbManager
{
public:
    DbManager(const char* filename);
    ~DbManager();
    sqlite3* getDb();
    
private:
    sqlite3* dbSource;
};

class GameData
{
public:
    static void initLanguageText();
    static void init();
    
    static const map<int, BuildingSetting>& getBuildingSettings();
    static const BuildingSetting& getBuildingSetting(int id);
    static const BuildingSetting* getNextBuildingSetting(int id);
    static const NpcSetting* getNpcSetting(int id);
    
    static const MapConfig* getMapConfig(int id);
    static const map<int, MapConfig>& getAllMapConfigs();
    
    static const MapWinningCondition* getMapWinningCondition(int id);
    
    static const EnemySetting* getEnemySetting(int id);
    static const map<int, EnemySetting*>& getAllEnemies();

    static const Magic* getMagic(int id);
    static const Magic* getNextMagic(int id);
    static size_t getNumberOfMagic();
    static vector<Magic*> getAllMagic();
    
    static const map<int, Skill*>& getAllSkills();
    static const Skill* getSkill(int skillId);
    
    static const ActiveSkillSetting* getActiveSkillSetting(int id);
    static const map<int, ActiveSkillSetting>& getActiveSkills();
    
    static void initBuildingSetting();
    
    static void initNpcMessageForMap(int mapId, queue<MapNpcMessage>* pResult);
    static void initMapConfig();
    static void initMapWinningConditions();
    
    static void initEnemySetting();
    static void initMagic();
    static void initSkills();
    
    static void initActiveSkills();
    static void initNpcSettings();
    
    static const char* getText(const char* key);
    
private:
//    static sqlite3* getDb();
};

#endif /* defined(__ike_tdgame__GameData__) */
