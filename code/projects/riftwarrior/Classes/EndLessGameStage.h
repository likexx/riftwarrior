//
//  EndLessGameStage.h
//  tdgame
//
//  Created by Wang Nero on 13-7-12.
//
//

#ifndef __tdgame__EndLessGameStage__
#define __tdgame__EndLessGameStage__

#include "GameStage.h"
#include <vector>

struct EnemiesZone{
    float probability;   
    std::pair<int, int> limite;
    
    EnemiesZone(int head,int tail,float p){
        this->probability = p;
        limite = make_pair(head, tail);
    }
};

struct EnemiesInfo{
    int maxHp;
    int enemyid;
    
    EnemiesInfo(int max,int id) : maxHp(max),enemyid(id){
    }
};

typedef vector<EnemiesZone> EnemiesZones;
typedef list<EnemiesInfo> EnemiesInfoList;

class EndLessGameStage : public GameStage
{
public:
    virtual bool init(int mapId);
    virtual void reset();
    virtual void loadEnemies();
    virtual void removeEnemy(Enemy* pEnemy);
    static EndLessGameStage* create(int mapId);    
private:
    void resetStage(int value);
    void loadBoss(int level);
    void loadEnemiess(int maxCount,int level);
    void loadZoneEnemiess(size_t zone, int maxCount, int level);
    void updateZonesProbability(int start,float p);
    void waveDataUpdate();
    int getEnemiesLeve();
    int getEnemiesMaxCount();
    void loadEnemiesInfo();
    void initEnemiesZones();
    void initDifficultyValue(int value);
    int m_iWaveCount;    
    EnemiesZones m_enemiesZones;
    EnemiesInfoList m_bossIDs;
    EnemiesInfoList m_enemiesIDs;
};

#endif /* defined(__tdgame__EndLessGameStage__) */
