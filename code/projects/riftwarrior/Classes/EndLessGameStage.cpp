//
//  EndLessGameStage.cpp
//  tdgame
//
//  Created by Wang Nero on 13-7-12.
//
//

#include "EndLessGameStage.h"
#include "GameScene.h"

bool operator < (const EnemiesInfo& left, const EnemiesInfo& right){
    if(left.maxHp < right.maxHp){
        return true;
    }else{
        return false;
    }
}


bool EndLessGameStage::init(int mapId)
{
    bool result = GameStage::init(mapId);
    this->resetStage(1);
    this->m_iWaveCount = 0;
    return result;
}

EndLessGameStage* EndLessGameStage::create(int mapId)
{
    EndLessGameStage* pStage = new EndLessGameStage();
    if (pStage && pStage->init(mapId))
    {
        pStage->autorelease();
        return pStage;
    }
    CC_SAFE_DELETE(pStage);
    return NULL;
}

void EndLessGameStage::reset()
{
    GameStage::reset();
    this->resetStage(1);
    this->m_iWaveCount = 0;
}

void EndLessGameStage::loadEnemies()
{
    int level = this->getEnemiesLeve();
    this->loadEnemiess(this->getEnemiesMaxCount(),level);    
    this->loadBoss(level);
    GameScene::getInstance()->sharedMainUI->getStageUI()->updateStageWave(this->m_iWaveCount);
}

void EndLessGameStage::removeEnemy(Enemy* pEnemy)
{
    this->enemies->removeObject(pEnemy);
    if(this->enemies->count() < 1){
        this->loadEnemies();
    }
}

//new

int EndLessGameStage::getEnemiesLeve()
{
    static int result = 1;
    if(m_iWaveCount % 5 == 0){
        result += 1;
    }
    if (m_iWaveCount > 30) {
        result += 2;
    }
    return result;
}

int EndLessGameStage::getEnemiesMaxCount()
{
    static int baseNum = 10;
    int result = 0;
    if(m_iWaveCount == 0){
        baseNum = 10;
    }
    ++m_iWaveCount;    
    if(this->m_iWaveCount % 2 == 0){
        this->waveDataUpdate();        
    }
    if(this->m_iWaveCount % 3 == 0){
        baseNum += rand() % baseNum;
        baseNum = baseNum < 100 ? baseNum : 100;
    }
    result = baseNum * m_iWaveCount + rand()% baseNum;
    result = result > 200 ? 200 : result;
    return result;
}

void EndLessGameStage::waveDataUpdate()
{
    if (m_iWaveCount == 0)
	{
        return;
    }

	int enemiesZoneIndex = 0;
	for (EnemiesZone& enemiesZone : this->m_enemiesZones)
	{
		++enemiesZoneIndex;
        if (enemiesZone.probability <= 0.2f)
		{
            continue;
        }
		else
		{
            float probability =  enemiesZone.probability * 0.7f;
			this->updateZonesProbability(enemiesZoneIndex, enemiesZone.probability - probability);
            enemiesZone.probability = probability;
            break;
        }
    }
}

void EndLessGameStage::resetStage(int value)
{
    this->loadEnemiesInfo();
    this->initEnemiesZones();
    this->initDifficultyValue(value);
}

void EndLessGameStage::loadBoss(int level)
{
    if((m_iWaveCount >= 10 && m_iWaveCount % 5 == 0) ||
       m_iWaveCount > 30){
        int count = 1;
        if(m_iWaveCount > 30){
            count += m_iWaveCount % 15;
        }
        for (; count > 0; --count) {
            int size = rand() % this->m_bossIDs.size();
            EnemiesInfoList::const_iterator v = this->m_bossIDs.begin();
            for (int count = 0; count < size; ++count,++v) {
            }
            Enemy * pEnemy = Enemy::create(v->enemyid,
                                           pGroundMap->getStartTilePositions()[0]);
            int bossLeve = level / 3;
            bossLeve = bossLeve == 0 ? 1 : bossLeve;
            pEnemy->init(bossLeve);
            enemies->addObject(pEnemy);
            pGroundMap->addChild(pEnemy);
            pEnemy->activate();
        }
    }
}

