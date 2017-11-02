//
//  Player.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/7/12.
//
//

#include "Player.h"

#include <cocos2d.h>

#include <unordered_set>

#include "GameScene.h"
#include "GameData.h"
#include "MapHelper.h"
#include "MessageBox.h"
#include "PlayerAttackComponent.h"
#include "BuildingBase.h"
#include "SmartAnimation.h"
#include "AnimationManager.h"
#include "GameControlMenu.h"
#include "WorldmapGameMenu.h"
#include "SimpleAudioEngine.h"
#include "constant.h"

using namespace cocos2d;
using namespace std;

#define PLAYER_MOVE_SPEED 45

int loadRecordCount( void * para, int n_column, char ** column_value, char ** column_name )
{
    int *count=(int*)para;
    *count=n_column;
    return 0;
}

Player* Player::getInstance()
{
    static Player* currentPlayer;
    
    if (!currentPlayer) {
        currentPlayer = new Player();
    }
    
    return currentPlayer;
}

Player::Player():
	currentPerformingSkillId(0),
	m_Money(200),
	m_Gems(5),
	m_Level(1),
	m_MaxHp(300),
	m_Hp(400),
	m_MaxMp(100),
	m_Mp(100),
	m_Experience(0),
	m_Speed(PLAYER_MOVE_SPEED),
	m_AttackRange(35),
	m_MeleeDamage(20),
	m_CurrentMapId(1),
	m_SkillPoints(0),
	m_pAnimation(NULL),
	m_ProgressFlags(0),
	m_CurrentMoveDirection(ENUM_MOVE_DIRECTION_NONE),
	m_DestinationChanged(false),
	m_MovingToEndPosition(false),
	m_IsSetToMove(false),
	m_ForceToMove(false),
	m_Movable(true),
	m_PerformingSkillType(ENUM_PLAYER_PERFORM_SKILL_NONE),
	m_CriticalLayer(NULL)
{
//    m_ProgressFlags = 0xFFFFFFFF;
    // needs to set something in the table, otherwise empty strings will
    // be ignored and mess up the data
    m_Skills[1] = 0;
    m_CompletedMapScores[1] = 0;
    m_MapPurchased[1] = 0;
    m_KilledEnemies[1] = 0;
    
    
}

void Player::reset()
{
    m_Hp = m_MaxHp;
    m_Mp = m_MaxMp;
    m_CurrentMoveDirection = ENUM_MOVE_DIRECTION_NONE;
    m_DestinationChanged = false;
    m_MovingToEndPosition = false;
    m_KillCount = 0;
    m_IsSetToMove = false;
    m_WaitForSetUnitDestination = false;
    m_PerformingSkillType = ENUM_PLAYER_PERFORM_SKILL_NONE;
    addHp(0);
    resetStatus();
    
    m_NearbyBuildingTiles.clear();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    unscheduleUpdate();
    
//    int mapId = GameScene::getInstance()->sharedGameStage->getMapId();
    
//    m_Money = 200 + (50 * (mapId - 1));
}


void Player::showOnMap()
{
    reset();
    
    if (m_pAnimation)
    {
        m_pAnimation->clear();
        m_pAnimation->removeFromParentAndCleanup(true);
    }
    
    m_pAnimation = PlayerAnimation::create();
    m_pAnimation->init();
    m_pAnimation->setScale(0.6f);

    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    CCPoint tilePosition = pStage->getGroundMap()->getEndTilePosition();
    
    if (pStage->getWinningCondition()->destroyBase)
    {
        // should stand ahead of base
        int sequence = pStage->getGroundMap()->getSequenceByTile(tilePosition);
        tilePosition = pStage->getGroundMap()->getTileBySequence(sequence - 1);
    }
    

    CCPoint realTilePos = ccp(tilePosition.x + 0.5, tilePosition.y + 0.5);
    StageMap* pGroundMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint mapGroundPos = MapHelper::tilePosToGroundMapPos(realTilePos);
    m_CurrentMapPosition = MapHelper::tilePosToMapPos(realTilePos.x, realTilePos.y, pGroundMap);
    m_CurrentTilePosition = realTilePos;
    
    m_MoveDestinationMapPos = m_CurrentMapPosition;
    
    m_pAnimation->setPosition(mapGroundPos);
    this->addChild(m_pAnimation);
    m_pAnimation->scheduleUpdate();
    
    m_OriginalStandAnimation = pGroundMap->getPlayerStandingDirection();
    
    m_pAnimation->setAnimation(m_OriginalStandAnimation);
    
    attackComponent = PlayerAttackComponent::create();
    this->addChild(attackComponent);
    attackComponent->scheduleUpdate();

    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    this->scheduleUpdate();
    
    pGroundMap->addChild(this);
}


void Player::showMessage(const char *pMessage)
{
    GameScene::getInstance()->sharedMainUI->getStageUI()->showNpcMessage("npc/player.png",pMessage);
}

void Player::actWhenAttacked(AttackableObject *pTarget)
{
    
}

void Player::finishAttack()
{
    attackComponent->removeTarget();
    if (isDead() || !isActive() || isPerformingSKill() || isStanding())
    {
        return;
    }

    if (reachedDestination())
    {
        tryToStand();
        m_IsSetToMove = false;
        GameScene::getInstance()->sharedGameStage->hidePlayerMoveMark();
    }
    else
    {
        switch(m_CurrentMoveDirection)
        {
            case ENUM_MOVE_DIRECTION_DOWN:
                m_pAnimation->setAnimation(ENUM_NPC_ANIMATION_MOVING_DOWN);
                break;
            case ENUM_MOVE_DIRECTION_RIGHT:
                m_pAnimation->setAnimation(ENUM_NPC_ANIMATION_MOVING_RIGHT);
                break;
            case ENUM_MOVE_DIRECTION_LEFT:
                m_pAnimation->setAnimation(ENUM_NPC_ANIMATION_MOVING_LEFT);
                break;
            case ENUM_MOVE_DIRECTION_UP:
                m_pAnimation->setAnimation(ENUM_NPC_ANIMATION_MOVING_UP);
                break;
            default:
                return;
                
        }
    }

    
}

bool Player::isSkillAlreadyTraining(int id)
{
    map<int, unsigned long>::iterator it = m_CurrentTrainingSkills.find(id);
    if (it != m_CurrentTrainingSkills.end()){
        return true;
    }else{
        return false;
    }
}

bool Player::tryStartTrainSkill(int id)
{
    if(this->m_Money >= Function::getSkillTrainingCost(id)){
        map<int, unsigned long>::iterator it = m_CurrentTrainingSkills.find(id);
        if (it != m_CurrentTrainingSkills.end())
        {
            // is already training
            return false;
        }
        
        m_CurrentTrainingSkills[id] = SystemHelper::getCurrentTimeInSeconds();
        this->updateMoney(-Function::getSkillTrainingCost(id));
        save();
        
        
        //flurry
        char msg[256] = {0};
        sprintf(msg, "start_train_skill_%d_%d", id, getSkillLevel(id));
        SystemHelper::logEvent(msg);
        
        return true;
    }else{
        return false;
    }
}


void Player::resetSkills()
{

}

CCPoint Player::getShootPosition()
{
    return getAttackablePosition();
}

CCPoint Player::getAttackablePosition()
{
    return m_pAnimation->getPosition();

}


void Player::updateMoney(int delta)
{
    if (delta == 0)
    {
        return;
    }
    
    m_Money += delta;
    
    if (!hasCompletedTutorial() && delta > 0)
    {
        m_Money += 4;
    }
}

void Player::updateGems(int delta)
{
    if (delta == 0)
    {
        return;
    }
    
    m_Gems += delta;
    
    saveGems();
}


void Player::purchaseMap(int mapId)
{
    int price = GameData::getMapConfig(mapId)->price;
    if (price > getMoney())
    {
        return;
    }

    updateMoney(-price);
    m_MapPurchased[mapId] = 1;
    
    save();
}

