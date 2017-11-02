//
//  Enemy.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/13/12.
//
//

#include "Enemy.h"
#include "MapHelper.h"
#include "GameScene.h"
#include "Player.h"
#include "Explosion.h"
#include "EnemySetting.h"
#include "GameData.h"
#include "EnemyCache.h"
#include <queue>
#include "CCSmartSprite.h"
#include "AnimationManager.h"
#include "EnemyShootAttack.h"
#include "ComputerPlayer.h"
#include "SpiderQueenAttack.h"
#include "SmartAnimation.h"
#include "SimpleAudioEngine.h"
#include "MessageBox.h"
using namespace CocosDenshion;

//static std::queue<Enemy*> enemyQueue;


void Enemy::initCache()
{

}

Enemy::Enemy(int id)
{
    initialize(id, 0);
}


Enemy::Enemy(int id, int parentId)
{
    initialize(id, parentId);
}

void Enemy::initialize(int id, int parentId)
{
    this->id = id;
    
    m_pEnemySetting = GameData::getEnemySetting(id);
    if (m_pEnemySetting == NULL)
    {
        return;
    }
    
    maxHp = m_pEnemySetting->hp;
    attack = m_pEnemySetting->attack;
    attackRange = m_pEnemySetting->attackRange;
    moveSpeed = m_pEnemySetting->moveSpeed;
    
    m_KilledByPlayer = false;
    m_KilledInExplosion = false;
    
    m_IsBoss = false;
    
    pAnimation = EnemyAnimation::create(id, parentId);
    
    m_Level = 1;
    m_MoveToEnd = false;
    m_RandSeed = rand();
    
    m_Team = ENUM_TEAM_COMPUTER;
    m_pAttackComponent = NULL;
    // if there is no computer player, enemies become the opponents
    
    m_DestinationTile = GameScene::getInstance()->sharedGameStage->getGroundMap()->getEndTilePosition();
    
    this->addChild(pAnimation);
}


void Enemy::createAttackComponent()
{
    switch (id) {
        case 1:
            m_pAttackComponent = TurtleThrowStoneAttack::create();
            break;
        case 12:
            m_pAttackComponent = SkeletonArrowAttack::create();
            break;
        case 9:
            m_pAttackComponent = SpiderWebAttack::create();
            break;
        case 16:
            m_pAttackComponent = SnakeVenomAttack::create();
            break;
        case 17:
            m_pAttackComponent = SpiderQueenAttack::create();
            break;
        default:
            m_pAttackComponent = EnemyAttackComponent::create();
            break;
    }
}

void Enemy::init(int level)
{
    maxHp *= level;
    attack *= level;
    
    currentHp = maxHp;
    
    m_Level = level;
    
}

Enemy::~Enemy()
{
    
    if (pAnimation)
    {
        pAnimation->clear();
        pAnimation->removeFromParent();
        pAnimation = NULL;
    }
    
    if (m_pAttackComponent)
    {
        m_pAttackComponent->removeFromParent();
        m_pAttackComponent = NULL;
    }
    
}

Enemy* Enemy::create(int id, CCPoint startTilePos)
{
    Enemy* pEnemy = new Enemy(id);
    
    if (pEnemy)
    {
        pEnemy->autorelease();
    }
    
    pEnemy->pAnimation->reset();
    pEnemy->currentHp = pEnemy->maxHp;

    pEnemy->setToTile(startTilePos.x, startTilePos.y);

    return pEnemy;
}

Enemy* Enemy::create(int id, int parentId, CCPoint startTilePos)
{
    Enemy* pEnemy = new Enemy(id, parentId);
    
    if (pEnemy)
    {
        pEnemy->autorelease();
    }
    
    pEnemy->pAnimation->reset();
    pEnemy->currentHp = pEnemy->maxHp;
    
    pEnemy->setToTile(startTilePos.x, startTilePos.y);
    
    return pEnemy;
}

int Enemy::getExperience()
{
 
    int regularExp = m_pEnemySetting->baseExp;

    return regularExp * m_Level;
}

int Enemy::getDropMoney()
{
    float money =  m_Level * m_pEnemySetting->dropMoney;
    money *= 1.0 + (float)Player::getInstance()->getSkillLevel(ENUM_SKILL_LOOT_MONEY) * 0.1;
    
    return money;
}

