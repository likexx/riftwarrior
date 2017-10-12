//
//  GameData.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/5/12.
//
//

#include "GameData.h"

#include "SystemHelper.h"

using namespace cocos2d;

#define DB_FILE_NAME    "db/game.db"

static map<int, MapConfig> _mapConfigMap;
static map<int, MapWinningCondition> _mapWinningConditions;

static map<int, BuildingSetting> _buildingSettings;
static map<int, Item> _itemData;

static map<int, EnemySetting*> _enemySettings;
static map<int, Magic*> _allMagic;
static map<int, Skill*> _allSkills;
static CCDictionary* _language = nullptr;

static map<int, ActiveSkillSetting> _activeSkillSettings;
static map<int, NpcSetting> _npcSettings;

static DbManager* _db;

DbManager::DbManager(const char* db)
{
    string dbPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(db);
    
    int result = sqlite3_open(dbPath.c_str(), &dbSource);
    if (result != SQLITE_OK)
    {
        CCLog("ERROR opening db");
        dbSource = NULL;
    }
}

DbManager::~DbManager()
{
    if (dbSource)
    {
        sqlite3_close(dbSource);
    }
}

sqlite3* DbManager::getDb()
{
    return dbSource;
}


void GameData::initLanguageText()
{
    if (_language)
    {
        _language->release();
        _language = NULL;
    }
    
    ENUM_LANGUAGE language = SystemHelper::getCurrentLanguage();
    switch (language)
    {
        case ENUM_LANGUAGE_CHINESE:
            _language = CCDictionary::createWithContentsOfFile("language/chinese.plist");
            break;
        case ENUM_LANGUAGE_ENGLISH:
            _language = CCDictionary::createWithContentsOfFile("language/english.plist");
            break;
    }
    
    _language->retain();
}

static int processBuildingData(void *NotUsed, int number, char **pRow, char **azColName)
{
    BuildingSetting data;
    data.init(pRow);
    _buildingSettings[data.id] = data;
    return 0;
}

static int processEnemyData(void *NotUsed, int number, char **pRow, char **azColName)
{
    EnemySetting* pData = new EnemySetting();
    pData->init(pRow);
    _enemySettings[pData->id] = pData;
    return 0;
}

static int processSkillData(void *NotUsed, int number, char **pRow, char **azColName)
{
    Skill* pData = new Skill();
    pData->init(pRow);
    _allSkills[pData->id] = pData;
    return 0;
}


static int processMagicData(void *NotUsed, int number, char **pRow, char **azColName)
{
    Magic* pData = new Magic();
    pData->init(pRow);
    _allMagic[pData->id] = pData;
    return 0;
}

static int processMapConfigData(void *NotUsed, int number, char **pRow, char **azColName)
{
    MapConfig map;
    map.init(pRow);
    _mapConfigMap[map.mapId] = map;
    return 0;
}

static int processActiveSkillData(void *NotUsed, int number, char **pRow, char **azColName)
{
    ActiveSkillSetting setting;
    setting.init(pRow);
    _activeSkillSettings[setting.id] = setting;
    return 0;
}

static int processNpcSetting(void *NotUsed, int number, char **pRow, char **azColName)
{
    NpcSetting setting;
    setting.init(pRow);
    _npcSettings[setting.id] = setting;
    return 0;
}


static int processMapWinningCondition(void *NotUsed, int number, char **pRow, char **azColName)
{
    MapWinningCondition condition;
    condition.init(pRow);
    _mapWinningConditions[condition.mapId] = condition;
    return 0;
}


void GameData::init()
{
    /*
    {
        DbManager dbManager("data.db");
        sqlite3* db = dbManager.getDb();
        char *zErrMsg = 0;
        sqlite3_exec(db, "SELECT * from building", testCallback, 0, &zErrMsg);
    }
    */
    _db = new DbManager(DB_FILE_NAME);

    initBuildingSetting();
    initMapConfig();
    initMapWinningConditions();
    
    initEnemySetting();
    initMagic();
    initSkills();
    initActiveSkills();
    initNpcSettings();
}

