//
//  CompletedStage.cpp
//  tdgame
//
//  Created by Like Zhang on 5/7/13.
//
//

#include "CompletedStage.h"

CompletedStage* CompletedStage::create(int mapId)
{
    CompletedStage* pStage = new CompletedStage();
    if (pStage && pStage->init(mapId))
    {
        pStage->autorelease();
        return pStage;
    }
    
    CC_SAFE_DELETE(pStage);
    return NULL;
}

bool CompletedStage::init(int mapId)
{
    if (!GameStage::init(mapId))
    {
        return false;
    }
    
    m_Boss = NULL;

    return true;
}


void CompletedStage::clear()
{
    GameStage::clear();
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    char plistPath[128]= {0};
    char pvrPath[128] = {0};
    
    for(int i=1;i<10;++i)
    {
        const EnemySetting* pSetting = GameData::getEnemySetting(i);
        if (!pSetting ||
            pSetting->type == ENUM_ENEMY_TYPE_CHILD)
        {
            continue;
        }
        
        sprintf(plistPath, "enemy/%d/animation.plist",i);
        sprintf(pvrPath, "enemy/%d/animation.pvr.ccz",i);
        frameCache->removeSpriteFramesFromFile(plistPath);
        
    }
    
    m_Boss = NULL;
    
}

void CompletedStage::reset()
{
    GameStage::reset();
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    char plistPath[128]= {0};
    char pvrPath[128] = {0};
    
    m_MaxEnemyId = 0;
    for(int i=1;i<50;++i)
    {
        const EnemySetting* pSetting = GameData::getEnemySetting(i);
        if (!pSetting ||
            pSetting->type == ENUM_ENEMY_TYPE_CHILD)
        {
            continue;
        }
        
        if (i>m_MaxEnemyId)
        {
            m_MaxEnemyId = i;
        }
        
        sprintf(plistPath, "enemy/%d/animation.plist",i);
        sprintf(pvrPath, "enemy/%d/animation.pvr.ccz",i);
        frameCache->addSpriteFramesWithFile(plistPath, pvrPath);
    }
    
    loadEnemies();
    
}


void CompletedStage::loadEnemies()
{
    m_CurrentStageEnemies = getCurrentStageEnemies();
    
    int lastTileIndex = pGroundMap->getSequenceByTile(pGroundMap->getEndTilePosition());
    
    int count = 0;
    while (enemies->count()< (unsigned int)m_pCurrentMapConfig->maxLevel * 2)
    {
        int id = m_CurrentStageEnemies[rand()%m_CurrentStageEnemies.size()];
        
        const EnemySetting* pSetting = GameData::getEnemySetting(id);
        if (!pSetting ||
            pSetting->type == ENUM_ENEMY_TYPE_BOSS)
        {
            continue;
        }
        
        Enemy * pEnemy = Enemy::create(id, pGroundMap->getTileBySequence(1 + rand()%(lastTileIndex/2 - 1)));
        int level = ceil(m_pCurrentMapConfig->maxLevel/2) + rand()%max((m_pCurrentMapConfig->maxLevel-1)/2, 1);
        pEnemy->init(level);
        pGroundMap->addChild(pEnemy);
        enemies->addObject(pEnemy);
        pEnemy->activate();
        
        ++count;
    }
    
    //create boss
    if (!m_Boss)
    {
        int id = m_CurrentStageEnemies[rand()%m_CurrentStageEnemies.size()];
        while (id == 1 && m_CurrentStageEnemies.size()>3)
        {
            // cannot be crab!
            id = m_CurrentStageEnemies[rand()%m_CurrentStageEnemies.size()];
        }
        
        m_Boss = Enemy::create(id, pGroundMap->getStartTilePositions()[0]);
        m_Boss->init(m_pCurrentMapConfig->maxLevel);
        m_Boss->setBoss(true);

        pGroundMap->addChild(m_Boss);
        enemies->addObject(m_Boss);
        
        m_Boss->retain();
        m_Boss->activate();
    }
    
    
}

void CompletedStage::removeEnemy(Enemy *pEnemy)
{
    enemies->removeObject(pEnemy);
    
    if (pEnemy == m_Boss)
    {
        m_Boss->release();
        m_Boss = NULL;
        completeStage();
        return;
    }
    
    if (enemies->count()<3 && m_Boss && !m_Boss->isDead())
    {
        // too less, load new enemies
        
        loadEnemies();
        
    }
}