int Enemy::getDropGems()
{

    return 0;
    
    /*
    int dropGems = GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId())->maxDiamondsAward * (0.5 + (float)(rand()%50)/100);
    dropGems *= 1 + rand()%m_Level;
    return dropGems;
    */
}

float Enemy::getDamage()
{
    return attack;
}

float Enemy::getAttackRange()
{
    return attackRange;
}

float Enemy::getAttackRate()
{
    return attackRate;
}

void Enemy::reduceHp(float damage, bool ignoreDefense)
{
    currentHp -= damage;
}

bool Enemy::isDead()
{
    return currentHp <= 0;
}

bool Enemy::ableToAttack()
{
    return isActive() &&
           !isDead() &&
           !failToAct() &&
           GameScene::getInstance()->sharedGameStage->isReady() &&
           !GameScene::getInstance()->sharedGameStage->isPaused();

}


float Enemy::getBaseSpeed()
{
    return m_pEnemySetting->moveSpeed;
}

void Enemy::setSpeed(float speed)
{
    moveSpeed = speed;
}


void Enemy::updateDestination()
{
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();

    if (m_IsBoss)
    {
        // boss won't move beyond half of the road, so player has some space
        if ((int)m_CurrentTilePosition.x == m_DestinationTile.x && (int)m_CurrentTilePosition.y == m_DestinationTile.y)
        {
            int endSequence = pMap->getSequenceByTile(pMap->getEndTilePosition());
            
            m_DestinationTile = pMap->getTileBySequence(rand()%(endSequence / 2) + 2);
        }
        
    }
    else if (GameScene::getInstance()->sharedGameStage->getWinningCondition()->killPlayer)
    {
        m_DestinationTile = Player::getInstance()->getCurrentMapTile();
    }
}

bool Enemy::updateDirection()
{
    /*
    bool result = Function::updateDirection(m_CurrentTilePosition, destination, &m_CurrentMoveDirection);
    
    
    return result;
     */
    
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    updateDestination();
    
    
    if (pMap->getSequenceByTile(m_DestinationTile) == pMap->getSequenceByTile(m_CurrentTilePosition))
    {
        return false;
    }
    
    bool moveToEnd = pMap->getSequenceByTile(m_DestinationTile) > pMap->getSequenceByTile(m_CurrentTilePosition);
    
    bool directionChanged = moveToEnd != m_MoveToEnd;
    m_MoveToEnd = moveToEnd;
    
    if (directionChanged)
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
    }
    
    bool result = Function::updateDirection(m_CurrentTilePosition, m_DestinationTile, &m_CurrentMoveDirection);
    
    return directionChanged || result;

}

void Enemy::setToTile(int x, int y)
{
    CCTMXTiledMap* pGroundMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    float dx = 0.25 + (rand()%500)/1000.0f;
    float dy = 0.25 + (rand()%500)/1000.0f;

    CCPoint tilePos = ccp(x+dx, y+dy);
    
    CCPoint groundMapPos = MapHelper::tilePosToGroundMapPos(tilePos);
    m_CurrentMapPosition = MapHelper::tilePosToMapPos(tilePos.x, tilePos.y, pGroundMap);
    m_CurrentTilePosition = MapHelper::mapPosToTilePos(m_CurrentMapPosition, pGroundMap, true); 
    pAnimation->setPosition(groundMapPos);
//    sprite->setPosition(groundMapPos);
}

const CCPoint& Enemy::getSpritePosition()
{
//    CCSprite* sprite = pAnimation->getCurrentSprite();
    return pAnimation ? pAnimation->getPosition() : CCPointZero;
}

CCPoint Enemy::getShootPosition()
{
    return getSpritePosition();
}

CCPoint Enemy::getAttackablePosition()
{
    return getSpritePosition();
}


CCNode* Enemy::getAnimationPart()
{
    return getAnimation();
}

void Enemy::remove()
{

    CCNode* parent = this->getParent();
    if (!parent)
    {
        return;
    }
    

    this->removeFromParentAndCleanup(true);

    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    this->removeFromParentAndCleanup(true);
    pStage->removeEnemy(this);

    this->active = false;
    this->unscheduleUpdate();
//    enemyQueue.push(this);
//    EnemyCache::put(this);
//    this->release();
}

