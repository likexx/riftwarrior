//
//  ComputerPlayer.cpp
//  tdgame
//
//  Created by Like Zhang on 5/6/13.
//
//

#include "ComputerPlayer.h"
#include "GameStage.h"
#include "GameScene.h"
#include "MapHelper.h"
#include "ComputerPlayerAttackComponent.h"
#include "EnemyAnimation.h"

#define LEVEL_LABEL_TAG 6002

ComputerPlayer* ComputerPlayer::create(int id, CCPoint startTilePos)
{
    ComputerPlayer* computerPlayer = new ComputerPlayer(id);
    
    if (computerPlayer)
    {
        computerPlayer->autorelease();
    }
    
    computerPlayer->pAnimation->reset();
    computerPlayer->currentHp = computerPlayer->maxHp;
    
    computerPlayer->setToTile(startTilePos.x, startTilePos.y);
    
    return computerPlayer;
}

ComputerPlayer::ComputerPlayer(int id)
:Enemy(id)
{
    m_Experience = 0;
    m_Money = 500;
    m_Gems = 100;
}

bool ComputerPlayer::init()
{
    return true;
}

void ComputerPlayer::remove()
{
    
    m_pAttackComponent->removeFromParentAndCleanup(true);
    m_pAttackComponent = NULL;
    
    pAnimation->removeFromParentAndCleanup(true);
    pAnimation= NULL;
    
    this->active = false;
    this->unscheduleUpdate();
    
    this->setVisible(false);
    

}


void ComputerPlayer::revive()
{
    pAnimation = EnemyAnimation::create(id, 0);
    addChild(pAnimation);
    
    m_MoveToEnd = false;
    m_RandSeed = rand();
    m_DestinationTile = GameScene::getInstance()->sharedGameStage->getGroundMap()->getEndTilePosition();
    
    CCPoint startPos = GameScene::getInstance()->sharedGameStage->getGroundMap()->getStartTilePositions()[0];
    
    setToTile(startPos.x, startPos.y);
    resetStatus();
    
    createAttackComponent();
    
    m_pAttackComponent->setTargetType(ATTACK_TARGET_ENEMY);
    this->addChild(m_pAttackComponent);
    
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    int endSequence = pMap->getSequenceByTile(pMap->getEndTilePosition());
    m_DestinationTile = pMap->getTileBySequence(rand()%(endSequence / 2) + 2);
    
    pAnimation->setupBossAnimation();
    
    setVisible(true);
    pAnimation->setVisible(true);
    
    setVisible(true);
    
    
    this->active = true;

    m_pAttackComponent->scheduleUpdate();
    //    pAnimation->scheduleUpdate();
    this->scheduleUpdate();
    
}


void ComputerPlayer::createAttackComponent()
{
    m_pAttackComponent = ComputerPlayerAttackComponent::create();
    
}

void ComputerPlayer::updateDestination()
{
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    if (currentHp < getMaxHp()/2)
    {
        m_DestinationTile = pMap->getStartTilePositions()[0];
    }
    else if (currentHp >= getMaxHp() &&
             (m_DestinationTile.x != pMap->getEndTilePosition().x || m_DestinationTile.y != pMap->getEndTilePosition().y))
    {
        m_DestinationTile = pMap->getEndTilePosition();
    }

}


void ComputerPlayer::updateExperience(int delta)
{
    m_Experience += delta;
    int nextLevelExp = getNextLevelExperience();
    
    if (m_Experience > nextLevelExp)
    {
        m_Experience -= nextLevelExp;
        Enemy::init(m_Level+1);
        
        if (pAnimation && pAnimation->getBossStar())
        {
            CCLabelBMFont* level = static_cast<CCLabelBMFont*>(pAnimation->getBossStar()->getChildByTag(LEVEL_LABEL_TAG));
            if (level)
            {
                char buffer[32] = {0};
                sprintf(buffer, "lv.%d", getLevel());
                level->setString(buffer);
            }
            
        }

    }

}


void ComputerPlayer::update(float dt)
{
    if (!isActive() && isDead())
    {
        return;
    }
    
    if (!m_pAttackComponent->hasTarget())
    {
        if (currentHp < getMaxHp())
        {
            currentHp += getMaxHp() * dt/30;
        }
        
        if (currentHp > getMaxHp())
        {
            currentHp = getMaxHp();
        }
    }
    
    
    if (pAnimation && pAnimation->getBossStar())
    {
        CCLabelBMFont* level = static_cast<CCLabelBMFont*>(pAnimation->getBossStar()->getChildByTag(LEVEL_LABEL_TAG));
        
        if (!level)
        {
            CCSprite* pBossStar = pAnimation->getBossStar();
            char buffer[32] = {0};
            sprintf(buffer, "lv.%d", getLevel());
            level = CCLabelBMFont::create(buffer, "font/arial.fnt");
            level->setAnchorPoint(ccp(0,0.5));
            level->setPosition(ccp(pBossStar->getContentSize().width, pBossStar->getContentSize().height/2));
            pBossStar->addChild(level);
        }
        
    }
    
    Enemy::update(dt);
}