//
//  BuildingShootComponent.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/17/12.
//
//

#include "BuildingShootComponent.h"
#include "GameScene.h"
#include "BuildingSetting.h"
#include "GameData.h"
#include "SimpleAudioEngine.h"
#include "MapHelper.h"
#include "Explosion.h"
#include "SpecialActions.h"


// line shoot weapon
BuildingShootComponent::~BuildingShootComponent()
{
}

bool BuildingShootComponent::init()
{
    BuildingAttackComponent::init();
    
    return true;
}


CCPoint BuildingShootComponent::getFirePosition()
{
//    float xOffsets[] = {130, 128, 91, 54, 90};
//    float yOffsets[] = {60,  40,  22, 80, 103};
    float xOffsets[] = {84, 84, 91, 88, 90};
    float yOffsets[] = {62, 67, 67, 69, 72};
    
    return calculateFirePosition(xOffsets, yOffsets);
}

CCActionInterval* BuildingShootComponent::createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed)
{
    float duration = getMapDistance(targetMapPos, currentMapPos) / mapSpeed;
    return CCMoveTo::create(duration, targetPos);
}


// throw weapon

BuildingThrowComponent::~BuildingThrowComponent()
{
}

bool BuildingThrowComponent::init()
{
    BuildingAttackComponent::init();
    
    return true;
}

CCPoint BuildingThrowComponent::getFirePosition()
{
    float xOffsets[] = {81, 73, 92, 104, 91};
    float yOffsets[] = {36, 42, 44, 33,  34};
    
    return calculateFirePosition(xOffsets, yOffsets);

}

CCActionInterval* BuildingThrowComponent::createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed)
{
    float duration = getMapDistance(targetMapPos, currentMapPos) / mapSpeed;
    
    
    return CCJumpTo::create(duration, targetPos, MapHelper::MAP_TILE_LENGTH * 3, 1);
}


// firegun
BuildingFiregunComponent::~BuildingFiregunComponent()
{
}

bool BuildingFiregunComponent::init()
{
    BuildingAttackComponent::init();
    
    return true;
}

CCPoint BuildingFiregunComponent::getFirePosition()
{
    // only use quadrant 1,2,3,6,7
    float xOffsets[] = {250, 221, 137, 44, 136};
    float yOffsets[] = {115, 65,  26,  167,198};

    return calculateFirePosition(xOffsets, yOffsets);
}

CCActionInterval* BuildingFiregunComponent::createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed)
{
    float duration = getMapDistance(targetMapPos, currentMapPos) / mapSpeed;
    return CCMoveTo::create(duration, targetPos);
}


// Sniper
BuildingSniperComponent::~BuildingSniperComponent()
{
}

bool BuildingSniperComponent::init()
{
    BuildingAttackComponent::init();
    
    return true;
}

CCPoint BuildingSniperComponent::getFirePosition()
{
    // only use quadrant 1,2,3,6,7
    float xOffsets[] = {264, 231, 136, 41, 136};
    float yOffsets[] = {113, 60,  28,  167,192};
    
    return calculateFirePosition(xOffsets, yOffsets);
}

CCActionInterval* BuildingSniperComponent::createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed)
{
//    float duration = getMapDistance(targetMapPos, currentMapPos) / mapSpeed;
    float duration = 0;
    return CCMoveTo::create(duration, targetPos);
}


void BuildingSniperComponent::startAttack()
{
    m_waitingForAttack = false;

    CCPoint targetPos = m_AimedPosition;

    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    
    Building* building = static_cast<Building*>(this->getParent());
    //    CCPoint bulletPos = building->getShootPosition();
    
    CCPoint bulletPos = getFirePosition();
    /*
    float orderZ = MapHelper::calculateOrderZ(MapHelper::groundMapPosToTilePos(bulletPos, pStage->getGroundMap(), true), pStage->getGroundMap());
    CCSprite* bullet = createBulletAnimation(building->getId(), building->getLevel());
    
    bullet->setScale(0.6);
    
    CCPoint normalized = ccpNormalize(ccp(targetPos.x - bulletPos.x, targetPos.y - bulletPos.y));
//    float rotateDegree = CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x));
    float rotateDegree = 45 * (m_Quadrant - 1);
    bullet->setRotation( rotateDegree + 180);
    bullet->setAnchorPoint(ccp(0,0.5));
    bullet->setPosition(bulletPos);
    pStage->getGroundMap()->addChild(bullet, orderZ);
    
    
    bullet->runAction(CCSequence::create(CCDelayTime::create(0.3),
                                         CCCallFuncO::create(this, callfuncO_selector(BuildingAttackComponent::damageEnemy), NULL),
                                         CCCallFuncO::create(this, callfuncO_selector(BuildingAttackComponent::removeBullet), bullet), NULL));
    */
    
    damageEnemy(NULL);
    
//    bullets->addObject(bullet);

}

// cannon
BuildingCannonComponent::~BuildingCannonComponent()
{
}

bool BuildingCannonComponent::init()
{
    BuildingAttackComponent::init();
    
    return true;
}

CCPoint BuildingCannonComponent::getFirePosition()
{
    // only use quadrant 1,2,3,6,7
    float xOffsets[] = {181, 172, 136, 92, 136};
    float yOffsets[] = {88,  66,  52, 102, 102};
    
    return calculateFirePosition(xOffsets, yOffsets);
}

CCActionInterval* BuildingCannonComponent::createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed)
{
    float duration = getMapDistance(targetMapPos, currentMapPos) / mapSpeed;
    return CCJumpTo::create(duration, targetPos, MapHelper::MAP_TILE_LENGTH * 2, 1);
}