void Enemy::destroy()
{
    if (!isActive())
    {
        return;
    }
    
    pAnimation->removeChildByTag(DIZZY_ANIMATION_TAG, true);
    pAnimation->removeChildByTag(SPIDERWEB_ANIMATION_TAG, true);
    
    char buffer[64] = {0};
    sprintf(buffer, "sound/dead%d.wav", 1+rand()%4);
    SimpleAudioEngine::sharedEngine()->playEffect(buffer);
    
    pAnimation->removeEffect();
    
    float deadBodyExistingTime = 3.0f;
    
    active = false;
    
    int dropMoney = getDropMoney();
    int dropGems = 0;
    
    if (m_IsBoss)
    {
        dropGems = getDropGems();
    }
    
    
    if (getCurrentTeam() == ENUM_TEAM_PLAYER)
    {
        dropMoney *= 2;
        dropGems *= 2;
    }
    
    //    Explosion::create("explosion/exp2.png",this->getAttackablePosition());
    showLootAnimation(dropMoney, dropGems);
    //    setVisible(false);
    //    pAnimation->setVisible(false);
    //    pAnimation->reset();
    ENUM_NPC_ANIMATION deadType = ENUM_NPC_ANIMATION_NONE;
    
    
    switch (m_CurrentMoveDirection)
    {
        case ENUM_MOVE_DIRECTION_UP:
            deadType = ENUM_NPC_ANIMATION_DEAD_UP;
            break;
        case ENUM_MOVE_DIRECTION_DOWN:
            deadType = ENUM_NPC_ANIMATION_DEAD_DOWN;
            break;
        case ENUM_MOVE_DIRECTION_LEFT:
            deadType = ENUM_NPC_ANIMATION_DEAD_LEFT;
            break;
        case ENUM_MOVE_DIRECTION_RIGHT:
            deadType = ENUM_NPC_ANIMATION_DEAD_RIGHT;
            break;
    }

    if (getAnimationSprite())
    {
        getAnimationSprite()->stopAllActions();
        getAnimationSprite()->setVisible(false);
    }
    
    if (deadType != ENUM_NPC_ANIMATION_NONE)
    {
        setAnimation(deadType);
    }
    else
    {
        setAnimation(ENUM_NPC_ANIMATION_DEAD_UP);
    }
    
    if (getAnimationSprite() == NULL)
    {
        // try using the 1st type
        setAnimation(ENUM_NPC_ANIMATION_DEAD_UP);
    }

    if (getAnimationSprite())
    {

        if (!m_KilledInExplosion)
        {
            if (rand()%3 == 0)
            {
                
                getAnimationSprite()->runAction(CCSequence::create(CCDelayTime::create(deadBodyExistingTime/2),
                                                                   CCFadeOut::create(deadBodyExistingTime/2),
                                                                   NULL));
                
                CCSmartSprite* blood = CCSmartSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("blood/blood1.png"));
                blood->setPosition(getAttackablePosition());
                GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(blood);
                blood->setScale(0.1f);
                blood->setVisible(false);
                // by default all action animations should be less than 1 seconds
                blood->runAction(CCSequence::create(CCDelayTime::create(0.5),
                                                    CCCallFunc::create(blood, callfunc_selector(CCSmartSprite::makeVisible)),
                                                    CCScaleTo::create(deadBodyExistingTime / 4, 0.8f),
                                                    CCFadeTo::create(0.1f, 160.0f),
                                                    CCScaleTo::create(deadBodyExistingTime * 4, 0.3f),
                                                    CCFadeOut::create(deadBodyExistingTime / 2),
                                                    CCCallFunc::create(blood, callfunc_selector(CCSmartSprite::removeSelf)),
                                                    NULL));
            }
            else
            {
                showBodyDestruction();
            }
            
        }
        else
        {
            
            //throw body to air from a random angle
            StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
            
            int currentTileIndex = pMap->getSequenceByTile(m_CurrentTilePosition);
            
            vector<CCPoint> dropTiles;
            
            for(int i=-1; i <= 1; ++i)
            {
                CCPoint tile = pMap->getTileBySequence(currentTileIndex + i);
                
                if (pMap->isRoadTile(tile))
                {
                    dropTiles.push_back(tile);
                }
            }
            
            if (dropTiles.size()>0)
            {
                CCPoint dropTile = dropTiles[rand()%dropTiles.size()];
                float dropOffsetX = (float)(rand()%10) / 10.0f;
                float dropOffsetY = (float)(rand()%10) / 10.0f;
                CCPoint dropMapPos = MapHelper::tilePosToGroundMapPos(ccp(dropTile.x + dropOffsetX, dropTile.y + dropOffsetY));
                
                float degree = (45.0 + rand()%90) * 3.14159 / 180.0;
                float height = pMap->getTileSize().height * sin(degree);
                
                pAnimation->runAction(CCSequence::create(CCJumpTo::create(1, dropMapPos, height, 1),
                                                         CCCallFunc::create(this, callfunc_selector(Enemy::showBodyDestruction)),
                                                         NULL));
            }
            else
            {
                showBodyDestruction();
            }
        }
 
    }
    else
    {
        deadBodyExistingTime = 0; // remove immediately
    }

    
    
    if (getCurrentTeam() == ENUM_TEAM_COMPUTER)
    {
        CCPoint playerTilePos = Player::getInstance()->getCurrentMapTile();
        if (abs((int)(playerTilePos.x - (int)m_CurrentTilePosition.x)) + abs((int)(playerTilePos.y - (int)m_CurrentTilePosition.y)) < 3)
        {
            Player::getInstance()->updateExperience(getExperience());
        }
        else
        {
            Player::getInstance()->updateExperience(getExperience()/2);
        }
        
        Player::getInstance()->updateMoney(dropMoney);
        
        if (dropGems > 0)
        {
            Player::getInstance()->updateGems(dropGems);
        }
        
        unsigned int score = getMaxHp();
        
        GameScene::getInstance()->sharedGameStage->addMonsterKillScore(score);
        
    }
    else if (getCurrentTeam() == ENUM_TEAM_PLAYER && GameScene::getInstance()->sharedGameStage->getComputerPlayer())
    {
        ComputerPlayer* pComputer = static_cast<ComputerPlayer*>(GameScene::getInstance()->sharedGameStage->getComputerPlayer());
        CCPoint computerTilePos = pComputer->getCurrentTilePosition();
        
        if (abs((int)(computerTilePos.x - (int)m_CurrentTilePosition.x)) + abs((int)(computerTilePos.y - (int)m_CurrentTilePosition.y)) < 3)
        {
            pComputer->updateExperience(getExperience());
        }
        
        pComputer->updateMoney(dropMoney);
        
        if (dropGems > 0)
        {
            pComputer->updateGems(dropGems);
        }
    }
    
    scheduleOnce(schedule_selector(Enemy::remove), deadBodyExistingTime);
    
    if (m_pEnemySetting->childId != 0)
    {
        // create 10 smaller spiders
        for (int i=0; i<10; ++i)
        {
            Enemy* enemy = Enemy::create(m_pEnemySetting->childId, getId(), m_CurrentTilePosition);
            
            enemy->init(m_Level);
            
            GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(enemy);
            GameScene::getInstance()->sharedGameStage->enemies->addObject(enemy);

            enemy->m_CurrentMoveDirection = m_CurrentMoveDirection;
            enemy->m_MoveToEnd = m_MoveToEnd;
            enemy->activate();
        }
    }
}