float Player::getDamage()
{
    float bonus = 0;

    int skillLevel = getSkillLevel(ENUM_SKILL_PLAYER_ATTACK_DAMAGE);
    if (skillLevel>0)
    {
        bonus = skillLevel*0.1;
    }
    return m_MeleeDamage * (1 + bonus);
}

float Player::getAttackRange()
{
    return m_AttackRange;
}

float Player::getAttackRate()
{
    return 0.05f;
}


float Player::getBaseSpeed()
{
    return PLAYER_MOVE_SPEED;
}

void Player::setSpeed(float speed)
{
    m_Speed = speed;
}

void Player::reduceHp(float damage, bool ignoreDefense)
{
    if (m_PerformingSkillType == ENUM_PLAYER_PERFORM_SKILL_WHIRLWIND)
    {
        // receive half damage when performing whirlwind
        damage/=2;
        
    }
    
    if (ignoreDefense)
    {
        addHp(-damage);
        return;
    }
    
    // defend value is depending on level
    float defense = getDefense();
    
    float receivedDamage = damage;
    
    const float minDamage = 1; // at least 1 points damage
    if (defense > damage)
    {
        receivedDamage = minDamage;
    }
    else
    {
        receivedDamage = max(damage - defense, minDamage);
    }
    
    addHp(-receivedDamage);
    
    GameScene::getInstance()->sharedGameStage->addMonsterKillScore(-receivedDamage);

}

CCNode* Player::getAnimationPart()
{
    return m_pAnimation;
}

bool Player::isDead()
{
    return m_Hp <= 0;
}

void Player::remove()
{
    if (getParent()!=NULL)
    {
        this->removeFromParentAndCleanup(true);
    }
}

bool Player::performSkill(int id)
{
    /*
    const Magic* pMagic = GameData::getMagic(id);
    if (!pMagic)
    {
        return false;
    }
    */
    if (!canPerformSkill(ENUM_PLAYER_PERFORM_SKILL_EARTH_SLASH))
    {
        return false;
        
    }
    
    currentPerformingSkillId = id;

    ENUM_NPC_ANIMATION prevAnimType = m_pAnimation->getCurrentAnimationType();
    
    finishAttack();
    
    ENUM_NPC_ANIMATION skillAnimDirection = ENUM_NPC_ANIMATION_NONE;
    
    ENUM_NPC_ANIMATION animDirectionStart = ENUM_NPC_ANIMATION_NONE;
    
    switch (id)
    {
        case 1:
            animDirectionStart = ENUM_NPC_ANIMATION_USE_SKILL_1_UP;
            break;
        case 2:
            animDirectionStart = ENUM_NPC_ANIMATION_USE_SKILL_2_UP;
            break;
        case 3:
            // for hp recovery, there is no special animation, just standing still
            animDirectionStart = ENUM_NPC_ANIMATION_STAND_UP;
            break;
            
    }
    
    ENUM_MOVE_DIRECTION moveDirection = getAttackDirection(prevAnimType);
    
    if (moveDirection == ENUM_MOVE_DIRECTION_NONE)
    {
        switch (m_OriginalStandAnimation)
        {
            case ENUM_NPC_ANIMATION_STAND_UP:
                moveDirection = ENUM_MOVE_DIRECTION_UP;
                break;
            case ENUM_NPC_ANIMATION_STAND_DOWN:
                moveDirection = ENUM_MOVE_DIRECTION_DOWN;
                break;
            case ENUM_NPC_ANIMATION_STAND_LEFT:
                moveDirection = ENUM_MOVE_DIRECTION_LEFT;
                break;
            case ENUM_NPC_ANIMATION_STAND_RIGHT:
                moveDirection = ENUM_MOVE_DIRECTION_RIGHT;
                break;
        }
    }
    
    switch (moveDirection)
    {
        case ENUM_MOVE_DIRECTION_UP:
            skillAnimDirection = (ENUM_NPC_ANIMATION)(animDirectionStart+0);
            break;
        case ENUM_MOVE_DIRECTION_DOWN:
            skillAnimDirection = (ENUM_NPC_ANIMATION)(animDirectionStart+1);
            break;
        case ENUM_MOVE_DIRECTION_LEFT:
            skillAnimDirection = (ENUM_NPC_ANIMATION)(animDirectionStart+2);
            break;
        case ENUM_MOVE_DIRECTION_RIGHT:
            skillAnimDirection = (ENUM_NPC_ANIMATION)(animDirectionStart+3);
            break;
    }
    
    
    m_Mp-= GameData::getActiveSkillSetting(id)->mpCost * (1 - 0.05 * getSkillLevel(ENUM_SKILL_SKILL_FOCUS));

    
    m_PerformingSkillType = (ENUM_PLAYER_PERFORM_SKILL_TYPE)(ENUM_PLAYER_PERFORM_SKILL_NONE + id);

    m_PreviousAnimation = m_pAnimation->getCurrentAnimationType();
    
    
    switch (m_PerformingSkillType)
    {
        case ENUM_PLAYER_PERFORM_SKILL_EARTH_SLASH:
            // FIXME
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/start_earthslash.mp3");
            m_pAnimation->setAnimationWithCallback(skillAnimDirection, CCCallFunc::create(this, callfunc_selector(Player::performEarthSlash)));
            break;
        case ENUM_PLAYER_PERFORM_SKILL_RECOVER:
            m_pAnimation->setAnimation(skillAnimDirection);
            performRecovery();
            break;
    }
    
    showSkillNameAnimation(id);


    return true;
}

void Player::showSkillNameAnimation(int id)
{
    // show skill name
    CCSprite * pSprite = m_pAnimation->getCurrentSprite();
    if (pSprite)
    {
        const float distance = pSprite->getTextureRect().size.height/5;
        
        CCPoint targetPos = ccp(0, pSprite->getTextureRect().size.height/3+distance);
        
        SmartAnimation* pNameAnimation = SmartAnimation::create(0.5, true);
        
        
        CCLabelBMFont* message = CCLabelBMFont::create(GameData::getActiveSkillSetting(id)->name.c_str(), "font/perform_skill.fnt");
        message->setAnchorPoint(ccp(0.5,0));
        message->setPosition(ccp(0,targetPos.y - distance));
        
        pNameAnimation->addChild(message);
        
        m_pAnimation->addChild(pNameAnimation);
        
        message->runAction((CCActionInterval*)(CCSequence::create(CCFadeIn::create(0.1f),
                                                                  CCDelayTime::create(0.3f),
                                                                  CCFadeOut::create(0.1f),
                                                                  NULL)));
        pNameAnimation->runAction((CCActionInterval*)(CCSequence::create(
                                                                         CCMoveTo::create(0.5f, targetPos),
                                                                         CCCallFunc::create(pNameAnimation, callfunc_selector(SmartAnimation::removeSelf)),
                                                                         NULL)));
        
    }
    
}

void Player::performRecovery()
{
    
    attackComponent->removeTarget();

    CCSprite* pAnimation = AnimationManager::getInstance()->getRecoverHpAnimation();
    pAnimation->setAnchorPoint(ccp(0.5, 0.35));
    m_pAnimation->addChild(pAnimation);

    // FIXME
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/cure.mp3");

    runAction((CCActionInterval*)CCSequence::create(CCDelayTime::create(0.2f),
                                                    CCCallFunc::create(this, callfunc_selector(Player::resetToPrevAnimation)),
                                                    NULL));

    float recoverAmount = m_MaxHp * (GameData::getActiveSkillSetting(m_PerformingSkillType)->damageValue + ((float)m_Level * 0.01f));
    // show HP increased animation
    CCSprite * pSprite = m_pAnimation->getCurrentSprite();
    if (pSprite)
    {
        const float distance = pSprite->getTextureRect().size.height/5;
        
        CCPoint targetPos = ccp(0, pSprite->getTextureRect().size.height/3+distance);
        
        SmartAnimation* pAnimation = SmartAnimation::create(0.5, true);
        
        char buffer[128] = {0};
        sprintf(buffer, "HP +%d",(int)recoverAmount);
        
        CCLabelBMFont* message = CCLabelBMFont::create(buffer, "font/perform_skill.fnt");
        message->setAnchorPoint(ccp(0.5,0));
        message->setPosition(ccp(0,targetPos.y - distance));
        
        pAnimation->addChild(message);
        
        m_pAnimation->addChild(pAnimation);
        
        message->setOpacity(0);
        
        message->runAction((CCActionInterval*)(CCSequence::create(CCDelayTime::create(0.3f),
                                                                  CCFadeIn::create(0.1f),
                                                                  CCDelayTime::create(0.3f),
                                                                  CCFadeOut::create(0.1f),
                                                                  NULL)));
        
        pAnimation->runAction((CCActionInterval*)(CCSequence::create(CCDelayTime::create(0.3f),
                                                                     CCMoveTo::create(0.5f, targetPos),
                                                                     CCCallFunc::create(pAnimation, callfunc_selector(SmartAnimation::removeSelf)),
                                                                     NULL)));
        
    }

    
    addHp(recoverAmount);
    
}

