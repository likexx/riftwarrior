//
//  Building.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/15/12.
//
//

#include "Building.h"
#include "GameStage.h"
#include "GameScene.h"
#include "GameData.h"
#include "MapHelper.h"
#include "SimpleAudioEngine.h"
#include "GameData.h"
#include "MultiTileBuilding.h"
#include "BuildingUpgradeMenu.h"
#include "BuildingShootComponent.h"
#include "BuildingMudAttackComponent.h"
#include "BuildingPoisonAttackComponent.h"
#include "BuildingMultiArrowAttackComponent.h"
#include "BuildingShotgunComponent.h"
#include "BuildingMachineGunComponent.h"
#include "BuildingRocketAttackComponent.h"
#include "BuildingBase.h"
#include "BuildingStatusBar.h"
#include "ComputerPlayer.h"
#include "Player.h"
#include "CollectableMoney.h"
#include "Explosion.h"
#include "AnimationManager.h"

#define EXTRA_ANIMATION_TAG 2001

Building* Building::create(int id, int x, int y, bool unplaced)
{
    const BuildingSetting& setting = GameData::getBuildingSetting(id);
    
    Building* building = NULL;


    if (setting.tileWidth == 1 && setting.tileLength == setting.tileWidth)
    {
        building = new Building(id, unplaced);
    }
    else
    {
        building = new MultiTileBuilding(id, unplaced, setting.tileWidth, setting.tileLength);
    }
    building->autorelease();
    
    building->init(x, y, unplaced);
    
    return building;
}

Building::Building(int id, bool unplaced)
	: m_pBaseSprite(nullptr),
	m_pRotatingSprite(nullptr),
	m_pBuildingMenu(nullptr),
	selected(false),
	active(false),
	m_Unplaceable(false),
	timeElasped(0.0f)
{
    this->id = id;
    tileWidth = 1;
    tileLength = 1;
    m_Level = 0;
    
    m_RepairTimes = 1; // can be repaired only once
    
    this->setAnchorPoint(CCPointZero);
    m_pAttackComponent = NULL;
    m_pBuildingMenu = NULL;
    m_pSetting = &GameData::getBuildingSetting(id);
    m_LevelLabel = NULL;
    m_pBaseSprite = NULL;
    m_pRotatingSprite = NULL;
}

bool Building::init(int x, int y, bool unplaced)
{
    
    moveToTile(ccp(x, y));
    
    initFromSetting();

    initBuilding();
    

    
//    attackRange /= CC_CONTENT_SCALE_FACTOR();


    this->scheduleUpdate();
    return true;
}

void Building::initBuilding()
{
    char buffer[128] = {0};
    sprintf(buffer, "building_%d_base.png", id);
    CCSpriteFrame* pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
    
    if (pSpriteFrame )
    {
        this->m_pBaseSprite = CCSprite::createWithSpriteFrame(pSpriteFrame);
    }
    else
    {
        if (m_pSetting->baseSprite.length()>0)
        {
            sprintf(buffer, "base_image/%s.png", m_pSetting->baseSprite.c_str());
            this->m_pBaseSprite = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage(buffer));
        }
    }
    
    sprintf(buffer, "building_%d_idle_1_0.png",id);
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
    if (!frame)
    {
        sprintf(buffer, "building_%d_1_0.png",id);
    }
    
    if (id != 0)
    {
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
        if (frame)
        {
            m_pRotatingSprite = CCSprite::createWithSpriteFrame(frame);
        }
        else
        {
            m_pRotatingSprite = CCSprite::create();
            m_pRotatingSprite->setVisible(false);
            
        }

        // for regular buiding
        // opaque before being activated
        if (m_pBaseSprite)
        {
            m_pBaseSprite->setOpacity(128);
        }
        m_pRotatingSprite->setOpacity(128);
        

        updateScale();
        
        CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
        CCSize tileSize = pMap->getTileSize();

        m_pRotatingSprite->setAnchorPoint(ccp(m_pSetting->anchorX, m_pSetting->anchorY));

        if (m_pBaseSprite)
        {
            CCSize size= m_pBaseSprite->getContentSize();
            m_pRotatingSprite->setPosition(ccp(size.width/2, size.height/2));

            m_pBaseSprite->addChild(m_pRotatingSprite);
            m_pBaseSprite->setAnchorPoint(ccp(m_pSetting->baseAnchorX, m_pSetting->baseAnchorY)); // base anchor point is always fixed to match the tile center
            
            m_pBaseSprite->setPosition(ccp(0,-tileSize.height/2));
        }
        else
        {
            m_pRotatingSprite->setPosition(ccp(0, - tileSize.height/2));
        }
    }
    else
    {
        m_pBaseSprite->setAnchorPoint(ccp(m_pSetting->baseAnchorX,m_pSetting->baseAnchorY)); // special case for temp tile holer
    }
    
    
    m_CurrentHp = getMaxHp();
    
    this->addChild(m_pBaseSprite ? m_pBaseSprite : m_pRotatingSprite);
    

}