void Enemy::showBodyDestruction()
{
    float deadBodyExistingTime = 3.0f;
    
    createDestructionAnimation(getAnimationSprite());
    
    CCSmartSprite* blood = CCSmartSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("blood/blood2.png"));
    blood->setPosition(getAttackablePosition());
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(blood);
    blood->setScale(0.1f);
    blood->runAction(CCSequence::create(CCScaleTo::create(0.2f, 1.0f),
                                        CCFadeOut::create(deadBodyExistingTime * 2.0f),
                                        CCCallFunc::create(blood, callfunc_selector(CCSmartSprite::removeSelf)),
                                        NULL));

}

void Enemy::showLootAnimation(int dropMoney, int dropGems)
{
    
//    PlayerItems& items = Player::getInstance()->playerItems();
//    items.addItem((1 + rand()%5) * 10 + (1 + rand()% 4), 1);
    
    // don't show for player side since it's looted by computer
    if (getCurrentTeam() == ENUM_TEAM_PLAYER)
    {
        return;
    }
    
    char buffer[64] = {0};
    
//    sprintf(buffer, "sound/coin%d.wav", m_IsKilledByPlayer ? 1 : 2);
    SimpleAudioEngine::sharedEngine()->playEffect("sound/coin2.wav", false);

    const float distance = 75.0f;
    const float totalTime = 0.75f;

    CCPoint targetPos = ccp(0, distance);
    
    CCSprite* moneyImage = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("gold.png"));


    sprintf(buffer, " +%d", m_KilledByPlayer ? 2 * dropMoney : dropMoney);
    CCLabelBMFont* message = CCLabelBMFont::create(buffer, "font/marker_felt_24.fnt");
    message->setAnchorPoint(ccp(0,0.5));
    message->setPosition(ccp(moneyImage->getContentSize().width, moneyImage->getContentSize().height/2));
    moneyImage->addChild(message);
    
    SmartAnimation* pMoneyAnimation = SmartAnimation::create(1, true);
    pMoneyAnimation->addChild(moneyImage);
    
    
    pAnimation->addChild(pMoneyAnimation);
    pMoneyAnimation->runAction((CCActionInterval*)(CCSequence::create(
                                                                      CCMoveTo::create(0.5, targetPos),
                                                                      CCCallFunc::create(pMoneyAnimation, callfunc_selector(SmartAnimation::removeSelf)),
                                                                      NULL)));

    if (dropGems > 0)
    {
        CCSprite* gemImage = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/gem.png"));
        
        sprintf(buffer, " +%d", dropGems);
        CCLabelBMFont* message = CCLabelBMFont::create(buffer, "font/marker_felt_24.fnt");
        message->setAnchorPoint(ccp(0,0.5));
        message->setPosition(ccp(gemImage->getContentSize().width, gemImage->getContentSize().height/2));
        gemImage->addChild(message);
        
        SmartAnimation* pGemAnimation = SmartAnimation::create(1, true);
        pGemAnimation->addChild(gemImage);
        
        gemImage->setOpacity(0);
        pAnimation->addChild(pGemAnimation);
        pGemAnimation->runAction((CCActionInterval*)(CCSequence::create(CCDelayTime::create(0.5),
                                                                        CCMoveTo::create(0.5, targetPos),
                                                                        CCCallFunc::create(pGemAnimation, callfunc_selector(SmartAnimation::removeSelf)),
                                                                        NULL)));
        gemImage->runAction((CCActionInterval*)(CCSequence::create(CCDelayTime::create(0.5),
                                                                   CCFadeTo::create(0, 255),
                                                                   NULL)));

    }
  
}