void Player::performEarthSlash()
{
    
    attackComponent->removeTarget();

    CCPoint tilePos = m_CurrentTilePosition;
    tilePos.x = (int)tilePos.x + 0.5;
    tilePos.y = (int)tilePos.y + 0.5;
    int range = 2 + getSkillLevel(ENUM_SKILL_EARTH_SPLASH);

    for (int i=0;i<range;++i)
    {
        switch (m_pAnimation->getCurrentAnimationType())
        {
            case ENUM_NPC_ANIMATION_USE_SKILL_2_UP:
                tilePos.y--;
                break;
            case ENUM_NPC_ANIMATION_USE_SKILL_2_DOWN:
                tilePos.y++;
                break;
            case ENUM_NPC_ANIMATION_USE_SKILL_2_LEFT:
                tilePos.x--;
                break;
            case ENUM_NPC_ANIMATION_USE_SKILL_2_RIGHT:
                tilePos.x++;
                break;
        }
        
        CCPoint groundPos = MapHelper::tilePosToGroundMapPos(tilePos);
        
        // pSprite is just used to hold position
        CCSprite* pSprite = CCSprite::create();
        pSprite->setPosition(groundPos);
        pSprite->retain();

        if (i == 0)
        {
            showEarthSlashSprite(pSprite);
        }
        else
        {
            this->runAction(
				(CCActionInterval*)CCSequence::create(
					CCDelayTime::create(0.3f * (i - 1)),
					CCCallFuncO::create(this, callfuncO_selector(Player::showEarthSlashSprite), pSprite),
					NULL));
        }
        
    }
    
    
    this->runAction((CCActionInterval*)CCSequence::create(CCDelayTime::create(0.2f),
                                                          CCCallFunc::create(this, callfunc_selector(Player::resetToPrevAnimation)),
                                                          NULL));
}

void Player::showEarthSlashSprite(CCObject * pObject)
{
	CCSprite * pSprite = static_cast<CCSprite *>(pObject);
    CCSmartSprite* pSkillSprite = AnimationManager::getInstance()->createSkillAnimation(ENUM_PLAYER_PERFORM_SKILL_EARTH_SLASH, 0.4f);
    pSkillSprite->setPosition(pSprite->getPosition());
    
    pSprite->release();
    
    pSkillSprite->setAnchorPoint(ccp(0.5, 0.25));
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(pSkillSprite);
    
    // FIXME
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/earthslash.mp3");
    
    pSkillSprite->runAction(
		(CCActionInterval*)CCSequence::create(
			CCDelayTime::create(0.25),
			CCCallFuncO::create(this, callfuncO_selector(Player::hurtWithEarthSlash), pSkillSprite),
			CCDelayTime::create(0.25),
			CCCallFunc::create(pSkillSprite, callfunc_selector(CCSmartSprite::removeSelf)),
			NULL));
}


void Player::hurtWithEarthSlash(CCObject * pObject)
{
	CCSprite * pSprite = static_cast<CCSprite *>(pObject);
    CCPoint tilePos = MapHelper::groundMapPosToTilePos(pSprite->getPosition(), GameScene::getInstance()->sharedGameStage->getGroundMap(), false);
    
    CCArray* targets = GameScene::getInstance()->sharedGameStage->enemies;
    
    CCObject* pTarget = NULL;
    
    
    float damage = getDamage()*0.5 + GameData::getActiveSkillSetting(ENUM_PLAYER_PERFORM_SKILL_EARTH_SLASH)->damageValue;
    damage *= 1 + getSkillLevel(ENUM_SKILL_EARTH_SPLASH) * 0.1;
    
    CCARRAY_FOREACH(targets, pTarget)
    {
        
        Enemy* enemy = static_cast<Enemy*>(pTarget);
        
        if (!enemy->isActive() || enemy->isDead())
        {
            continue;
        }
        
        CCPoint enemyTilePos = enemy->getCurrentTilePosition();
        
        
        if ((int)enemyTilePos.x == (int)tilePos.x && (int)enemyTilePos.y == (int)tilePos.y)
        {
            enemy->reduceHp(damage);
            
            if (enemy->isDead())
            {
                getKillBonus(enemy);
            }
            
            enemy->actWhenAttacked(this);
            
        }
    }

}

void Player::resetToPrevAnimation()
{
    m_pAnimation->setAnimation(m_PreviousAnimation);
    m_PerformingSkillType = ENUM_PLAYER_PERFORM_SKILL_NONE;
    updateDirection();
}

void Player::startWhirlWind()
{
    if (!canPerformSkill(ENUM_PLAYER_PERFORM_SKILL_WHIRLWIND))
    {
        return;
    }
    
    
    ENUM_NPC_ANIMATION skillAnimDirection = ENUM_NPC_ANIMATION_NONE;
    
    ENUM_NPC_ANIMATION animDirectionStart = animDirectionStart = ENUM_NPC_ANIMATION_USE_SKILL_1_UP;
    
    ENUM_MOVE_DIRECTION moveDirection = getAttackDirection(m_pAnimation->getCurrentAnimationType());
    
    if (moveDirection == ENUM_MOVE_DIRECTION_NONE)
    {
        switch (m_OriginalStandAnimation)
        {
            case ENUM_NPC_ANIMATION_STAND_UP:
                moveDirection = ENUM_MOVE_DIRECTION_UP;
                break;
            case ENUM_NPC_ANIMATION_STAND_DOWN:
                moveDirection = ENUM_MOVE_DIRECTION_DOWN;
                break;
            case ENUM_NPC_ANIMATION_STAND_LEFT:
                moveDirection = ENUM_MOVE_DIRECTION_LEFT;
                break;
            case ENUM_NPC_ANIMATION_STAND_RIGHT:
                moveDirection = ENUM_MOVE_DIRECTION_RIGHT;
                break;
        }
    }
    
    switch (moveDirection)
    {
        case ENUM_MOVE_DIRECTION_UP:
            skillAnimDirection = (ENUM_NPC_ANIMATION)(animDirectionStart+0);
            break;
        case ENUM_MOVE_DIRECTION_DOWN:
            skillAnimDirection = (ENUM_NPC_ANIMATION)(animDirectionStart+1);
            break;
        case ENUM_MOVE_DIRECTION_LEFT:
            skillAnimDirection = (ENUM_NPC_ANIMATION)(animDirectionStart+2);
            break;
        case ENUM_MOVE_DIRECTION_RIGHT:
            skillAnimDirection = (ENUM_NPC_ANIMATION)(animDirectionStart+3);
            break;
    }


    m_pAnimation->setAnimation(skillAnimDirection);

    m_PerformingSkillType = ENUM_PLAYER_PERFORM_SKILL_WHIRLWIND;
    
    showSkillNameAnimation(m_PerformingSkillType);

    // FIXME
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/start_whirlwind.mp3");
}

void Player::stopWhirlWind()
{
    
    finishAttack();
    
    m_PerformingSkillType = ENUM_PLAYER_PERFORM_SKILL_NONE;
}