void Building::initFromSetting()
{
    m_Level = 1;
    m_AttackRange = m_pSetting->baseRange;
    m_AttackDamage = m_pSetting->baseAttack;
    m_AttackInterval = m_pSetting->attackInterval;
    
    recalculateMaxHp();
    m_CurrentHp = getMaxHp();
}

void Building::updateScale()
{
    float scale = m_pSetting->defaultScale;

    CCTMXTiledMap* pTileMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    
    if (m_pBaseSprite && id != 0)
    {
        CCRect baseRect = m_pBaseSprite->getTextureRect();
        scale = scale * pTileMap->getTileSize().width / (baseRect.getMaxX() - baseRect.getMinX());
        m_pBaseSprite->setScale(scale);
        
        CCRect rotatingRect = m_pRotatingSprite->getTextureRect();
//        CCLog("%f, %f",  m_pBaseSprite->getContentSize().width , textureRect.getMaxX() - textureRect.getMinX());
//        float rotateScale = (baseRect.getMaxX() - baseRect.getMinX()) / (rotatingRect.getMaxX() - rotatingRect.getMinX());
//        if
//        m_pRotatingSprite->setScale(scale);
    }
    else if (!m_pBaseSprite)
    {
        CCRect rotatingRect = m_pRotatingSprite->getTextureRect();
        scale = scale * pTileMap->getTileSize().width / (rotatingRect.getMaxX() - rotatingRect.getMinX());
        m_pRotatingSprite->setScale(scale);
    }

}

int Building::getBuildingTileIndex(int x, int y)
{
    
    CCSize size = GameScene::getInstance()->sharedGameStage->getGroundMap()->getMapSize();
    
    if (x < 0 ||
        y < 0 ||
        x > size.width-1 ||
        y > size.height-1)
    {
        return -1;
    }
    
    int index = x * size.height + y;
    
    return index;
}


float Building::getOrderZ()
{
    return m_CurrentOrderZ;
}

CCSize Building::getTileSize()
{
    return CCSizeMake(1, 1);
}


void Building::select()
{
    selected = true;
}

void Building::unselect()
{
    selected = false;
    removeBuildingMenu();
}

bool Building::isSelected()
{
    return selected;
}

bool Building::isActive()
{
    return active && getParent() != NULL;
}

void Building::activate()
{
    active = true;
    BuildingStatusBar* pStatusBar = BuildingStatusBar::create(this);
    
    CCSprite* pMainSprite = isBaseVisible() ? m_pBaseSprite :  m_pRotatingSprite;
    
    CCSize size = pMainSprite->getContentSize();
    
    CCPoint pos = ccp(size.width/2, size.height/5);
    
    
    CCSprite* shadow = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("npc_shadow.png"));
    shadow->setAnchorPoint(ccp(0.5, 0.6));
    shadow->setPosition(pMainSprite->getPosition());
    addChild(shadow);

    
    pStatusBar->setAnchorPoint(ccp(0.5, 0.5));
    pStatusBar->setPosition(pMainSprite->getPosition());
    this->addChild(pStatusBar);
    

    
//    pStatusBar->setScale((pMainSprite->getTextureRect().size.height/3) / pStatusBar->getContentSize().height);
    
    
    this->setColor(ccc3(255,255,255));
    if (m_pBuildingMenu)
    {
        this->removeChild(m_pBuildingMenu, true);
    }
    