const char* GameData::getText(const char *key)
{
    const CCString* pValue = _language->valueForKey(key);
    return pValue->getCString();
}


const map<int, BuildingSetting>& GameData::getBuildingSettings()
{
    return _buildingSettings;
}

const BuildingSetting& GameData::getBuildingSetting(int id)
{
    const BuildingSetting& setting = _buildingSettings[id];
    return setting;
}

const BuildingSetting* GameData::getNextBuildingSetting(int id)
{
    map<int, BuildingSetting>::iterator it = _buildingSettings.begin();
    
    while(it != _buildingSettings.end())
    {
        const BuildingSetting& setting = it->second;
        if (setting.require == id)
        {
            return &setting;
        }
        ++it;
    }

    return nullptr;
}


void GameData::initNpcMessageForMap(int mapId, queue<MapNpcMessage> *pResult)
{
    sqlite3* db = _db->getDb();
    
    char* errorMessage;
    int nrow=0, ncolumn=0;
    char **row = nullptr;
    char sql[1024] = {0};
    sprintf(sql, "SELECT * FROM map_npc_message WHERE map_id=%d ORDER BY wave,sequence", mapId);

    sqlite3_get_table(db,sql,&row,&nrow,&ncolumn,&errorMessage);
    
    for(int i=1; i< nrow + 1; ++i)
    {
        int j = i * ncolumn;
        
        MapNpcMessage data;
        data.mapId = atoi(row[j]);
        data.sequence = atoi(row[j+1]);
        data.wave = atoi(row[j+2]);
        
        char path[128] = {0};
        if (strcmp(row[j+3], "player_talk") == 0 ||
            strcmp(row[j+3], "player") == 0)
        {
            sprintf(path, "npc/%s.png", row[j+3]);
        }
        else
        {
            sprintf(path, "npc/%s_talk.png", row[j+3]);
        }
        data.npcImage = path;
        data.npcName = row[j+3];
        
        data.message = row[j + 4 + SystemHelper::getCurrentLanguage()];
        data.tileX = 0;
        data.tileY = 0;
        
        if (row[j+6] && strlen(row[j+6])>1)
        {
            string positionPairString(row[j + 6]);
            size_t splitter = positionPairString.find(',');
            data.tileX = ::atoi(positionPairString.substr(0, splitter).c_str());
            data.tileY = ::atoi(positionPairString.substr(splitter + 1).c_str());
        }
        
        pResult->push(data);
    }
    
    sqlite3_free_table(row);
}

const MapConfig* GameData::getMapConfig(int id)
{
    map<int, MapConfig>::iterator it = _mapConfigMap.find(id);
    
    if (it == _mapConfigMap.end())
    {
        return NULL;
    }
    
    return &it->second;
}

const map<int, MapConfig>& GameData::getAllMapConfigs()
{
    return _mapConfigMap;
}

void GameData::initMapConfig()
{
    sqlite3* db = _db->getDb();
    
    char* errorMessage = 0;
    
    sqlite3_exec(db, "SELECT * from map_config", processMapConfigData, 0, &errorMessage);
}


void GameData::initMapWinningConditions()
{
    sqlite3* db = _db->getDb();
    
    char* errorMessage = 0;
    
    sqlite3_exec(db, "SELECT * from map_winning_condition", processMapWinningCondition, 0, &errorMessage);
}

const MapWinningCondition* GameData::getMapWinningCondition(int id)
{
    map<int, MapWinningCondition>::iterator it = _mapWinningConditions.find(id);
    
    // return default (id 1) if not found
    return it != _mapWinningConditions.end() ? &it->second : &_mapWinningConditions.find(1)->second;
    
}

void GameData::initBuildingSetting()
{
    sqlite3* db = _db->getDb();
    
    char* errorMessage = 0;
    
    sqlite3_exec(db, "SELECT * from building", processBuildingData, 0, &errorMessage);
    
    /*
    int nrow=0, ncolumn=0;
    char **row;
    const char* sql="SELECT * FROM building";
    sqlite3_get_table(db,sql,&row,&nrow,&ncolumn,&errorMessage);
    
    for(int i=1; i< nrow + 1; ++i)
    {
        int j = i * ncolumn;
        
        BuildingSetting building;
        building.init(row, j);
        
        _buildingSettings[building.id] = building;
    }
    */

}

