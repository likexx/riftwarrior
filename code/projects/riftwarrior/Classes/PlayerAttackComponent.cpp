//
//  PlayerAttackComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 2/7/13.
//
//

#include "PlayerAttackComponent.h"
#include "Player.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

bool PlayerAttackComponent::init()
{
    if (!EnemyAttackComponent::init())
    {
        return false;
    }

    // FIXME
    //SimpleAudioEngine::sharedEngine()->preloadEffect("sound/player_attack1.mp3");
    //SimpleAudioEngine::sharedEngine()->preloadEffect("sound/player_attack2.mp3");
    //SimpleAudioEngine::sharedEngine()->preloadEffect("sound/player_attack3.mp3");
    return true;
}

void PlayerAttackComponent::update(float dt)
{
    CCPoint targetPos = CCPointZero;
    
    Player* pPlayer = static_cast<Player*>(getParent());
    if (!pPlayer->ableToAttack() || !m_attackMoveFinished)
    {
        return;
    }
    
    if (selectedTarget)
    {
        if (!selectedTarget->isActive() ||
            selectedTarget->isDead() ||
            !isWithinAttackRange(selectedTarget->getAttackableMapPosition()))
        {
            //            removeTarget();
            pPlayer->finishAttack();
        }
    }
    else
    {
        selectedTarget = findTarget();
        if (selectedTarget)
        {
            selectedTarget->retain();
            targetPos = selectedTarget->getAttackablePosition();
        }
    }
    if (selectedTarget)
    {
        aimAtTarget(selectedTarget->getAttackablePosition());
    }

    
}

AttackableObject* PlayerAttackComponent::findTarget()
{
    CCArray* enemies = GameScene::getInstance()->sharedGameStage->enemies;
    
    Player* pPlayer = static_cast<Player*>(this->getParent());
    
    if (AttackableObject* pComputerPlayer = GameScene::getInstance()->sharedGameStage->getComputerPlayer())
    {
        if (!pComputerPlayer->isDead() &&
            isWithinAttackRange(pComputerPlayer->getAttackableMapPosition()))
        {
            return pComputerPlayer;
        }
    }
    
    
    if (Building* pBase = GameScene::getInstance()->sharedGameStage->getComputerBase())
    {
        if (isWithinAttackRange(pBase->getAttackableMapPosition()))
        {
            return pBase;
        }
    }
    
    
    CCObject* pTarget = NULL;
    CCARRAY_FOREACH(enemies, pTarget)
    {
        Enemy* enemy = static_cast<Enemy*>(pTarget);
        
        if (!enemy->isActive() || enemy->isDead())
        {
            continue;
        }
        
        CCPoint targetMapPos = enemy->getCurrentMapPosition();
        
        if (isWithinAttackRange(targetMapPos))
        {
            return enemy;
        }
        
    }
    
    return NULL;
}

bool PlayerAttackComponent::isWithinAttackRange(const cocos2d::CCPoint &targetMapPos)
{
    Player* pPlayer = static_cast<Player*>(this->getParent());
    
    CCPoint currentPlayerMapPos = pPlayer->getAttackableMapPosition();
    int range = pPlayer->getAttackRange();
    
    return AttackComponent::isWithinAttackRange(currentPlayerMapPos, targetMapPos, range, range*range);
    
}


void PlayerAttackComponent::startAttack()
{
    m_attackMoveFinished = true;
    
    if (!selectedTarget)
    {
        return;
    }
    
    //    CCPoint targetPos = selectedTarget->getAttackablePosition();
    
    CCPoint targetMapPos = selectedTarget->getAttackableMapPosition();
    if (!isWithinAttackRange(targetMapPos))
    {
        selectedTarget = NULL;
        return;
    }
    
    //    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    
    Player* pPlayer = static_cast<Player*>(this->getParent());
    
    if (selectedTarget->isDead())
    {
        pPlayer->finishAttack();
        return;
    }
    
    m_attackMoveFinished = false;
    
    PlayerAnimation* pAnimation = pPlayer->getAnimation();
    
    CCCallFuncO* callback = CCCallFuncO::create(this, callfuncO_selector(PlayerAttackComponent::hurtTarget), NULL);
    
    ENUM_NPC_ANIMATION animationType;
    
    switch (m_Quadrant) {
        case 1:
            animationType = ENUM_NPC_ANIMATION_ATTACK_RIGHT;
            break;
        case 2:
            animationType = ENUM_NPC_ANIMATION_ATTACK_TOPRIGHT;
            break;
        case 3:
            animationType = ENUM_NPC_ANIMATION_ATTACK_TOP;
            break;
        case 4:
            animationType = ENUM_NPC_ANIMATION_ATTACK_TOPLEFT;
            break;
        case 5:
            animationType = ENUM_NPC_ANIMATION_ATTACK_LEFT;
            break;
        case 6:
            animationType = ENUM_NPC_ANIMATION_ATTACK_LEFTBOTTOM;
            break;
        case 7:
            animationType = ENUM_NPC_ANIMATION_ATTACK_BOTTOM;
            break;
        case 8:
            animationType = ENUM_NPC_ANIMATION_ATTACK_RIGHTBOTTOM;
            break;
        default:
            break;
    }
    
    pAnimation->setAnimationWithCallback(animationType, callback);
    
    char name[64] = {0};
    sprintf(name, "sound/player_attack%d.mp3", rand()%3 + 1);
    // FIXME
    SimpleAudioEngine::sharedEngine()->playEffect(name);
}

void PlayerAttackComponent::hurtTarget(CCNode* pBullet)
{
    Player* pPlayer = static_cast<Player*>(this->getParent());
    
    m_attackMoveFinished = true;
    
    if (!selectedTarget)
    {
        if (pBullet)
        {
            pBullet->stopAllActions();
        }
    }
    else
    {
        if ((pBullet && bulletHitTarget(pBullet->getPosition(), selectedTarget->getAttackablePosition())) || !pBullet)
        {

            if (!selectedTarget->isDead())
            {
                selectedTarget->reduceHp(pPlayer->getDamage());

                pPlayer->getHitBonus(static_cast<Enemy*>(selectedTarget));

                if (selectedTarget->isDead())
                {
                    pPlayer->getKillBonus(static_cast<Enemy*>(selectedTarget));
                    
                }
                else
                {
                    selectedTarget->actWhenAttacked(pPlayer);
                }
            }
            
        }
        
        if (selectedTarget->isDead())
        {
            
            if (pBullet)
            {
                pBullet->stopAllActions();
            }
            
            
        }
    }
    
    pPlayer->finishAttack();
    
}