//        m_pAttackComponent = BuildingShootComponent::create();
    
    switch (m_pSetting->attackType)
    {
        case ENUM_BUILDING_ATTACK_THROW_STONE:
            m_pAttackComponent = BuildingThrowComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_SINGLE_ARROW:
            m_pAttackComponent = BuildingShootComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_MUD:
            m_pAttackComponent = BuildingMudAttackComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_MULTI_ARROW:
            m_pAttackComponent = BuildingMultiArrowAttackComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_CANNON:
            m_pAttackComponent = BuildingCannonComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_FIREGUN:
            m_pAttackComponent = BuildingFiregunComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_POISON:
            m_pAttackComponent = BuildingPoisonAttackComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_SHOTGUN:
            m_pAttackComponent = BuildingShotgunComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_SNIPER:
            m_pAttackComponent = BuildingSniperComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_MACHINEGUN:
            m_pAttackComponent = BuildingMachineGunComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_ROCKET:
            m_pAttackComponent = BuildingRocketAttackComponent::create();
            break;
        case ENUM_BUILDING_ATTACK_BASE:
            m_pAttackComponent = BuildingBase::create();
            break;
    }
    
    
    if (m_pAttackComponent)
    {
        this->addChild(m_pAttackComponent);
        m_pAttackComponent->setupAnimation(id);
        m_pAttackComponent->scheduleUpdate();
    }
//    sprite->setVisible(false);
    if (m_pBaseSprite)
    {
        m_pBaseSprite->setOpacity(255);
    }
    
    m_pRotatingSprite->setOpacity(255);

    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    float orderZ = MapHelper::calculateOrderZ(originTilePos, pMap);
    this->getParent()->reorderChild(this, orderZ);
    
    
    if (m_pSetting->type == ENUM_BUILDING_ASSIST)
    {
        
        switch (getId())
        {
            case 3:
            case 7:
            {
                showAssistingTileAnimations();
                
            }
                break;
        }
    }
    
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

}

void Building::showAssistingTileAnimations()
{
     StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    
    char buffer[128] = {0};
    
    CCArray* frames = CCArray::create();
    for(int i=0;i<8;++i)
    {
        sprintf(buffer, "building_%d_tile_%d.png",id, i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
        if (!frame)
        {
            break;
        }
        
        frames->addObject(frame);
    }
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.4f);
    
    sprintf(buffer, "building_%d_tile_0.png",id);
    CCPoint originTile = getOriginTilePosition();
    
    for (int i= originTile.y-1; i<=originTile.y+1; i++)
    {
        for (int j=originTile.x-1; j<=originTile.x+1; j++)
        {
            if (m_Level == 1)
            {
                if (i!=originTile.y &&
                    j!=originTile.x)
                {
                    continue;
                }
            }
            else
            {
                if (i==originTile.y ||
                    j==originTile.x)
                {
                    continue;
                }

            }
            
            if (i==originTile.y &&
                j==originTile.x)
            {
                continue;
            }
            
            if (pMap->isRoadTile(ccp(j,i)))
            {
                CCPoint groundMapPos = MapHelper::tilePosToGroundMapPos(ccp(j+0.5,i+0.5));
                

                CCSprite* tileSprite = CCSprite::createWithSpriteFrameName(buffer);
                tileSprite->setPosition(groundMapPos);
                
                CCAnimate* action = CCAnimate::create(animation);
                CCRepeatForever* repeats = CCRepeatForever::create(action);

                tileSprite->runAction(repeats);
                
                float orderZ = MapHelper::calculateOrderZ(ccp(j-2, i-2), pMap);
                
                pMap->addChild(tileSprite, orderZ);
                
                m_AdditionalNodes.push_back(tileSprite);
                //                            float orderZ = MapHelper::calculateOrderZ(ccp(j+0.5, i+0.5), pMap);
                //                            addChild(tileSprite, getOrderZ() - 128);
            }
        }
    }

}

int Building::getId()
{
    return id;
}

void Building::actWhenAttacked(AttackableObject *pTarget)
{
    
}

bool Building::ableToAttack()
{
    return !failToAct();
}


float Building::getBaseSpeed()
{
    return 0;
}

void Building::setSpeed(float speed)
{
    
}

CCNode* Building::getAnimationPart()
{
    return getRotatingPart();
}


int Building::getMaxLevel()
{
    int maxLevel = GameData::getBuildingSetting(getId()).maxLevel;
    
    maxLevel += Player::getInstance()->getSkillLevel(ENUM_SKILL_UPGRADE_LIMIT);
    
    return maxLevel;
}

int Building::getBaseCost()
{
    int cost =  GameData::getBuildingSetting(id).baseCost;
    int level = Player::getInstance()->getSkillLevel(ENUM_SKILL_CONSTRUCTION);
    if (level > 0)
    {
        cost -= cost*(level * 0.05);
    }
    
    return cost;
}

int Building::getGemCost()
{
    return m_pSetting->gemCost;
}

int Building::getUpgradeCost()
{
    return getBaseCost() * pow(2, getLevel());
}

int Building::getUpgradeGemCost()
{
    return getGemCost()/3;
}