void Enemy::arriveDestination()
{

    
    const MapWinningCondition* pWinningCondition = GameScene::getInstance()->sharedGameStage->getWinningCondition();
    
    if (pWinningCondition->killPlayer)
    {
        Player* player = Player::getInstance();
        if (!player->isDead() && m_Team != ENUM_TEAM_PLAYER)
        {
            m_pAttackComponent->trySetTarget(player);
        }
    }
    
}

bool Enemy::isActive()
{
    return active && getParent() != NULL;
}


void Enemy::setStatus(ENUM_BUFF_STATUS status, float value, float duration)
{
    if (m_pEnemySetting->type == ENUM_ENEMY_TYPE_BOSS)
    {
        return;
    }
    
    AttackableObject::setStatus(status, value, duration);
}


void Enemy::finishAttack()
{
    m_pAttackComponent->removeTarget();
    if (isDead() || !isActive())
    {
        return;
    }
    
    resetIdleAnimation();
    
}

void Enemy::resetIdleAnimation()
{
    if (!isActive() || isDead())
    {
        return;
    }
    
    switch(m_CurrentMoveDirection)
    {
        case ENUM_MOVE_DIRECTION_DOWN:
            setAnimation(ENUM_NPC_ANIMATION_MOVING_DOWN);
            break;
        case ENUM_MOVE_DIRECTION_RIGHT:
            setAnimation(ENUM_NPC_ANIMATION_MOVING_RIGHT);
            break;
        case ENUM_MOVE_DIRECTION_LEFT:
            setAnimation(ENUM_NPC_ANIMATION_MOVING_LEFT);
            break;
        case ENUM_MOVE_DIRECTION_UP:
            setAnimation(ENUM_NPC_ANIMATION_MOVING_UP);
            break;
        default:
            return;
            
    }

}