///
///  dt: frame interval, passing from update() method
///
void Player::performWhirlWind(float dt)
{
    float mpCostPerSecond = GameData::getActiveSkillSetting(m_PerformingSkillType)->mpCost * (1 - 0.05 * getSkillLevel(ENUM_SKILL_SKILL_FOCUS));
    
    float mpCost = mpCostPerSecond * dt;
    
    if (m_Mp < mpCost)
    {
        stopWhirlWind();
        return;
    }
    
    m_Mp -= mpCost;
    
    
    CCArray* targets = GameScene::getInstance()->sharedGameStage->enemies;
    
    CCObject* pTarget = NULL;
    int attackRange = getAttackRange() * 1.5;
    int rangePowered = attackRange * attackRange;
    
    float damage = getDamage() * GameData::getActiveSkillSetting(m_PerformingSkillType)->damageValue;
    damage += getDamage() * getSkillLevel(ENUM_SKILL_WHIRLWIND) * 0.1;
    
    int randValue = (int)((float)clock())/(CLOCKS_PER_SEC)%100;

    if (randValue > (40 + 5 * getSkillLevel(ENUM_SKILL_WHIRLWIND)))
    {
        // check hit per second
        return;
    }

    CCARRAY_FOREACH(targets, pTarget)
    {
        
        Enemy* enemy = static_cast<Enemy*>(pTarget);
        
        if (!enemy->isActive() || enemy->isDead())
        {
            continue;
        }
        
        CCPoint targetPos = enemy->getAttackablePosition();
        
        const CCPoint& targetMapPos = MapHelper::isoMapPosToOrthMapPos(targetPos);
        
        //            const CCPoint& targetMapPos = pTargetToAim->getAttackableMapPosition();
        
        if (AttackComponent::isWithinAttackRange(m_CurrentMapPosition, targetMapPos, attackRange, rangePowered))
        {
            enemy->reduceHp(damage * 0.1);
            
            if (enemy->isDead())
            {
                getKillBonus(enemy);
            }
            enemy->actWhenAttacked(this);

        }
    }

}

void Player::getKillBonus(Enemy *pEnemy)
{
    int level = getSkillLevel(ENUM_SKILL_LEECH_HP);
    
    if (level > 0)
    {
        addHp((float)getMaxHp() * 0.02 * level);
    }
    
    level = getSkillLevel(ENUM_SKILL_LEECH_MP);
    if (level > 0)
    {
        m_Mp +=(float)getMaxMp() * 0.02 * level;
        
        if (m_Mp > getMaxMp())
        {
            m_Mp = getMaxMp();
        }
    }
    
    pEnemy->setKilledByPlayer();

    float damage = pEnemy->getDamage();
    float defense = getDefense();
    
    float ratio = 1;
    
    if (damage < defense * 1.5)
    {
        if (damage < defense)
        {
            ratio = 0.0f;
        }
        else if (damage < defense * 1.2f)
        {
            ratio = 0.5f;
        }
        else
        {
            ratio = 0.8f;
        }
    }
    
    updateExperience(ratio * pEnemy->getExperience());
    updateMoney(ratio * pEnemy->getDropMoney());
    
    increaseKillCount();
    
}

void Player::getHitBonus(Enemy *pEnemy)
{

    
}

bool Player::isActive()
{
    return true;
}

void Player::setMoveDestination(const cocos2d::CCPoint &groundMapPos, bool placeMark)
{
    CCPoint tilePos = MapHelper::groundMapPosToTilePos(groundMapPos, GameScene::getInstance()->sharedGameStage->getGroundMap(), true);
    
    if (isWaitingForPlaceUnitDestination())
    {
        Building* base = GameScene::getInstance()->sharedGameStage->getPlayerBase();
        BuildingBase* baseComponent = static_cast<BuildingBase*>(base->getAttackComponent());
        baseComponent->setDestinationTile(tilePos);
        GameScene::getInstance()->sharedGameStage->placeUnitMoveMark(groundMapPos);
        setWaitingForPlaceUnitDestination(false);
        GameScene::getInstance()->sharedMainUI->getStageUI()->removeHint();
    }
    else
    {
        m_MoveDestinationMapPos = MapHelper::tilePosToMapPos(tilePos.x, tilePos.y, GameScene::getInstance()->sharedGameStage->getGroundMap());
        m_DestinationChanged = true;
        
        if (placeMark)
        {
            GameScene::getInstance()->sharedGameStage->placePlayerMoveMark(groundMapPos);
        }
    }
}

void Player::stopMove()
{
    m_MoveDestinationMapPos = m_CurrentMapPosition;
    m_DestinationChanged = true;
    m_ForceToMove = false;
    GameScene::getInstance()->sharedGameStage->stopFollowPlayer();
}

const CCPoint& Player::getAttackableMapPosition()
{
    return m_CurrentMapPosition;
}

void Player::destroy()
{
    remove();
    
    if (GameScene::getInstance()->sharedGameStage->getWinningCondition()->killPlayer)
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showGameOverMenu();
    }
}


bool Player::isTouched(const cocos2d::CCPoint &touchPos)
{
    if (!failToAct())
    {
        // only detect touch on player when he is not actable (needs to be tapped to release)
        return false;
    }
    
    CCPoint pos = m_pAnimation->convertToWorldSpace(touchPos);
    CCPoint groundMapPos = GameScene::getInstance()->sharedGameStage->getGroundMap()->convertToNodeSpace(pos);
    CCPoint tileTouched = MapHelper::groundMapPosToTilePos(groundMapPos, GameScene::getInstance()->sharedGameStage->getGroundMap(), false);
    
    if (tileTouched.x == m_CurrentTilePosition.x && tileTouched.y == m_CurrentTilePosition.y)
    {
        return true;
    }

    return false;
    
    /*
    CCRect rect = m_pAnimation->getCurrentSprite()->boundingBox();
    
    return rect.containsPoint(touchPos);
    */
}

bool Player::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint touchPos = m_pAnimation->convertTouchToNodeSpace(pTouch);
    
    if (isTouched(touchPos))
    {
        m_TapStartPosition = touchPos;
        return true;
    }

    
    return false;
}



void Player::ccTouchEnded(CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    CCPoint touchPos = m_pAnimation->convertTouchToNodeSpace(pTouch);
    
    if (isTouched(touchPos))
    {
        updateStatusByTap();
    }
    
}


void Player::draw()
{
    if (m_IsSetToMove)
    {
        Function::drawSolidCircleOnMap(m_pAnimation->getPosition(), 
			m_pAnimation->getCurrentSprite()->getContentSize().width * 0.075f * m_pAnimation->getScale(), 
			CC_DEGREES_TO_RADIANS(0), 
			50, 
			ccc4f(0.0f, 1.0f, 0.1f, 0.25f));
    }


/*
    if (m_Selected)
    {
    }
*/
}