int Building::getRepairGemCost()
{
    return getGemCost()/3;
}

int Building::getRepairCost()
{
    int currentCost = getBaseCost() * pow(2, getLevel() - 1);

    return (currentCost * 0.75) * m_RepairTimes;
}

void Building::recalculateMaxHp()
{
    int hp;
    
    hp = m_pSetting->baseHp * pow(3, getLevel()-1);
    
    hp *= 1 + Player::getInstance()->getSkillLevel(ENUM_SKILL_BUILDING_CONCRETE) * 0.1;

    m_MaxHp = hp;
}

void Building::repair()
{
    
    recalculateMaxHp();
    int currentMaxHp = getMaxHp();
    
    float repairRatio = 0.6f;
    
    m_CurrentHp = m_CurrentHp + currentMaxHp * repairRatio;
    if (m_CurrentHp > currentMaxHp)
    {
        m_CurrentHp = currentMaxHp;
    }

    m_RepairTimes++;
    
}

float Building::getDamage()
{
    float damage = m_AttackDamage;

    float bonus = Player::getInstance()->getSkillLevel(ENUM_SKILL_SHOOT_ACCURACY) * 0.1;
    damage *= (1+bonus);
    
    return damage;
}

float Building::getAttackRange()
{
    float range = m_AttackRange;

    float bonus = Player::getInstance()->getSkillLevel(ENUM_SKILL_TELESCOPE) * 0.05;
    range *= (1 + bonus);

    if (GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId())->environment == ENUM_MAP_ENVIRONMENT_SANDY_WIND ||
        GameData::getMapConfig(GameScene::getInstance()->sharedGameStage->getMapId())->maxLevel == 1)
    {
        // range reduce to 70% in sandy wind environment or endless mode
        range *= 0.7f;
    }
    
    return range;
}

float Building::getAttackInterval()
{
    return m_AttackInterval;
}

float Building::getAttackRate()
{
    return m_AttackInterval;
}


CCPoint Building::getShootPosition()
{
    return getFireGroundPosition();
}

CCPoint Building::getAttackablePosition()
{
    return m_CenterIsoMapPos;
}

const CCPoint& Building::getAttackableMapPosition()
{
    return m_CenterMapPos;
}

void Building::reduceHp(float damage, bool ignoreDefense)
{
    m_CurrentHp -= damage;
    
    GameScene::getInstance()->sharedGameStage->addMonsterKillScore(-damage);

}


bool Building::isDead()
{
    return m_CurrentHp <= 0;
}

void Building::remove()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	for (CCNode* addtionalNode : m_AdditionalNodes)
    {
        addtionalNode->removeFromParentAndCleanup(true);
    }
    
    if (GameScene::getInstance()->sharedGameStage->getBuildingManager()->removeBuilding(originTilePos.x, originTilePos.y))
    {
        active = false;
        m_CurrentHp = 0;
//        createDestructionAnimation();
        if (selected)
        {
            GameScene::getInstance()->sharedMainUI->getStageUI()->removeBuildingMenu();
        }
        
        GameScene::getInstance()->sharedGameStage->removeBuilding(this);
    }
    
}

void Building::destroy()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("explode.wav");
    
    Explosion::create("building_explosion",this->getAttackablePosition());
    
    remove();
    
    if (m_IsPlayerBuilding && GameScene::getInstance()->sharedGameStage->getComputerPlayer())
    {
        // destroyed by computer
        ComputerPlayer* pComputer = static_cast<ComputerPlayer*>(GameScene::getInstance()->sharedGameStage->getComputerPlayer());
        pComputer->updateExperience(100 * getLevel());
    }
    else if (!m_IsPlayerBuilding)
    {
        Player::getInstance()->updateExperience(100 * getLevel());
    }

}



void Building::showBuildingActionMenu()
{
    if (m_pBuildingMenu || Player::getInstance()->isDead() ||GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }
    
    if (GameScene::getInstance()->sharedGameStage->getBuildingManager()->hasUnplacedBuilding())
    {
        GameScene::getInstance()->sharedGameStage->getBuildingManager()->removeBuildingToPlace();
    }

    if (m_pSetting->attackType != ENUM_BUILDING_ATTACK_BASE)
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showBuildingUpgradeMenu(this);
    }
    else
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showPlayerBaseMenu(this);
    }
    
}

void Building::removeBuildingMenu()
{
    if (m_pBuildingMenu != NULL)
    {
        this->removeChild(m_pBuildingMenu, true);
        m_pBuildingMenu = NULL;
    }
}