void GameData::initEnemySetting()
{
    sqlite3* db = _db->getDb();
    
    char* errorMessage = 0;
    
    sqlite3_exec(db, "SELECT * from enemy", processEnemyData, 0, &errorMessage);
}

const EnemySetting* GameData::getEnemySetting(int id)
{
    map<int, EnemySetting*>::iterator it = _enemySettings.find(id);
    if (it != _enemySettings.end())
    {
        return it->second;
    }
    
    return NULL;
}

const map<int, EnemySetting*>& GameData::getAllEnemies()
{
    return _enemySettings;
}

void GameData::initSkills()
{
    sqlite3* db = _db->getDb();
    
    char* errorMessage = 0;
    
    sqlite3_exec(db, "SELECT * from skill", processSkillData, 0, &errorMessage);
}


const map<int, Skill*>& GameData::getAllSkills()
{
    return _allSkills;
}

const Skill* GameData::getSkill(int skillId)
{
    map<int, Skill*>::iterator it = _allSkills.find(skillId);
    if (it != _allSkills.end())
    {
        return it->second;
    }
    
    return NULL;
}

void GameData::initNpcSettings()
{
    sqlite3* db = _db->getDb();
    
    char* errorMessage = 0;
    
    sqlite3_exec(db, "SELECT * from npc", processNpcSetting, 0, &errorMessage);
}

const NpcSetting* GameData::getNpcSetting(int id)
{
    map<int, NpcSetting>::iterator it = _npcSettings.find(id);
    
    return it != _npcSettings.end() ? &it->second : NULL;
}

void GameData::initActiveSkills()
{
    sqlite3* db = _db->getDb();
    
    char* errorMessage = 0;
    
    sqlite3_exec(db, "SELECT * from active_skill", processActiveSkillData, 0, &errorMessage);
}

const map<int, ActiveSkillSetting>& GameData::getActiveSkills()
{
    return _activeSkillSettings;
}

const ActiveSkillSetting* GameData::getActiveSkillSetting(int id)
{
    map<int, ActiveSkillSetting>::iterator it = _activeSkillSettings.find(id);
    
    if (it != _activeSkillSettings.end())
    {
        return &it->second;
    }
    
    return NULL;
}

void GameData::initMagic()
{
    sqlite3* db = _db->getDb();
    
    char* errorMessage = 0;
    
    sqlite3_exec(db, "SELECT * from magic", processMagicData, 0, &errorMessage);
    
    /*
    int nrow=0, ncolumn=0;
    char **row;
    const char* sql="SELECT * FROM magic";
    sqlite3_get_table(db,sql,&row,&nrow,&ncolumn,&errorMessage);
    
    for(int i=1; i< nrow + 1; ++i)
    {
        int j = i * ncolumn;
        
        Magic* pMagic = new Magic();
        
        pMagic->init(row, j);
        
        _allMagic[pMagic->id] = pMagic;
    }
     */
}


const Magic* GameData::getMagic(int id)
{
    map<int, Magic*>::iterator it = _allMagic.find(id);
    
    if (it == _allMagic.end())
    {
        return NULL;
    }
    
    return it->second;
}

const Magic* GameData::getNextMagic(int id)
{
    map<int, Magic*>::iterator it = _allMagic.begin();
    
    while (it != _allMagic.end())
    {
        Magic* current = it->second;
        if (current->require == id)
        {
            break;
        }
        
        it++;
    }
    
    return it->second;
    
}

vector<Magic*> GameData::getAllMagic()
{
    vector<Magic*> result;
    map<int, Magic*>::iterator it = _allMagic.begin();
    
    while(it != _allMagic.end())
    {
        result.push_back(it->second);
        it++;
    }
    
    return result;
}

size_t GameData::getNumberOfMagic()
{
    return _allMagic.size();
}