void EndLessGameStage::loadEnemiess(int maxCount, int level)
{
    for (size_t count = 0; count < this->m_enemiesZones.size(); ++count) 
	{
        this->loadZoneEnemiess(count, maxCount, level);
    }
}

void EndLessGameStage::loadZoneEnemiess(size_t zone, int maxCount, int level)
{
    if (zone < this->m_enemiesZones.size() && maxCount > 0)
	{
        int zoneCount = maxCount * this->m_enemiesZones[zone].probability;
        int zoneEnemiesCount = 0;
        vector<int> zonem_enemiesIDs;
        EnemiesInfoList::const_iterator v = this->m_enemiesIDs.begin();         
        for (int count = this->m_enemiesZones[zone].limite.first; count > 0; --count,++v) {
        }
        for (int head = this->m_enemiesZones[zone].limite.first;
             head <= this->m_enemiesZones[zone].limite.second; ++head,++v) {
            zonem_enemiesIDs.push_back(v->enemyid);
        }
        
        float delaytime = 0.0f;
        float interval = 1.0f;
        
        if(m_iWaveCount > 5){
            interval -= 0.2f;
        }
        
        while (zoneEnemiesCount++ < zoneCount) {
            Enemy * pEnemy = Enemy::create(zonem_enemiesIDs[rand()%zonem_enemiesIDs.size()],
                                          pGroundMap->getStartTilePositions()[0]);
            pEnemy->init(level);
            enemies->addObject(pEnemy);            
            pGroundMap->addChild(pEnemy);
            delaytime = zoneEnemiesCount * interval;
            
            pEnemy->scheduleOnce(schedule_selector(Enemy::activate), delaytime);
        }
    }
}

void EndLessGameStage::loadEnemiesInfo()
{
    this->m_bossIDs.clear();
    this->m_enemiesIDs.clear();
    const map<int, EnemySetting*> enemies = GameData::getAllEnemies();
    for (map<int, EnemySetting*>::const_iterator get = enemies.begin(); get != enemies.end(); ++get) {
        if(get->second->type != ENUM_ENEMY_TYPE_BOSS && get->second->type != ENUM_ENEMY_TYPE_CHILD){
            this->m_enemiesIDs.push_back(*new EnemiesInfo(get->second->hp,get->second->id));
        }else if(get->second->type == ENUM_ENEMY_TYPE_BOSS){
            this->m_bossIDs.push_back(*new EnemiesInfo(get->second->hp,get->second->id));
        }
    }
    this->m_bossIDs.sort();    
    this->m_enemiesIDs.sort();
}

void EndLessGameStage::initEnemiesZones()
{
    this->m_enemiesZones.clear();
    int startIndex = 0;
    int size = this->m_enemiesIDs.size();
    while (size > 0) {
        int used = 3;
        if(size -= used,size < 0){
            used += size;
        }
        used += startIndex;
        this->m_enemiesZones.push_back(*new EnemiesZone(startIndex,used - 1,0.0f));
        startIndex = used;
    }
}

void EndLessGameStage::initDifficultyValue(int dValue)
{
    if(this->m_enemiesZones.size() > 0 && dValue > 0){
        float frist = 1 / (float)dValue;
        this->m_enemiesZones[0].probability = frist;
        this->updateZonesProbability(1, 1 - frist);
    }
}

void EndLessGameStage::updateZonesProbability(int start, float p)
{
    float usedP = 0.0f;
    float size = this->m_enemiesZones.size() - start;
    if (start > 0 && size > 1 && p > 0)
	{
        for (size_t count = start; count < this->m_enemiesZones.size(); ++count) 
		{
            if (usedP >= p)
			{
                break;
            }
			else
			{
                float percent = 1.0f - (float)(count - start + 1) / size;
                percent = percent == 0 ? 1.0f : percent;
                this->m_enemiesZones[count].probability += percent * (p - usedP);
                usedP += percent * (p - usedP);
            }
        }
        if ((p - usedP) > 0)
		{
            this->m_enemiesZones[start].probability += p - usedP;
        }
    }
	else
	{
        this->m_enemiesZones[start].probability += p;
    }
}