void Building::upgradeLevel()
{
    
    m_Level++;

    recalculateMaxHp();

    switch (m_pSetting->type)
    {
        case ENUM_WEAPON_SNIPE:
            m_AttackDamage *= 2.0f;
            m_AttackRange += MapHelper::MAP_TILE_LENGTH * 0.5f;
            break;
        case ENUM_WEAPON_MISSLE:
            m_AttackRange += MapHelper::MAP_TILE_LENGTH;
            m_AttackDamage *= 1.5f;
            break;
        case ENUM_WEAPON_SHOT:
            m_AttackInterval *= 0.8f;
            if (m_AttackInterval < 0)
            {
                m_AttackInterval = 0;
            }
            m_AttackDamage *= 1.2f;
            break;
            
    }
    
    m_CurrentHp = getMaxHp();
    
    if (m_pSetting->type == ENUM_BUILDING_ASSIST && m_Level == 2)
    {
        showAssistingTileAnimations();
    }
}

CCPoint Building::getFireGroundPosition()
{
    CCSprite* pSprite = m_pBaseSprite ? m_pBaseSprite : m_pRotatingSprite;
    /*
    if (baseSpriteVisible)
    {
        return MapHelper::tilePosToGroundMapPos(originTilePos);
    }
    */
    CCPoint pos = pSprite->getPosition();
    pos = pSprite->getParent()->convertToWorldSpace(pos);
    pos = GameScene::getInstance()->sharedGameStage->getGroundMap()->convertToNodeSpace(pos);
    return pos;
}

void Building::finishAttack()
{
    
}

void Building::setStatus(ENUM_BUFF_STATUS status, float value, float duration)
{
    if (status == ENUM_BUFF_STATUS_SPIDER_WEB)
    {
        AttackableObject::setStatus(status, value, duration);
    }
    
    return;
}

bool Building::isTouched(const cocos2d::CCPoint &touchPos)
{
    if (!m_IsPlayerBuilding)
    {
        return false;
    }
    
    // check if it's touching the tile
    CCPoint pos = this->convertToWorldSpace(touchPos);
    CCPoint groundMapPos = GameScene::getInstance()->sharedGameStage->getGroundMap()->convertToNodeSpace(pos);
    CCPoint tileTouched = MapHelper::groundMapPosToTilePos(groundMapPos, GameScene::getInstance()->sharedGameStage->getGroundMap(), false);
    
    if (tileTouched.x == originTilePos.x && tileTouched.y == originTilePos.y)
    {
        return true;
    }
    // only check the touching tile
    
    return false;
    /*
    CCRect boundingRect;
    if (m_pBaseSprite)
    {
        
        boundingRect = m_pBaseSprite->getTextureRect();
        
        float minX = boundingRect.getMinX();
        float maxX = boundingRect.getMaxX();
        float minY = boundingRect.getMinY();
        float maxY = boundingRect.getMaxY();
        float width = maxX - minX;
        float height = maxY - minY;
        boundingRect.setRect(minX + width * 0.1, minY + 5, width * 0.8, height - 10);
        
        if (boundingRect.containsPoint(touchPos))
        {
            return true;
        }

    }

    if (m_pRotatingSprite)
    {
        boundingRect = m_pRotatingSprite->getTextureRect();
        
        float x = boundingRect.getMinX();
        float y = boundingRect.getMinY();
        
        CCPoint min = m_pRotatingSprite->getParent()->convertToWorldSpace(ccp(x,y));
        min = this->convertToNodeSpace(min);
        
        x = boundingRect.getMaxX();
        y = boundingRect.getMaxY();
        CCPoint max = m_pRotatingSprite->getParent()->convertToWorldSpace(ccp(x,y));
        max = this->convertToNodeSpace(max);
        
        
        boundingRect.setRect(min.x, min.y, max.x - min.x, max.y - min.y);
        
        // copy & paste (just being lazy)
        float minX = boundingRect.getMinX();
        float maxX = boundingRect.getMaxX();
        float minY = boundingRect.getMinY();
        float maxY = boundingRect.getMaxY();
        float width = maxX - minX;
        float height = maxY - minY;
        boundingRect.setRect(minX + width * 0.1, minY + height * 0.1, width * 0.8, height*0.8);
        
        if (boundingRect.containsPoint(touchPos))
        {
            return true;
        }
    }
    
    return false;
     */
}

bool Building::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (CCDirector::sharedDirector()->isPaused())
    {
        return false;
    }
    
