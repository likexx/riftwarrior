//
//  QuickBattleStage.cpp
//  tdgame
//
//  Created by Like Zhang on 5/6/13.
//
//

#include "QuickBattleStage.h"
#include "BuildingBase.h"
#include "GameScene.h"

QuickBattleStage* QuickBattleStage::create(int mapId)
{
    QuickBattleStage* pStage = new QuickBattleStage();
    if (pStage && pStage->init(mapId))
    {
        pStage->autorelease();
        return pStage;
    }
    
    CC_SAFE_DELETE(pStage);
    return NULL;
}

bool QuickBattleStage::init(int mapId)
{
    if (!GameStage::init(mapId))
    {
        return false;
    }
    
    m_pPlayerTower = NULL;
    m_pComputerTower = NULL;
    
    return true;
}

ComputerPlayer* QuickBattleStage::getComputerPlayer()
{
    return m_pComputer;
}

Building* QuickBattleStage::getPlayerBase()
{
    return m_pPlayerTower;
}

Building* QuickBattleStage::getComputerBase()
{
    return m_pComputerTower;
}

void QuickBattleStage::clear()
{
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    char plistPath[128]= {0};
    char pvrPath[128] = {0};
    
	for (const int& id : m_UsableEnemies)
    {
        sprintf(plistPath, "enemy/%d/animation.plist",id);
        sprintf(pvrPath, "enemy/%d/animation.pvr.ccz",id);
        frameCache->removeSpriteFramesFromFile(plistPath);
        
    }
    sprintf(plistPath, "enemy/%d/animation.plist",6);
    frameCache->removeSpriteFramesFromFile(plistPath);
    
    if (m_pPlayerTower)
    {
        m_pPlayerTower->removeFromParentAndCleanup(true);
        m_pPlayerTower = NULL;
    }
    
    if (m_pComputerTower)
    {
        m_pComputerTower->removeFromParentAndCleanup(true);
        m_pComputerTower = NULL;
    }
    
    GameStage::clear();
    
}

void QuickBattleStage::removeBuilding(Building *pBuilding)
{
    if (!GameScene::getInstance()->sharedGameStage->isReady())
    {
        return;
    }
    
    if (pBuilding == m_pPlayerTower)
    {
        m_pPlayerTower = NULL;
        
        // lose
        GameScene::getInstance()->sharedMainUI->getStageUI()->showGameOverMenu();
    }
    else if (pBuilding == m_pComputerTower)
    {
        m_pComputerTower = NULL;
        completeStage();
    }
}

void QuickBattleStage::reset()
{
    stopAllActions();

    GameStage::reset();

    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    char plistPath[128]= {0};
    char pvrPath[128] = {0};

    for(int i=1;i<100;++i)
    {
        const EnemySetting* pSetting = GameData::getEnemySetting(i);
        if (!pSetting ||
            (pSetting->type != ENUM_ENEMY_TYPE_NORMAL))
        {
            continue;
        }
        
        m_UsableEnemies.push_back(i);
        
        sprintf(plistPath, "enemy/%d/animation.plist",i);
        sprintf(pvrPath, "enemy/%d/animation.pvr.ccz",i);
        frameCache->addSpriteFramesWithFile(plistPath, pvrPath);
    }

    sprintf(plistPath, "enemy/%d/animation.plist",6);
    sprintf(pvrPath, "enemy/%d/animation.pvr.ccz",6);
    frameCache->addSpriteFramesWithFile(plistPath, pvrPath);

    m_pComputer = ComputerPlayer::create(6, pGroundMap->getStartTilePositions()[0]);
    m_pComputer->Enemy::init(1);
    pGroundMap->addChild(m_pComputer);
    m_pComputer->activate();
    

    CCPoint startPos = pGroundMap->getStartTilePositions()[0];
    pBuildingManager->placeBuilding(startPos.x, startPos.y, 100, true, false);
    m_pComputerTower = pBuildingManager->getBuilding(startPos.x, startPos.y);
    
    CCPoint endPos = pGroundMap->getEndTilePosition();
    
    
    pBuildingManager->placeBuilding(endPos.x, endPos.y, 100, true, true);
    m_pPlayerTower = pBuildingManager->getBuilding(endPos.x, endPos.y);

    runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCCallFunc::create(this, callfunc_selector(QuickBattleStage::loadEnemies))
                                                                            ,CCDelayTime::create(5),
                                                                            NULL)));
}

void QuickBattleStage::setupWinningCondition()
{
    
    m_CurrentWinningCondition.killPlayer = false;
    m_CurrentWinningCondition.killNpcId = 0;
    m_CurrentWinningCondition.minKilledEnemies = 0;
    m_CurrentWinningCondition.maxPassEnemies = 0;
    m_CurrentWinningCondition.maxTime = 0;
    m_CurrentWinningCondition.destroyBase = true;
}

void QuickBattleStage::loadEnemies()
{
    
    int lastTileIndex = pGroundMap->getSequenceByTile(pGroundMap->getEndTilePosition());
    
    
    int count = 0;
    while (count < 1)
    {
        int id = m_UsableEnemies[rand()%m_UsableEnemies.size()];
        
        
        Enemy * pEnemy = Enemy::create(id, pGroundMap->getTileBySequence(1 + rand()%4));
        pEnemy->init(m_pCurrentMapConfig->maxLevel);
        pGroundMap->addChild(pEnemy);
        enemies->addObject(pEnemy);
        pEnemy->activate();
        
        ++count;
    }
    

}