ENUM_MOVE_DIRECTION Player::getAttackDirection(ENUM_NPC_ANIMATION prevAnimation)
{
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();

    CCPoint tilePos = ccp((int)m_CurrentTilePosition.x, (int)m_CurrentTilePosition.y);
    
    switch (prevAnimation)
    {
        case ENUM_NPC_ANIMATION_STAND_UP:
            return ENUM_MOVE_DIRECTION_UP;
            
        case ENUM_NPC_ANIMATION_MOVING_UP:
            if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y-1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x - 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x + 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
        
        case ENUM_NPC_ANIMATION_STAND_DOWN:
            return ENUM_MOVE_DIRECTION_DOWN;
            
        case ENUM_NPC_ANIMATION_MOVING_DOWN:
            if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y+1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x - 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x + 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            
        case ENUM_NPC_ANIMATION_STAND_LEFT:
            return ENUM_MOVE_DIRECTION_LEFT;
            
        case ENUM_NPC_ANIMATION_MOVING_LEFT:
            if (pMap->getSequenceByTile(ccp(tilePos.x - 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y - 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y + 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            
        case ENUM_NPC_ANIMATION_STAND_RIGHT:
            return ENUM_MOVE_DIRECTION_RIGHT;
            
        case ENUM_NPC_ANIMATION_MOVING_RIGHT:
            if (pMap->getSequenceByTile(ccp(tilePos.x + 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y - 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y + 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            
        case ENUM_NPC_ANIMATION_ATTACK_BOTTOM:
            if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y + 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x + 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x -1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
        case ENUM_NPC_ANIMATION_ATTACK_RIGHTBOTTOM:
            if (pMap->getSequenceByTile(ccp(tilePos.x + 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y + 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y - 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
        case ENUM_NPC_ANIMATION_ATTACK_RIGHT:
            if (pMap->getSequenceByTile(ccp(tilePos.x + 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y - 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y + 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
        case ENUM_NPC_ANIMATION_ATTACK_TOPRIGHT:
            if (pMap->getSequenceByTile(ccp(tilePos.x + 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y - 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y + 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            

        case ENUM_NPC_ANIMATION_ATTACK_TOP:
            if (pMap->getSequenceByTile(ccp(tilePos.x - 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x + 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y - 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
        case ENUM_NPC_ANIMATION_ATTACK_TOPLEFT:
            if (pMap->getSequenceByTile(ccp(tilePos.x - 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y - 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y + 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            
        case ENUM_NPC_ANIMATION_ATTACK_LEFT:
            if (pMap->getSequenceByTile(ccp(tilePos.x - 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y + 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y - 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
            
        case ENUM_NPC_ANIMATION_ATTACK_LEFTBOTTOM:
            if (pMap->getSequenceByTile(ccp(tilePos.x - 1, tilePos.y)) > 0)
            {
                return ENUM_MOVE_DIRECTION_LEFT;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y + 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_DOWN;
            }
            else if (pMap->getSequenceByTile(ccp(tilePos.x, tilePos.y - 1)) > 0)
            {
                return ENUM_MOVE_DIRECTION_UP;
            }
            else
            {
                return ENUM_MOVE_DIRECTION_RIGHT;
            }
    }

 /*
    int currentTileIndex = pMap->getSequenceByTile(m_CurrentTilePosition);
    int nextTileIndex = 0;
    if (m_MovingToEndPosition)
    {
        nextTileIndex = currentTileIndex+1;
    }
    else
    {
        nextTileIndex = currentTileIndex-1;
    }
    
    CCPoint nextTile = pMap->getTileBySequence(nextTileIndex);
    
    if (nextTile.x == 0 && nextTile.y == 0)
    {
        return m_CurrentMoveDirection;
    }
    
    if (nextTile.x < (int)m_CurrentTilePosition.x)
    {
        return ENUM_MOVE_DIRECTION_LEFT;
    }
    else if (nextTile.x > (int)m_CurrentTilePosition.x)
    {
        return ENUM_MOVE_DIRECTION_RIGHT;
    }
    else if (nextTile.y < (int)m_CurrentTilePosition.y)
    {
        return ENUM_MOVE_DIRECTION_UP;
    }
    else if (nextTile.y > (int)m_CurrentTilePosition.y)
    {
        return ENUM_MOVE_DIRECTION_DOWN;
    }
    */
    return m_CurrentMoveDirection;
}


bool Player::updateDirection()
{
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    ENUM_MOVE_DIRECTION old = m_CurrentMoveDirection;

    bool directionChanged = false;
    
    if (m_DestinationChanged)
    {
        bool movingToEnd = pMap->isTargetAfterCurrentPostion(m_CurrentMapPosition, m_MoveDestinationMapPos);
        
        if ( (m_MovingToEndPosition && !movingToEnd) ||
             (!m_MovingToEndPosition && movingToEnd))
        {
            switch (m_CurrentMoveDirection)
            {
                case ENUM_MOVE_DIRECTION_DOWN:
                    m_CurrentMoveDirection = ENUM_MOVE_DIRECTION_UP;
                    break;
                case ENUM_MOVE_DIRECTION_UP:
                    m_CurrentMoveDirection = ENUM_MOVE_DIRECTION_DOWN;
                    break;
                case ENUM_MOVE_DIRECTION_LEFT:
                    m_CurrentMoveDirection = ENUM_MOVE_DIRECTION_RIGHT;
                    break;
                case ENUM_MOVE_DIRECTION_RIGHT:
                    m_CurrentMoveDirection = ENUM_MOVE_DIRECTION_LEFT;
                    break;
            }
            directionChanged = true;
        }
        m_MovingToEndPosition = movingToEnd;
        m_DestinationChanged = false;
    }

    Function::updateDirection(m_CurrentTilePosition, MapHelper::mapPosToTilePos(m_MoveDestinationMapPos, pMap), &m_CurrentMoveDirection);

    return directionChanged || m_CurrentMoveDirection != old;
}


bool Player::reachedDestination()
{
//    bool standing = isStanding();

    CCRect rect = CCRectMake(m_CurrentMapPosition.x - MapHelper::MAP_TILE_LENGTH/5, m_CurrentMapPosition.y - MapHelper::MAP_TILE_LENGTH/5, MapHelper::MAP_TILE_LENGTH * 0.4, MapHelper::MAP_TILE_LENGTH * 0.4);

    bool result = rect.containsPoint(m_MoveDestinationMapPos);
    
    if (!result)
    {
        CCPoint destTile = MapHelper::mapPosToTilePos(m_MoveDestinationMapPos, GameScene::getInstance()->sharedGameStage->getGroundMap());
        result = ((int)m_CurrentTilePosition.x == (int)destTile.x &&
                  (int)m_CurrentTilePosition.y == (int)destTile.y) &&
        (isStanding() || Function::isAroundTileCenter(m_CurrentTilePosition, m_CurrentMoveDirection)
         );
    }


    
    return result;
}

bool Player::isWithdrawing()
{
    CCPoint endPos = GameScene::getInstance()->sharedGameStage->getGroundMap()->getEndTilePosition();
    if ((int)m_CurrentTilePosition.x == endPos.x && (int)m_CurrentTilePosition.y == endPos.y)
    {
        return false; // already at the last
    }
    
    if (reachedDestination())
    {
        return false; // reaching destinated tile
    }
    
    return m_MovingToEndPosition;
}

void Player::tryToStand()
{
    ENUM_NPC_ANIMATION animation = m_pAnimation->getCurrentAnimationType();
    
    if (isPerformingSKill())
    {
        return;
    }
    
    if (isStanding())
    {
        return;
    }
    
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    int sequence = pMap->getSequenceByTile(ccp((int)m_CurrentTilePosition.x, (int)m_CurrentTilePosition.y));
    CCPoint tilePos = pMap->getTileBySequence(sequence-1);
    
    if (tilePos.x < (int)m_CurrentTilePosition.x)
    {
        animation = ENUM_NPC_ANIMATION_STAND_LEFT;
    }
    else if (tilePos.x > (int)m_CurrentTilePosition.x)
    {
        animation = ENUM_NPC_ANIMATION_STAND_RIGHT;
    }
    else if (tilePos.y < (int)m_CurrentTilePosition.y)
    {
        animation = ENUM_NPC_ANIMATION_STAND_UP;
    }
    else if (tilePos.y > (int)m_CurrentTilePosition.y)
    {
        animation = ENUM_NPC_ANIMATION_STAND_DOWN;
    }
    
    
    if (animation != ENUM_NPC_ANIMATION_NONE)
    {
        m_pAnimation->setAnimation(animation);
    }
}

bool Player::ableToAttack()
{
    return !isDead() &&
           !m_ForceToMove &&
           !isWithdrawing() &&
           !isPerformingSKill() &&
           !failToAct() &&
           !GameScene::getInstance()->sharedGameStage->isPaused();
}

void Player::completeSkillTrainingWithGems(int id, int cost)
{
    if(this->isSkillStartTraining(id) && this->getGems() >= cost){
        map<int, unsigned long>::iterator it = m_CurrentTrainingSkills.find(id);
        it->second -= this->getTrainingSkillStartTime(id);
        this->updateGems(-cost);
        
        char msg[256] = {0};
        sprintf(msg, "train_skill_%d_%d_with_gems", id, getSkillLevel(id));
        SystemHelper::logEvent(msg);
        
    }
}


void Player::updateTrainingSkills()
{
    map<int, unsigned long>::iterator it = m_CurrentTrainingSkills.begin();
    bool updateTree = false;
    while (it != m_CurrentTrainingSkills.end())
    {
        int id = it->first;
        unsigned long startTime = getTrainingSkillStartTime(id);
        
        unsigned long timeElapsed = SystemHelper::getCurrentTimeInSeconds() - startTime;
        
        if (timeElapsed < (unsigned long)Function::getSkillTrainingTime(id))
        {
            // not enough time
            it++;
            continue;
        }
        
        m_CurrentTrainingSkills.erase(it++);
        
        m_Skills[id]++;
        updateTree = true;
        save();
    }
    CCNode* P = this->getParent();    
    if(updateTree){
        if(this->getParent() == NULL){
            GameScene::getInstance()->sharedMainUI->getWorldMap()->getWorldmapGameMenu()->updateSkillTree();
        }else{
            GameScene::getInstance()->sharedMainUI->getStageUI()->UpdateSkillTreeMenu();
        }
    }
}

void Player::update(float dt)
{
    updateTrainingSkills();
    
    if (GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }
    
    m_TimeElapsedSinceLastUpdate = dt;
    
    if (!isActive())
    {
        return;
    }
    
    if (isDead())
    {
        destroy();
        return;
    }
    
    
    updateStatus(dt);

    if (failToAct())
    {
        return;
    }
    
    StageMap* pGroundMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    
    bool directionChanged = updateDirection();
    
    
    if (attackComponent->hasTarget())
    {
        if (m_ForceToMove || isWithdrawing())
        {
            finishAttack();
        }
        else if (!isPerformingSKill())
        {
            return;
        }
    }
    else
    {
        // recover when not attacking
        if (!isPerformingSKill())
        {
            updateMp(dt);
            updateHp(dt);
        }

    }
    
    if (isPerformingSKill())
    {
        if (m_PerformingSkillType == ENUM_PLAYER_PERFORM_SKILL_WHIRLWIND)
        {
            performWhirlWind(dt);
        }
        else
        {
            return;
        }
    }
    
    if (reachedDestination())
    {
        tryToStand();
        m_IsSetToMove = false;
        GameScene::getInstance()->sharedGameStage->hidePlayerMoveMark();
        return;
    }
    
    if (m_CurrentMoveDirection == ENUM_MOVE_DIRECTION_NONE)
    {
        return;
    }
    
    if (isStanding())
    {
        directionChanged = true;
    }
    
    if (!m_Movable)
    {
        return;
    }
    
    m_IsSetToMove = true;
    
    
    float timeElapsed = dt;
    CCPoint m_OldPosition = m_CurrentMapPosition;
    
    int moveAttempts = 0;

    while (moveAttempts < 5) {
        switch(m_CurrentMoveDirection)
        {
            case ENUM_MOVE_DIRECTION_DOWN:
                if (!directionChanged)
                {
                    m_CurrentMapPosition.y += m_Speed * timeElapsed;
                }
                else if (!isPerformingSKill())
                {
                    m_pAnimation->setAnimation(ENUM_NPC_ANIMATION_MOVING_DOWN);
                }
                break;
            case ENUM_MOVE_DIRECTION_RIGHT:
                if (!directionChanged)
                {
                    m_CurrentMapPosition.x += m_Speed * timeElapsed;
                }
                else if (!isPerformingSKill())
                {
                    m_pAnimation->setAnimation(ENUM_NPC_ANIMATION_MOVING_RIGHT);
                }
                
                break;
            case ENUM_MOVE_DIRECTION_LEFT:
                if (!directionChanged)
                {
                    m_CurrentMapPosition.x -= m_Speed * timeElapsed;
                }
                else if (!isPerformingSKill())
                {
                    m_pAnimation->setAnimation(ENUM_NPC_ANIMATION_MOVING_LEFT);
                }
                
                break;
            case ENUM_MOVE_DIRECTION_UP:
                if (!directionChanged)
                {
                    m_CurrentMapPosition.y -= m_Speed * timeElapsed;
                }
                else if (!isPerformingSKill())
                {
                    m_pAnimation->setAnimation(ENUM_NPC_ANIMATION_MOVING_UP);
                }
                
                break;
            default:
                return;
                
        }
        
        //    CCPoint newPos = MapHelper::mapPosToGroundPos(mapPosition.x, mapPosition.y, pGroundMap);
        CCPoint newPos = MapHelper::mapPosToGroundMapPos(m_CurrentMapPosition);
        CCPoint newTilePos = MapHelper::mapPosToTilePos(m_CurrentMapPosition, pGroundMap, true);
        
        if (pGroundMap->isRoadTile(newTilePos))
        {
            m_CurrentTilePosition = newTilePos;
            m_pAnimation->setPosition(newPos);
            break;
        }
        moveAttempts++;
        timeElapsed *= 0.7;
    }
    
    float orderZ = MapHelper::calculateOrderZ(m_CurrentTilePosition, pGroundMap);
    this->getParent()->reorderChild(this, orderZ);
    
    updateNearbyBuildableTiles();
}


void Player::updateNearbyBuildableTiles()
{
    StageMap* pGroundMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    // update nearby building tiles
    vector<CCPoint> nearbyBuidlingTiles = pGroundMap->getNeighborBuildingTiles(m_CurrentTilePosition, 3);
    
    unordered_set<int> newBuildingTiles;
    
	for (const CCPoint& tile : nearbyBuidlingTiles)
    {
        int mapIndex = pGroundMap->getMapIndex(tile);
        newBuildingTiles.insert(mapIndex);
        
        if (m_NearbyBuildingTiles.find(mapIndex) == m_NearbyBuildingTiles.end())
        {
            // add to map
            CCSprite* pMask = CCSprite::create("UI/buildarea_mark.png");
            CCPoint groundmapPos = MapHelper::tilePosToGroundMapPos(ccpAdd(tile, ccp(0.5f, 0.5f)));
            pMask->setPosition(groundmapPos);
            pGroundMap->addChild(pMask, 0);
            
            m_NearbyBuildingTiles[mapIndex] = pMask;
            
        }
    }
    
    // remove non-existing building tiles
    vector<int> tilesToRemove;
    auto buildingTileIt = m_NearbyBuildingTiles.begin();
    while (buildingTileIt != m_NearbyBuildingTiles.end())
    {
        int mapIndex = buildingTileIt->first;
        if (newBuildingTiles.find(mapIndex) == newBuildingTiles.end())
        {
            tilesToRemove.push_back(mapIndex);
        }
        
        buildingTileIt++;
    }
    
    for (const int & tileIndex : tilesToRemove)
    {
        auto it = m_NearbyBuildingTiles.find(tileIndex);
        it->second->removeFromParent();
        m_NearbyBuildingTiles.erase(it);
    }
}


bool Player::isBuildableTile(const cocos2d::CCPoint &tile)
{
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    int mapIndex = pMap->getMapIndex(tile);
    if (m_NearbyBuildingTiles.find(mapIndex) != m_NearbyBuildingTiles.end())
    {
        return true;
    }
    
    return false;
}

void Player::updateHp(float dt)
{
    if (getHp() >= getMaxHp())
    {
        return;
    }
    
    float recovered = dt * (float)getMaxHp()*0.01;
    float bonus = getSkillLevel(ENUM_SKILL_HP_REGENERATION)*0.1;
    
    float value = recovered * (1.0f+bonus);
    addHp(value);
}


void Player::addHp(float value)
{
    m_Hp += value;
    
    if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_GOING_TO_DIE) &&
        isInCriticalCondition())
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showTutorialInstruction(ENUM_FIRST_TIME_GOING_TO_DIE);
    }
    //    m_Hp = 95;
    
    if (m_Hp > m_MaxHp)
    {
        m_Hp = m_MaxHp;
    }
    else if (m_Hp < 0)
    {
        m_Hp = 0;
    }
    
    if (isInCriticalCondition() && !m_CriticalLayer)
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        m_CriticalLayer = CCLayerColor::create();
        CCSprite* mask = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/blood_mask.png"));
        mask->setPosition(ccp(winSize.width/2, winSize.height/2));
        mask->setColor(ccc3(255,0,0));
        mask->setScaleX(winSize.width/mask->getContentSize().width);
        mask->setScaleY(winSize.height/mask->getContentSize().height);
        mask->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(0.5, 128),
                                                                                                 CCFadeTo::create(0.5, 255),
                                                                                                 NULL)));
        m_CriticalLayer->addChild(mask);
        GameScene::getInstance()->sharedGameStage->getParent()->addChild(m_CriticalLayer);
        
        GameControlMenu* cm = GameScene::getInstance()->sharedMainUI->getStageUI()->getControlMenu();

    }
    else if (m_CriticalLayer && !isInCriticalCondition())
    {
        m_CriticalLayer->removeFromParentAndCleanup(true);
        m_CriticalLayer = NULL;
    }

}

void Player::updateMp(float dt)
{
    m_Mp += dt;
    
    if (m_Mp > m_MaxMp)
    {
        m_Mp = m_MaxMp;
    }
    if (m_Mp < 0)
    {
        m_Mp = 0;
    }

}

void Player::updateExperience(int delta)
{
    m_Experience += delta * (1.0 + 0.1 * getSkillLevel(ENUM_SKILL_BATTLE_EXPERIENCE));
    unsigned int nextLevelExp = getNextLevelExperience();
    
    if (nextLevelExp == 0)
    {
        return;
    }
    
    if (m_Experience > nextLevelExp)
    {
        float oldAttack = m_MeleeDamage;
        
        m_Experience = 0; // reset experience when leveling up
        m_Level++; // hahahaha :D

        float oldHp = m_MaxHp;
        m_MaxHp += 150 + 100 * (m_Level/5);
        m_MaxMp += 100;

        m_MeleeDamage += 10 + 10 * (m_Level/10);
        
        m_Hp = m_MaxHp;
        addHp(0);
        m_Mp = m_MaxMp;
        
        m_SkillPoints+=2; // add 2 points for each level up
        
        showLevelUpAnimation(m_MaxHp - oldHp, m_MeleeDamage-oldAttack);


        GameScene::getInstance()->sharedMainUI->getStageUI()->showInstructionForSkillTree();


        
        // check if there is any new weapon can be build
        MessageBox* pMessageBox = GameScene::getInstance()->sharedMainUI->getStageUI()->getMessageBox();
        
        int newSkillCount = 0;
        
        const map<int, BuildingSetting>& buildings = GameData::getBuildingSettings();
        map<int, BuildingSetting>::const_iterator it = buildings.begin();
        char buffer[256] = {0};
        
        while (it != buildings.end())
        {
            if (it->second.requireLevel == m_Level)
            {
                MessageIcon icon;
                
                sprintf(buffer, "UI/building/building%d_normal.png", it->second.id);
                icon.imagePath = buffer;
                icon.name = it->second.name;
                icon.description = it->second.introduction;

                pMessageBox->addMessageIcon(icon);
                newSkillCount++;

            }
            ++it;
        }
        

        const map<int, Skill*>& skills = GameData::getAllSkills();
        map<int, Skill*>::const_iterator skillIt = skills.begin();
        while (skillIt != skills.end())
        {
            if (skillIt->second->requireLevel == m_Level)
            {
                MessageIcon icon;
                
                sprintf(buffer, "UI/skilltree/skill%d_normal.png", skillIt->second->id);
                icon.imagePath = buffer;
                icon.name = skillIt->second->name + "\n"+ GameData::getText("needs_learning");
                icon.description = skillIt->second->introduction;

                
                pMessageBox->addMessageIcon(icon);
                
                newSkillCount++;
            }
            
            ++skillIt;
        }

        
//        pMessageBox->showMessageWithIcons(GameData::getText("level_up_hint"), icons, true);
        if (newSkillCount > 0 &&
            GameScene::getInstance()->sharedGameStage->isReady() &&
            !GameScene::getInstance()->sharedGameStage->isPaused())
        {
            pMessageBox->setMessage(GameData::getText("level_up_hint"));
            
            pMessageBox->showMessageIcons();
            
        }
        
        // save afer leveling up
        save();
    }
}

void Player::showLevelUpAnimation(float hpDiff, float attackDiff)
{
    
    //    PlayerItems& items = Player::getInstance()->playerItems();
    //    items.addItem((1 + rand()%5) * 10 + (1 + rand()% 4), 1);
    
    CCSprite * pSprite = m_pAnimation->getCurrentSprite();
    if (!pSprite)
    {
        return;
    }
    
    const float distance = pSprite->getTextureRect().size.height/5;
    
    CCPoint targetPos = ccp(0, pSprite->getTextureRect().size.height/3+distance);

    SmartAnimation* pLevelAnimation = SmartAnimation::create(1, true);
    
    CCString* text = CCString::createWithFormat(GameData::getText("level_up_level"), 1);
    
    CCLabelBMFont* message = CCLabelBMFont::create(text->getCString(), "font/levelup.fnt");
    message->setAnchorPoint(ccp(0.5,0));
    message->setPosition(ccp(0,targetPos.y - distance));
    
    pLevelAnimation->addChild(message);
    pLevelAnimation->setScale(1.5);
    
    m_pAnimation->addChild(pLevelAnimation);

    message->runAction((CCActionInterval*)(CCSequence::create(CCFadeIn::create(0.1f),
                                                              CCDelayTime::create(0.3f),
                                                              CCFadeOut::create(0.1f),
                                                              NULL)));
    pLevelAnimation->runAction((CCActionInterval*)(CCSequence::create(
                                                                 CCMoveTo::create(0.5f, targetPos),
                                                                 CCCallFunc::create(pLevelAnimation, callfunc_selector(SmartAnimation::removeSelf)),
                                                                 NULL)));
   
    
    SmartAnimation* pAttackValueAnimation = SmartAnimation::create(1, true);
    
    CCString* text2 = CCString::createWithFormat(GameData::getText("level_up_attack"), attackDiff);
    
    message = CCLabelBMFont::create(text2->getCString(), "font/levelup.fnt");
    message->setAnchorPoint(ccp(0.5,0));
    message->setPosition(ccp(0, targetPos.y - distance));
    
    pAttackValueAnimation->addChild(message);
    pAttackValueAnimation->setScale(1.5);
    
    m_pAnimation->addChild(pAttackValueAnimation);

    message->setOpacity(0);
    message->runAction((CCActionInterval*)(CCSequence::create(CCDelayTime::create(0.4f),
                                                              CCFadeIn::create(0.1f),
                                                              CCDelayTime::create(0.3f),
                                                              CCFadeOut::create(0.1f),
                                                              NULL)));
    pAttackValueAnimation->runAction((CCActionInterval*)(CCSequence::create(CCDelayTime::create(0.4f),
                                                                            CCMoveTo::create(0.5f, targetPos),
                                                                            CCCallFunc::create(pAttackValueAnimation, callfunc_selector(SmartAnimation::removeSelf)),
                                                                            NULL)));
    

    SmartAnimation* pHpValueAnimation = SmartAnimation::create(1, true);
    text2 = CCString::createWithFormat(GameData::getText("level_up_hp"), (int)hpDiff);
    
    message = CCLabelBMFont::create(text2->getCString(), "font/levelup.fnt");
    message->setAnchorPoint(ccp(0.5,0));
    message->setPosition(ccp(0, targetPos.y - distance));
    
    pHpValueAnimation->addChild(message);
    pHpValueAnimation->setScale(1.5);
    
    m_pAnimation->addChild(pHpValueAnimation);
    
    message->setOpacity(0);
    message->runAction((CCActionInterval*)(CCSequence::create(CCDelayTime::create(0.9f),
                                                              CCFadeIn::create(0.1f),
                                                              CCDelayTime::create(0.3f),
                                                              CCFadeOut::create(0.1f),
                                                              NULL)));
    pHpValueAnimation->runAction((CCActionInterval*)(CCSequence::create(CCDelayTime::create(0.9f),
                                                                        CCMoveTo::create(0.5f, targetPos),
                                                                        CCCallFunc::create(pHpValueAnimation, callfunc_selector(SmartAnimation::removeSelf)),
                                                                            NULL)));
    
    
}

void Player::addPointsToSkill(int skillId,bool cost)
{

}

bool Player::canUseBuilding(int buildingId)
{
    return GameData::getBuildingSetting(buildingId).requireLevel <= m_Level;
    
}

bool Player::canUseMagic(int skillId)
{
    return false;
//    return m_UsableMagic.find(skillId) != m_UsableMagic.end();
}


void Player::save()
{
    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    
    pUserDefault->setFloatForKey("game_version", GAME_VERSION);


    string completedMapValue = "";
    map<int, int>::iterator it = m_CompletedMapScores.begin();
    
    while(it!=m_CompletedMapScores.end())
    {
        char data[16] = {0};
        sprintf(data, "%d,%d", it->first, it->second);
        completedMapValue += data;
        completedMapValue += ",";
        ++it;
    }
 
    //pUserDefault->setStringForKey("completed_map", completedMapValue);

    string playerSkillValue = "";
    map<int, int>::iterator skillIt = m_Skills.begin();
    while(skillIt != m_Skills.end())
    {
        int skillId = skillIt->first;
        int level = skillIt->second;
        
        char data[16] = {0};
        sprintf(data, "%d,%d,", skillId, level);
        playerSkillValue += data;
        
        skillIt++;
    }

    string playerKilledEnemies = "";
    map<int, unsigned int>::iterator enemyIt = m_KilledEnemies.begin();
    while(enemyIt != m_KilledEnemies.end())
    {
        int id = enemyIt->first;
        int count = enemyIt->second;
        
        char data[32] = {0};
        sprintf(data, "%d,%d,", id, count);
        playerKilledEnemies += data;
        
        enemyIt++;
    }

    string playerPurchasedMap= "";
    map<int, int>::iterator purchasedMapIt = m_MapPurchased.begin();
    while(purchasedMapIt != m_MapPurchased.end())
    {
        int id = purchasedMapIt->first;
        int value = purchasedMapIt->second;
        
        char data[32] = {0};
        sprintf(data, "%d,%d,", id, value);
        playerPurchasedMap += data;
        
        purchasedMapIt++;
    }
    
    string playerTrainingSkills= "";
    map<int, unsigned long>::iterator trainingSkillIt = m_CurrentTrainingSkills.begin();
    while(trainingSkillIt != m_CurrentTrainingSkills.end())
    {
        int id = trainingSkillIt->first;
        unsigned long value = trainingSkillIt->second;
        
        char data[32] = {0};
        sprintf(data, "%d,%lu,", id, value);
        playerTrainingSkills += data;
        
        trainingSkillIt++;
    }
    
    const char* pszData1 = completedMapValue.c_str();
    const char* pszData2 = playerSkillValue.c_str();
    
    char buffer[4096] = {0};
    sprintf(buffer, "%d:%lu:%d:%lu:%d:%d:%d:%d:%f:%s:%s:%s:%s:%s:",
            m_Level,
            m_Experience,
            m_Money,
            m_Gems,
            m_SkillPoints,
            m_MaxHp,
            m_MaxMp,
            m_ProgressFlags,
            m_MeleeDamage,
            pszData1,
            pszData2,
            playerKilledEnemies.c_str(),
            playerPurchasedMap.c_str(),
            playerTrainingSkills.c_str());
    
    
    string encryptedData = SystemHelper::encryptWithAES(buffer);
    
    pUserDefault->setStringForKey("player_data", encryptedData);

    pUserDefault->flush();
}


void Player::updatePlayerData(size_t i, const string &value)
{
    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    
    string originalData = pUserDefault->getStringForKey("player_data");
    
    string decryptedData = SystemHelper::decryptWithAES(originalData.c_str());
    
    vector<string> values = Function::splitString(decryptedData, ":");
    
    if (values.size() < i + 1)
    {
        // not saved yet
        save();
        return;
    }
    
    values[i] = value.c_str();
    
    string finalValue;
	for (const string& value : values)
    {
        finalValue.append(value);
        finalValue.append(":");
    }
    
    string encryptedData = SystemHelper::encryptWithAES(finalValue.c_str());
    
    pUserDefault->setStringForKey("player_data", encryptedData);
    
    pUserDefault->flush();
}

void Player::saveGems()
{
    char buffer[128] = {0};
    sprintf(buffer, "%lu", m_Gems);
    
    updatePlayerData(3, buffer);
    
}

void Player::saveMoney()
{
    char buffer[128] = {0};
    sprintf(buffer, "%d", m_Money);
    
    updatePlayerData(2, buffer);

}

void Player::load()
{
    CCUserDefault* pUserDefault = CCUserDefault::sharedUserDefault();
    
    float gameVersion = pUserDefault->getFloatForKey("game_version");

    if (gameVersion == 0)
    {
        
        save();
        

    }
    else
    {
        string originalData = pUserDefault->getStringForKey("player_data");
        
        string decryptedData = SystemHelper::decryptWithAES(originalData.c_str());
        
        vector<string> values = Function::splitString(decryptedData, ":");

        if (values.size()>1)
        {
            size_t i = 0;
            m_Level = atoi(values[i++].c_str());
            m_Experience = atol(values[i++].c_str());
            
            m_Money = atol(values[i++].c_str());
            m_Gems = atol(values[i++].c_str());
            m_SkillPoints = atoi(values[i++].c_str());
            m_MaxHp = atoi(values[i++].c_str());
            m_MaxMp = atoi(values[i++].c_str());
            m_ProgressFlags = (unsigned int)atoi(values[i++].c_str());
            m_MeleeDamage = atoi(values[i++].c_str());
            
            m_Hp = m_MaxHp;
            m_Mp = m_MaxMp;
            
            m_CompletedMapScores.clear();
            
            if (i < values.size())
            {
                string& value = values[i++];
                vector<int> ids = Function::splitStringToIntegers(value, ",");
                vector<int>::iterator it = ids.begin();
                for (size_t j = 0; j < ids.size(); j += 2)
                {
                    setMapScore(ids[j], ids[j + 1]);
                    ++it;
                }
            }
            
            m_Skills.clear();
            if (i < values.size())
            {
                string& value = values[i++];
                vector<string> skills = Function::splitString(value, ",");
                for (size_t j = 0; j < skills.size(); j += 2)
                {
                    string s = skills[j];
                    int id = atoi(skills[j].c_str());
                    int level = atoi(skills[j + 1].c_str());
                    m_Skills[id] = level;
                }
            }
            
            m_KilledEnemies.clear();
            if (i<values.size())
            {
                string& value = values[i++];
                vector<string> data = Function::splitString(value, ",");
                for (size_t j = 0; j < data.size(); j += 2)
                {
                    int id = atoi(data[j].c_str());
                    int count = atoi(data[j + 1].c_str());
                    m_KilledEnemies[id] = count;
                }
            }

            m_MapPurchased.clear();
            if (i < values.size())
            {
                string& value = values[i++];
                vector<string> data = Function::splitString(value, ",");
                for (size_t j = 0; j < data.size(); j += 2)
                {
                    int id = atoi(data[j].c_str());
                    int count = atoi(data[j + 1].c_str());
                    m_MapPurchased[id] = count;
                }
            }

            m_CurrentTrainingSkills.clear();
            if (i < values.size())
            {
                string& value = values[i++];
                vector<string> data = Function::splitString(value, ",");
                for (size_t j = 0; j < data.size(); j += 2)
                {
                    int id = atoi(data[j].c_str());
                    unsigned long startTime = atol(data[j + 1].c_str());
                    m_CurrentTrainingSkills[id] = startTime;
                }
            }
        }
        
        if ((int)gameVersion*10 < (int)GAME_VERSION*10)
        {
            // update version
            save();
        }
        
    }
    
    reset();

}