//    CCPoint screenLocation = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
//    CCPoint touchGroundPos = MapHelper::screenPosToGroundPos(screenLocation);
    CCPoint touchPos = this->convertTouchToNodeSpace(pTouch);

    bool touched = isTouched(touchPos);
    
    if (touched)
    {
        m_TapStartPosition = touchPos;
        
        return true;
    }
    
    return false;
}

void Building::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!selected || isActive())
    {
        return;
    }
    
    // Maybe we should disable moving building?
    /*
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;

    CCPoint screenLocation = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    CCPoint groundPos = GameScene::getInstance()->sharedGameStage->getGroundMap()->convertToNodeSpace(screenLocation);
    
    CCPoint tilePos = MapHelper::groundMapPosToTilePos(groundPos, pStage->getGroundMap(), false);
    moveToTile(tilePos);
    */
}

void Building::ccTouchEnded(CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint touchPos = this->convertTouchToNodeSpace(pTouch);
    bool touched = isTouched(touchPos);
    
    if (touched)
    {
        CCPoint diff = ccpSub(touchPos, m_TapStartPosition);
        
        if (abs((int)diff.x) <50 && abs((int)diff.y)<50)
        {
            
            if (failToAct())
            {
                updateStatusByTap();
            }
            else
            {
                BuildingManager* pBuildingManager = GameScene::getInstance()->sharedGameStage->getBuildingManager();
                pBuildingManager->unselectAllBuildings();
                selected = true;
                
                if (isActive())
                {
                    showBuildingActionMenu();
                }
                
            }
            
        }
        
    }
    else
    {
        selected = false;
    }

    
    if (!selected || isActive())
    {
        return;
    }
    
}

void Building::moveToTile(const cocos2d::CCPoint &tilePos)
{
    CCPoint finalPos = MapHelper::tilePosToGroundMapPos(tilePos);
    
    this->setPosition(finalPos);
    
    originTilePos = tilePos;
    
//    resetAttackableTiles();
    
    CCTMXTiledMap* pTileMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    m_CurrentOrderZ = MapHelper::calculateOrderZ(ccp(tilePos.x,tilePos.y), pTileMap);
    
    
    // find center point offset (relative)
    centerMapOffset = ccp(tileWidth * MapHelper::MAP_TILE_LENGTH / 2.0f, tileLength * MapHelper::MAP_TILE_LENGTH / 2.0f);
    CCPoint origin = MapHelper::orthMapPosToIsoMapPos(ccp(0, 0));
    centerGroundOffset = MapHelper::orthMapPosToIsoMapPos(centerMapOffset);
    
    centerGroundOffset = ccpSub(centerGroundOffset, origin);
    
    m_CenterIsoMapPos = ccpAdd(getPosition(), centerGroundOffset);;
    m_CenterMapPos = MapHelper::isoMapPosToOrthMapPos(m_CenterIsoMapPos);


}


void Building::dropMoney()
{
    CollectableMoney* money = CollectableMoney::create();
    money->setBuildingTileIndex(getBuildingTileIndex(originTilePos.x, originTilePos.y));
    
    CCPoint offset = CCPointZero;
    int dx = rand()%20;
    offset.x = rand()%2 == 0 ? centerGroundOffset.x + dx : centerGroundOffset.x - dx;
    int dy = rand()%20;
    offset.y = rand()%2 == 0 ? centerGroundOffset.y + dy : centerGroundOffset.y - dy;
    
    money->setOffsetToBuilding(offset);
    
    offset = ccpAdd(getPosition(), offset);
    money->setPosition(offset);
    
    GameScene::getInstance()->sharedGameStage->addChild(money);
    
}

void Building::update(float dt)
{
    if (GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }

    
    if (!isActive())
    {
        
        return;
    }
    
    updateStatus(dt);
    
    /*
    int recoverSkillLevel = Player::getInstance()->getSkillLevel(ENUM_SKILL_BUILDING_HP_REGENERATION);
    if (recoverSkillLevel > 0)
    {
        float recoverd = getMaxHp() * recoverSkillLevel * 0.005 * dt;
        m_CurrentHp += recoverd;
        if (m_CurrentHp > getMaxHp())
        {
            m_CurrentHp = getMaxHp();
        }
    }
    */
    /*
    if (this->id == 1)
    {
        // for money building
        timeElasped += dt;
        
        if (timeElasped > 15)
        {
            dropMoney();
            timeElasped = 0;
        }
        return;
        
    }
    */

}