void Enemy::activate()
{
    active = true;
    
    createAttackComponent();
    
    m_pAttackComponent->setTargetType(ATTACK_TARGET_ENEMY);
    this->addChild(m_pAttackComponent);

    
    if (m_IsBoss || this == GameScene::getInstance()->sharedGameStage->getComputerPlayer())
    {
        StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
        int endSequence = pMap->getSequenceByTile(pMap->getEndTilePosition());
        m_DestinationTile = pMap->getTileBySequence(rand()%(endSequence / 2) + 2);

        pAnimation->setupBossAnimation();
    }
    
    setVisible(true);
    pAnimation->setVisible(true);
    
    m_pAttackComponent->scheduleUpdate();
//    pAnimation->scheduleUpdate();
    this->scheduleUpdate();
    
    if (currentHp < getMaxHp() || isDead())
    {
        CCLog("shouldn't happen");
    }
    
    if (GameData::getEnemySetting(getId())->introduction.length()>0 && Player::getInstance()->getEnemiesKillCount(getId()) == 0)
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->addInfoButton(getId());
    }
    
    Player::getInstance()->addKilledEnemies(getId());
    

}


void Enemy::actWhenAttacked(AttackableObject *pTarget)
{
    if (isDead())
    {
        return;
    }
    
    if (m_pAttackComponent && !m_pAttackComponent->trySetTarget(pTarget))
    {
        return;
    }
    
    
}

const CCPoint& Enemy::getAttackableMapPosition()
{
    return m_CurrentMapPosition;
}

bool Enemy::reachDestination()
{
    CCRect rect = CCRectMake(m_CurrentMapPosition.x - MapHelper::MAP_TILE_LENGTH/5, m_CurrentMapPosition.y - MapHelper::MAP_TILE_LENGTH/5, MapHelper::MAP_TILE_LENGTH * 0.4, MapHelper::MAP_TILE_LENGTH * 0.4);
    CCPoint destMapPos = MapHelper::tilePosToMapPos(m_DestinationTile.x + 0.5, m_DestinationTile.y+0.5, GameScene::getInstance()->sharedGameStage->getGroundMap());
    bool result = rect.containsPoint(destMapPos);
    
    if (!result)
    {
        CCPoint destTile = m_DestinationTile;
        result = ((int)m_CurrentTilePosition.x == (int)destTile.x &&
                  (int)m_CurrentTilePosition.y == (int)destTile.y) &&
        Function::isAroundTileCenter(m_CurrentTilePosition, m_CurrentMoveDirection);
    }
    

    
    return result;

}


