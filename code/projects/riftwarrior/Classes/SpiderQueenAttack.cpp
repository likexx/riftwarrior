//
//  SpiderQueenAttack.cpp
//  tdgame
//
//  Created by Like Zhang on 5/1/13.
//
//

#include "SpiderQueenAttack.h"
#include "Enemy.h"
#include "AnimationManager.h"
#include "GameScene.h"
#include "MapHelper.h"
#include "Player.h"

// magic sprite starts from 3000
#define SPIDER_QUEEN_POISON_TAG 3002

bool SpiderQueenAttack::init()
{
    
    if (!EnemyAttackComponent::init())
    {
        return false;
    }
    
    m_AttackStarted = false;
    m_LastActionTime = 0;
    
    return true;
    
}


void SpiderQueenAttack::update(float dt)
{
    if (GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }
    
    if (!m_attackMoveFinished)
    {
        return;
    }
    
    m_LastActionTime+=dt;
    
    if (Player::getInstance()->isDead() ||
        !Player::getInstance()->isActive())
    {
        return;
    }

    Enemy* pEnemy = static_cast<Enemy*>(getParent());

    if (GameScene::getInstance()->sharedGameStage->getGroundMap()->getChildByTag(SPIDER_QUEEN_POISON_TAG))
    {
        // attack animation is still running
        if (m_AttackStarted)
        {
            CCPoint playerTile = Player::getInstance()->getCurrentMapTile();

			for (const CCPoint& tilePos : m_AttackingPositions)
            {
                if ((int)playerTile.x == (int)tilePos.x &&
                    (int)playerTile.y == (int)tilePos.y)
                {
                    Player::getInstance()->setStatus(ENUM_BUFF_STATUS_POISON, pEnemy->getDamage(), 3);
                }
            }
        }
    }
    else
    {
        // attack animation is done. reset data
        m_AttackingPositions.clear();
        m_AttackingPositions.resize(0);
        m_AttackStarted = false;
    }
    
    if (m_LastActionTime > pEnemy->getEnemySetting()->attackDelay)
    {
        startAttack();
    }
    
}

void SpiderQueenAttack::startAttack()
{
    m_LastActionTime = 0;
    m_attackMoveFinished = false;

    Enemy* pEnemy = static_cast<Enemy*>(getParent());
    
    EnemyAnimation* pAnimation = pEnemy->getAnimation();
    
    CCCallFunc* pCallback = CCCallFunc::create(this, callfunc_selector(SpiderQueenAttack::hurtTarget));

    ENUM_NPC_ANIMATION animationType;
    
    switch (pEnemy->getMoveDirection())
    {
        case ENUM_MOVE_DIRECTION_UP:
            animationType = ENUM_NPC_ANIMATION_ATTACK_TOPRIGHT;
            break;
        case ENUM_MOVE_DIRECTION_DOWN:
            animationType = ENUM_NPC_ANIMATION_ATTACK_LEFTBOTTOM;
            break;
        case ENUM_MOVE_DIRECTION_LEFT:
            animationType = ENUM_NPC_ANIMATION_ATTACK_TOPLEFT;
            break;
        case ENUM_MOVE_DIRECTION_RIGHT:
            animationType = ENUM_NPC_ANIMATION_ATTACK_RIGHTBOTTOM;
            break;
    }
    
    pAnimation->setAnimationWithCallback(animationType, pCallback);
}

void SpiderQueenAttack::finishAttack()
{
    m_attackMoveFinished = true;
    
    Enemy* pEnemy = static_cast<Enemy*>(getParent());
    EnemyAnimation* pAnimation = pEnemy->getAnimation();
    ENUM_NPC_ANIMATION animationType;

    switch (pEnemy->getMoveDirection())
    {
        case ENUM_MOVE_DIRECTION_UP:
            animationType = ENUM_NPC_ANIMATION_MOVING_UP;
            break;
        case ENUM_MOVE_DIRECTION_DOWN:
            animationType = ENUM_NPC_ANIMATION_MOVING_DOWN;
            break;
        case ENUM_MOVE_DIRECTION_LEFT:
            animationType = ENUM_NPC_ANIMATION_MOVING_LEFT;
            break;
        case ENUM_MOVE_DIRECTION_RIGHT:
            animationType = ENUM_NPC_ANIMATION_MOVING_RIGHT;
            break;
    }
    
    pAnimation->setAnimation(animationType);
}

void SpiderQueenAttack::hurtTarget(cocos2d::CCNode *pBullet)
{
    
    Enemy* pEnemy = static_cast<Enemy*>(getParent());
    
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    
    CCPoint lastTilePos = pMap->getEndTilePosition();
    int lastIndex = pMap->getSequenceByTile(lastTilePos);
    
    int i = 1;

    do
    {
        CCPoint playerTile = Player::getInstance()->getCurrentMapTile();

        CCPoint tilePos = pMap->getTileBySequence(i);
        
        CCPoint pos = MapHelper::tilePosToGroundMapPos(tilePos);
        
        Enemy* pEnemy = static_cast<Enemy*>(getParent());
        
        CCSmartSprite* pMagicAnimSprite = AnimationManager::getInstance()->createMagicAnimation(pEnemy->getEnemySetting()->magicType, 2);
        
        pMagicAnimSprite->setAnchorPoint(ccp(0.6,0.4));
        
        pMagicAnimSprite->setPosition(pos);
        
        GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(pMagicAnimSprite, 0, SPIDER_QUEEN_POISON_TAG);

        this->runAction(CCSequence::create(CCDelayTime::create(0.3f),
                                           CCCallFuncO::create(this, callfuncO_selector(SpiderQueenAttack::hurtPlayer), pMagicAnimSprite),
                                           NULL
                                           ));

        m_AttackingPositions.push_back(tilePos);
        
        i+= rand()%4;
        
        
    } while (i<=lastIndex);
    
    CCSize mapSize = pMap->getMapSize();
    
    for(int i=0;i<mapSize.height;++i)
    {
        for (int j=0;j<mapSize.width;++j)
        {
            if (pMap->isInsidePlaceableArea(ccp(j, i)))
            {
                Building* pBuilding = GameScene::getInstance()->sharedGameStage->getBuildingManager()->getBuilding(j, i);
                if (pBuilding && pBuilding->isActive())
                {

                    if (true || rand()%2 == 0)
                    {
                        
                        CCPoint pos = MapHelper::tilePosToGroundMapPos(ccp(j, i));
                        
                        Enemy* pEnemy = static_cast<Enemy*>(getParent());
                        
                        CCSmartSprite* pMagicAnimSprite = AnimationManager::getInstance()->createMagicAnimation(pEnemy->getEnemySetting()->magicType, 2);
                        
                        pMagicAnimSprite->setAnchorPoint(ccp(0.6,0.4));
                        
                        pMagicAnimSprite->setPosition(pos);
                        
                        GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(pMagicAnimSprite);

                        pBuilding->reduceHp(pEnemy->getDamage());
                        
                        if (pBuilding->isDead())
                        {
                            pBuilding->destroy();
                        }
                    }
                    
                }
            }
        }
    }
    
//    scheduleOnce(schedule_selector(EnemyAttackComponent::makeRoundAreaDamage), 1.0);

    finishAttack();
}

void SpiderQueenAttack::hurtPlayer(CCNode* pNode)
{
    if (!Player::getInstance()->isActive() ||
        Player::getInstance()->isDead())
    {
        return;
    }
    
    m_AttackStarted = true;
    
}