/*
bool Building::isWithinAttackRange(const cocos2d::CCPoint &tilePos)
{
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    CCSize mapSize = pStage->getGroundMap()->getMapSize();
    
    int index = (int)tilePos.y * mapSize.width + (int)tilePos.x;
    
    __gnu_cxx::hash_set<int>::iterator it = m_AttackableTiles.find(index);
    
    if (it!=m_AttackableTiles.end())
    {
        int offset = *it;
        CCLog("%d", offset);
    }
    
    return m_AttackableTiles.find(index) != m_AttackableTiles.end();
    
}
*/

void Building::showAttackRange()
{
    if (!isActive() && GameScene::getInstance()->sharedGameStage->getBuildingManager()->hasBuilding(originTilePos.x, originTilePos.y))
    {
        // don't show range if it's unplaced and the location is occupied already
        return;
    }
    
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    CCSize mapSize = pStage->getGroundMap()->getMapSize();

    /*
    __gnu_cxx::hash_set<int>::iterator it = m_AttackableTiles.begin();
    while (it!= m_AttackableTiles.end())
    {
        int index = *it;
        
        int y = index / mapSize.width;
        int x = index - y * mapSize.width;
        
        CCPoint tilePos = ccp(x,y);
        CCPoint pos[4];
        
        pos[0] = MapHelper::tilePosToStagePos(tilePos);
        pos[0] = this->convertToNodeSpace(pStage->convertToWorldSpace(pos[0]));
        
        pos[1] = MapHelper::tilePosToStagePos(ccp(tilePos.x+1, tilePos.y));
        pos[1] = this->convertToNodeSpace(pStage->convertToWorldSpace(pos[1]));
        
        pos[2] = MapHelper::tilePosToStagePos(ccp(tilePos.x+1, tilePos.y+1));
        pos[2] = this->convertToNodeSpace(pStage->convertToWorldSpace(pos[2]));
        
        pos[3] = MapHelper::tilePosToStagePos(ccp(tilePos.x, tilePos.y+1));
        pos[3] = this->convertToNodeSpace(pStage->convertToWorldSpace(pos[3]));
        
        ccDrawSolidPoly(pos, 4, ccc4f(0,0.4,0,0.00));
        it++;
    }
    
    AttackableObject::draw();
    return;
    */
    
    Function::lazy_init();
    
    const int segments = 40;
//    float radius = attackRange / getScale();
    float radius = getAttackRange();
    const float angle = 0;
    
//    CCPoint offset = getFireOffset();
//    offset = CCPointZero;
    
    /*
    CCPoint originPos = getPosition();
    
    CCPoint originMapPos = MapHelper::isoMapPosToOrthMapPos(originPos);
    CCPoint centerMapOffset = ccp(MapHelper::MAP_TILE_LENGTH * tileWidth / 2, MapHelper::MAP_TILE_LENGTH * tileLength / 2);
    CCPoint centerMapPos = ccpAdd(originMapPos, centerMapOffset);
    
    CCPoint centerGroudPos = MapHelper::orthMapPosToIsoMapPos(centerMapPos);
    */
    CCPoint centerMapPos = m_CenterMapPos;
    CCPoint centerGroundPos = m_CenterIsoMapPos;
    
    int additionalSegment = 1;
    
    const float coef = 2.0f * (float)M_PI/segments;
    
    GLfloat *vertices = (GLfloat*)calloc( sizeof(GLfloat)*2*(segments+2), 1);
    if( ! vertices )
        return;
    

    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads + angle) + centerMapPos.x;
        GLfloat k = radius * sinf(rads + angle) + centerMapPos.y;
  
        CCPoint screenPos = ccpSub(MapHelper::orthMapPosToIsoMapPos(ccp(j,k)), centerGroundPos);
        screenPos = ccpAdd(screenPos, centerGroundOffset);
        j = screenPos.x;
        k = screenPos.y;
  
        vertices[i*2] = j;
        vertices[i*2+1] = k;
    }
    
//    vertices[(segments+1)*2] = center.x;
//    vertices[(segments+1)*2+1] = center.y;

    ccColor4F color = ccc4f(0.3f, 0.3f, 0.8f, 0.2f);
    
    Function::setupShader((GLfloat*)&color.r);
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) segments+additionalSegment);
    
    free( vertices );
    CC_INCREMENT_GL_DRAWS(1);
    
/*
    glLineWidth(10);
    ccDrawColor4B(255, 0, 0, 255);
    ccDrawLine(ccp(0,0), ccp(400,400));
    ccDrawLine(ccpAdd(CCPointZero, centerGroundOffset), ccp(400,400));
*/
}