void Enemy::update(float dt)
{
    
    if (GameScene::getInstance()->sharedGameStage->isPaused() ||
        !GameScene::getInstance()->sharedGameStage->isReady())
    {
        return;
    }

    if (isDead())
    {
        destroy();
        return;
    }
    
    if (!active)
    {
        return;
    }
    
    
    if (!GameScene::getInstance()->sharedGameStage->getGroundMap()->isRoadTile(m_CurrentTilePosition))
    {
        remove();
        return;
    }
    
    updateStatus(dt);
    
    if (failToAct())
    {
        return;
    }
    

    if (m_pAttackComponent->hasTarget())
    {
        return;
    }

    
    
    StageMap* pGroundMap = GameScene::getInstance()->sharedGameStage->getGroundMap();

    
    bool directionChanged = updateDirection();
    
    CCPoint playerTilePos = Player::getInstance()->getCurrentMapTile();
    
    const MapWinningCondition* pWinningCondition = GameScene::getInstance()->sharedGameStage->getWinningCondition();
    
    if (!directionChanged)
    {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
        if (pWinningCondition->killPlayer &&
            m_pAttackComponent->isWithinAttackRange(Player::getInstance()->getCurrentMapPosition()) &&
            !Player::getInstance()->isDead() &&
            m_Team != ENUM_TEAM_PLAYER)
        {
            trySetAttackTarget(Player::getInstance());
            return;
        }
        
        if (pWinningCondition->maxPassEnemies > 0)
        {
            CCPoint endTilePos = pGroundMap->getEndTilePosition();
            if ((int)m_CurrentTilePosition.x == endTilePos.x &&
                (int)m_CurrentTilePosition.y == endTilePos.y &&
                Function::isAroundTileCenter(m_CurrentTilePosition, m_CurrentMoveDirection))
            {
                active = false;
                remove();
                GameScene::getInstance()->sharedGameStage->updatePassedEnemies(1);
                return;
            }
        }
        
        if (reachDestination())
        {
            return;
        }
    }
    

    float timeElapsed = dt;
    
    switch(m_CurrentMoveDirection)
    {
        case ENUM_MOVE_DIRECTION_DOWN:
            if (!directionChanged)
            {
                m_CurrentMapPosition.y += moveSpeed * timeElapsed;
            }
            else
            {
                setAnimation(ENUM_NPC_ANIMATION_MOVING_DOWN);
            }
            break;
        case ENUM_MOVE_DIRECTION_RIGHT:
            if (!directionChanged)
            {
                m_CurrentMapPosition.x += moveSpeed * timeElapsed;
            }
            else
            {
                setAnimation(ENUM_NPC_ANIMATION_MOVING_RIGHT);
            }
            break;
        case ENUM_MOVE_DIRECTION_LEFT:
            if (!directionChanged)
            {
                m_CurrentMapPosition.x -= moveSpeed * timeElapsed;
            }
            else
            {
                setAnimation(ENUM_NPC_ANIMATION_MOVING_LEFT);
            }
            break;
        case ENUM_MOVE_DIRECTION_UP:
            if (!directionChanged)
            {
                m_CurrentMapPosition.y -= moveSpeed * timeElapsed;
            }
            else
            {
                setAnimation(ENUM_NPC_ANIMATION_MOVING_UP);
            }
            break;
        default:
            return;
            
    }

//    CCPoint newPos = MapHelper::mapPosToGroundPos(mapPosition.x, mapPosition.y, pGroundMap);
    CCPoint newPos = MapHelper::mapPosToGroundMapPos(m_CurrentMapPosition);
    CCPoint newTilePos = MapHelper::mapPosToTilePos(m_CurrentMapPosition, pGroundMap, true);
    m_CurrentTilePosition = newTilePos;
    
    // update opacity
    CCTMXLayer* pShadowLayer = GameScene::getInstance()->sharedGameStage->getGroundMap()->getShadowLayer();
    if (pShadowLayer)
    {
        if (GameScene::getInstance()->sharedGameStage->getGroundMap()->isTileValid(newTilePos))
            {
                CCSprite* shadowSprite =  pShadowLayer->tileAt(ccp((int)newTilePos.x, (int)newTilePos.y));
                if (shadowSprite)
                {
                    int opacity = 255 - shadowSprite->getOpacity();
                    CCSprite* pSprite = pAnimation->getCurrentSprite();
                    if (pSprite && opacity != pSprite->getOpacity())
                    {
                        
                        pSprite->setOpacity(opacity);
                    }
                }
                
            }
    }

    
    pAnimation->setPosition(newPos);
    float orderZ = MapHelper::calculateOrderZ(m_CurrentTilePosition, pGroundMap);
    pGroundMap->reorderChild(this, orderZ);
//    pAnimation->getParent()->reorderChild(pAnimation, orderZ);
//    CCLog("order z: %f", pAnimation->getZOrder());
    /*
    if (attackComponent)
    {
        attackComponent->update(dt);
    }
     */

}



void Enemy::draw()
{
    CCNode::draw();
    
    if (!active || GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }
    
    CCSprite* pSprite = pAnimation ? pAnimation->getCurrentSprite() : NULL;
    
    if (!pSprite)
    {
        return;
    }
    
    if (getCurrentHp() == getMaxHp())
    {
        return;
    }
    
    const float spriteScale = pSprite->getScale();
    CCSize size = pSprite->getTextureRect().size;

    size.width *=spriteScale;
    size.height *=spriteScale;

    size.width *= 0.5;

    int opacity = pSprite->getOpacity();
    if (opacity < 64)
    {
        // too dark to draw HP bar
        return;
    }
    
    CCPoint pos = pAnimation->getPosition();
    glLineWidth( 5.0f );
    CCPoint startPos = CCPointMake(pos.x - size.width/2, pos.y - size.height * pSprite->getAnchorPoint().y);

    ccDrawColor4B(255,0,0,255);
    ccDrawLine( startPos, CCPointMake(startPos.x+size.width, startPos.y) );

    ccDrawColor4B(128,255,128,255);
    ccDrawLine( startPos, CCPointMake(startPos.x+size.width * currentHp/maxHp, startPos.y) );
    
    // restore line width
    glLineWidth(1.0f);

}
