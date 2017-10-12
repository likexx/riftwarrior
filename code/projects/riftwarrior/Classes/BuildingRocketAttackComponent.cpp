//
//  BuildingRocketAttackComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 4/28/13.
//
//

#include "BuildingRocketAttackComponent.h"
#include "Enemy.h"
#include "Building.h"
#include "GameScene.h"
#include "Explosion.h"
#include "MapHelper.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "math.h"
#include "SpecialActions.h"
using namespace CocosDenshion;

#define PI 3.14159

BuildingRocketAttackComponent::~BuildingRocketAttackComponent()
{
    
}

bool BuildingRocketAttackComponent::init()
{
    bool result = BuildingAttackComponent::init();
    
    return result;
}

CCActionInterval* BuildingRocketAttackComponent::createBulletAction(cocos2d::CCPoint currentMapPos, cocos2d::CCPoint targetMapPos, cocos2d::CCPoint targetPos, float mapSpeed)
{
    float duration = getMapDistance(targetMapPos, currentMapPos) / mapSpeed;
    return KKActionThrowTo::create(duration, targetPos, MapHelper::MAP_TILE_LENGTH * 4);
}

void BuildingRocketAttackComponent::startAttack()
{
    m_waitingForAttack = false;
    
    
    removeTarget();
    
    Building* pBuilding = static_cast<Building*>(getParent());
    
    int numberOfBullets = pBuilding->getLevel() * 3;
    
    float centerDegree = 45 * m_Quadrant;
    
    CCArray* actions = CCArray::create();

    for(int i=0;i<numberOfBullets;++i)
    {
        createBullet(centerDegree);
        
        runAction(CCSequence::create(CCDelayTime::create(0.04*i),
                                     CCCallFunc::create(this, callfunc_selector(BuildingRocketAttackComponent::playerFireSound)),
                                     NULL));
        
    }

    
    
}

void BuildingRocketAttackComponent::playerFireSound()
{
    
    Building* pBuilding = static_cast<Building*>(getParent());

    char buffer[128] = {0};
    sprintf(buffer, "sound/building_%d_fire.mp3", pBuilding->getId());
    // FIXME
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(buffer);
}

CCPoint BuildingRocketAttackComponent::getTargetPosition(cocos2d::CCPoint currentMapPos, float degree)
{
    
    //    GameStage* pStage = GameScene::getInstance()->sharedGameStage;

    CCPoint targetTilePos = MapHelper::groundMapPosToTilePos(m_TargetPosition, GameScene::getInstance()->sharedGameStage->getGroundMap(), false);
    targetTilePos.x += 0.5;
    targetTilePos.y += 0.5;
    
    // scatter around 2 tiles
    if (rand()%2 == 0)
    {
        targetTilePos.x += ((float)(rand()%200))/100.0f;
    }
    else
    {
        targetTilePos.x -= ((float)(rand()%200))/100.0f;
    }

    if (rand()%2 == 0)
    {
        targetTilePos.y += ((float)(rand()%200))/100.0f;
    }
    else
    {
        targetTilePos.y -= ((float)(rand()%200))/100.0f;
    }

    
    return MapHelper::tilePosToGroundMapPos(targetTilePos);

}

void BuildingRocketAttackComponent::playAttackAnimation()
{
    Building* building = static_cast<Building*>(this->getParent());
    CCSprite* pRotatingPart = building->getRotatingPart();
    
    if (m_Quadrant == 4 ||
        m_Quadrant == 5 ||
        m_Quadrant == 8)
    {
        pRotatingPart->setFlipX(true);
    }
    else
    {
        pRotatingPart->setFlipX(false);
    }
    
    
    if (!selectedTarget)
    {
        return;
    }
    
    AttackableObject* pTarget = static_cast<AttackableObject*>(selectedTarget);
    
    m_TargetPosition = pTarget->getAttackablePosition();

    
    CCFiniteTimeAction* actions = CCSequence::create(m_AnimationActions[m_Quadrant],
                                                     CCCallFunc::create(this, callfunc_selector(BuildingRocketAttackComponent::startAttack)),
                                                     CCCallFunc::create(this, callfunc_selector(BuildingRocketAttackComponent::resetAnimation)),
                                                     NULL);
    
    pRotatingPart->runAction(actions);
    
    
    
}


CCPoint BuildingRocketAttackComponent::getFirePosition()
{
    float xOffsets[] = {195, 186, 136, 67, 136};
    float yOffsets[] = {61,  35,  30, 79, 86};
    
    return calculateFirePosition(xOffsets, yOffsets);
}


CCSprite* BuildingRocketAttackComponent::createBullet(float degree)
{
    
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    
    Building* building = static_cast<Building*>(this->getParent());
    CCPoint bulletPos = getFirePosition();
    
    float orderZ = MapHelper::calculateOrderZ(MapHelper::groundMapPosToTilePos(bulletPos, pStage->getGroundMap(), true), pStage->getGroundMap());
    CCSprite* bullet = createBulletAnimation(building->getId(), building->getLevel());
    
    bullet->setScale(0.3f);
    
    CCPoint bulletTilePos = MapHelper::groundMapPosToTilePos(bulletPos, pStage->getGroundMap(), true);
    CCPoint bulletMapPos = MapHelper::tilePosToMapPos(bulletTilePos.x, bulletTilePos.y, pStage->getGroundMap());
    CCPoint targetPos = getTargetPosition(bulletMapPos, degree);
//    CCPoint targetPos = m_TargetPosition;
    
    CCPoint normalized = ccpNormalize(ccp(targetPos.x - bulletPos.x, targetPos.y - bulletPos.y));
    float rotateDegree = CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x));
    bullet->setRotation( rotateDegree + 180);
    
    
    bullet->setPosition(bulletPos);
    pStage->getGroundMap()->addChild(bullet, orderZ);
    
    CCPoint currentMapPos = MapHelper::isoMapPosToOrthMapPos(building->getShootPosition());
    
    const CCPoint& targetMapPos = MapHelper::isoMapPosToOrthMapPos(targetPos);
    
    CCActionInterval* bulletAction = createBulletAction(currentMapPos, targetMapPos, targetPos, m_pSetting->bulletSpeed);

    float duration = getMapDistance(targetMapPos, currentMapPos) / m_pSetting->bulletSpeed;
    
    float midX = currentMapPos.x + (targetMapPos.x - currentMapPos.x)/2;
    float midY = currentMapPos.y + (targetMapPos.y - currentMapPos.y)/2;
    CCPoint middlePos = MapHelper::mapPosToGroundMapPos(ccp(midX, midY));
    middlePos.y += MapHelper::MAP_TILE_LENGTH * 3;
    
    bullet->runAction(CCSequence::create(bulletAction,
                                         CCCallFuncO::create(this, callfuncO_selector(BuildingAttackComponent::damageEnemy), bullet),
                                         CCCallFuncO::create(this, callfuncO_selector(BuildingAttackComponent::removeBullet), bullet), NULL));
    
    bullets->addObject(bullet);
    
    return bullet;
}


void BuildingRocketAttackComponent::processBullets(float dt)
{
    BuildingAttackComponent::processBullets(dt);
    
    return;
    
}