void Building::setColor(ccColor3B color)
{
    if (m_pBaseSprite)
    {
        m_pBaseSprite->setColor(color);
    }
    
    if (m_pRotatingSprite)
    {
        m_pRotatingSprite->setColor(color);
    }
}

void Building::showConstructionAnimation()
{
    showExtraAnimation(AnimationManager::getInstance()->sharedBuildingConstructionFrames, ccp(0.5,0.35), 1.9f);
}

void Building::showUpgradeAnimation()
{
    showExtraAnimation(AnimationManager::getInstance()->sharedBuildingUpgradeFrames, ccp(0.5,0.2), 1);
}

void Building::showExtraAnimation(CCArray* frames, CCPoint anchorPoint, float duration)
{
    
    // building is activated after 2 sec. animation should be done before that
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, duration/frames->count());
    CCAnimate* animate = CCAnimate::create(animation);
    
    CCSmartSprite* sprite = CCSmartSprite::createWithSpriteFrame(static_cast<CCSpriteFrame*>(frames->objectAtIndex(0)));

    sprite->setAnchorPoint(anchorPoint);
    sprite->setPosition(m_CenterIsoMapPos);
    
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(sprite);
    
    sprite->runAction(CCSequence::create(animate, CCCallFunc::create(sprite, callfunc_selector(CCSmartSprite::removeSelf)), NULL));
}

void Building::removeExtraAnimation()
{
    
    getBaseSprite()->removeChildByTag(EXTRA_ANIMATION_TAG, true);
}

void Building::showMapTiles()
{
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    CCTMXTiledMap* pTileMap = pStage->getGroundMap();
    
    CCSize tileSize = pTileMap->getTileSize();

//    float originX=this->getContentSize().width * this->getAnchorPoint().x;
//    float originY=this->getContentSize().height * this->getAnchorPoint().y;
    float originX = 0;
    float originY = 0;
    CCPoint points[4];
    

    glLineWidth(2.0f);
    
    ccColor4F color = ccc4f(0.3f, 1.0f, 0.3f, 1.0f);
    
    if (!active)
    {
        color = ccc4f(0.0f, 0.0f, 0.5f, 0.5f);
    }
    
    
    if (0 == (Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_PLACE_SNIPE_WEAPON))
    {
        // if still in tutorial
        CCPoint tilePos = getOriginTilePosition();

        // place to target position
        CCPoint expectTilePos = GameScene::getInstance()->sharedGameStage->getGroundMap()->getFirstTimeLightWeaponPlaceTile();
        if ((int)tilePos.x != expectTilePos.x || (int)tilePos.y != expectTilePos.y)
        {
            color = ccc4f(1, 0, 0, 1);
        }
    }

    
    for(int i = 0; i < tileLength; ++i)
    {
        if (i > 0)
        {
            originX -= tileSize.width/2;
            originY -= tileSize.height/2;
        }
        
        for (int j = 0; j < tileWidth; ++j)
        {
            float x = originX + j * tileSize.width / 2;
            float y = originY - j * tileSize.height / 2;
            
            points[0] = ccp(x,y);
            points[1] = ccp(x-tileSize.width/2, y-tileSize.height/2);
            points[2] = ccp(x, y - tileSize.height);
            points[3] = ccp(x+tileSize.width/2, y-tileSize.height/2);
//            ccDrawPoly(points, 4, true);
            ccDrawSolidPoly(points, 4, color);
        }
    }

}

void Building::draw()
{
//    CCNode::draw();
    
    if (selected || !isActive())
    {
    
        if (id!= 0)
        {
            showMapTiles();
            
            if (!m_Unplaceable)
            {
                showAttackRange();
            }
        }
    }
    
    /*
    if (isActive())
    {
        CCSize size = baseSpriteVisible ? m_pBaseSprite->getContentSize() : m_pRotatingSprite->getContentSize();
    
        size.width *= 0.5;
    
        glLineWidth( 5.0f );
        CCPoint startPos = CCPointMake(-size.width/2, size.height/8);
    
        ccDrawColor4B(255,0,0,255);
        ccDrawLine( startPos, CCPointMake(startPos.x+size.width, startPos.y) );
    
        ccDrawColor4B(128,255,128,255);
        ccDrawLine( startPos, CCPointMake(startPos.x+size.width * hp/maxHp, startPos.y) );

        // restore line width
        glLineWidth(1.0f);
    }
    */
  
//    showAttackRange();
}