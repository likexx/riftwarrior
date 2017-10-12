//
//  BuildingMultiArrowAttackComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 4/4/13.
//
//

#include "BuildingMultiArrowAttackComponent.h"
#include "Enemy.h"
#include "Building.h"
#include "GameScene.h"
#include "Explosion.h"
#include "MapHelper.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "math.h"
using namespace CocosDenshion;

#define PI 3.14159

BuildingMultiArrowAttackComponent::~BuildingMultiArrowAttackComponent()
{
    
}

bool BuildingMultiArrowAttackComponent::init()
{
    bool result = BuildingAttackComponent::init();
    
    return result;
}

CCActionInterval* BuildingMultiArrowAttackComponent::createBulletAction(cocos2d::CCPoint currentMapPos, cocos2d::CCPoint targetMapPos, cocos2d::CCPoint targetPos, float mapSpeed)
{
    float duration = getMapDistance(targetMapPos, currentMapPos) / mapSpeed;
    return CCMoveTo::create(duration, targetPos);
}

void BuildingMultiArrowAttackComponent::startAttack()
{
    m_waitingForAttack = false;
    
    removeTarget();


    float centerDegree = 45 * m_Quadrant;

    Building* building = static_cast<Building*>(this->getParent());
    CCPoint bulletPos = getFirePosition();

    float minDegree = centerDegree - 45;
    float maxDegree = centerDegree + 45;
    float step = 90/( 2 * building->getLevel());
    
    for (float degree = minDegree; degree <= maxDegree; degree += step)
    {
        createBullet(degree);
    }
    
}

CCPoint BuildingMultiArrowAttackComponent::getTargetPosition(cocos2d::CCPoint currentMapPos, float range,float degree)
{

//    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    
    CCPoint targetMapPos = currentMapPos;
    
    targetMapPos.x += range * cos(degree * PI/180);
    targetMapPos.y -= range * sin(degree * PI/180);
    
    return MapHelper::mapPosToGroundMapPos(targetMapPos);
    /*
    CCRect boundary = pStage->getMapBoundary();
    
    float minX = boundary.getMinX();
    float minY = boundary.getMinY();
    float maxX = boundary.getMaxX();
    float maxY = boundary.getMaxY();

    CCPoint target = currentMapPos;
    
    if (degree < 0)
    {
        degree = 360 + degree;
    }

    float width = 0;
    
    if ( degree > 90 && degree < 270)
    {
        target.x = minX;
        width = currentPos.x - target.x;
    }
    else if (degree < 90 || degree > 270)
    {
        target.x = maxX;
        width = target.x - currentPos.x;
    }

    if (degree <=180 && degree != 90)
    {
        float dy = width * tan(degree * PI /180);
        target.y = currentPos.y + (dy > 0 ? dy : -dy);
    }
    else if (degree >180 && degree != 270)
    {
        float dy = width * tan(degree * PI /180);
        
        target.y = currentPos.y - (dy > 0 ? dy : -dy);
    }
    else if (degree == 90)
    {
        target.y = maxY;
    }
    else if (degree==270)
    {
        target.y = minY;
    }
    
    return target;
    */
}


CCPoint BuildingMultiArrowAttackComponent::getFirePosition()
{
    float xOffsets[] = {112, 102, 90, 75, 91};
    float yOffsets[] = {57,  48,  40, 65, 68};
    
    return calculateFirePosition(xOffsets, yOffsets);
}


CCSprite* BuildingMultiArrowAttackComponent::createBullet(float degree)
{

    GameStage* pStage = GameScene::getInstance()->sharedGameStage;

    Building* building = static_cast<Building*>(this->getParent());
    CCPoint bulletPos = getFirePosition();
    
    float orderZ = MapHelper::calculateOrderZ(MapHelper::groundMapPosToTilePos(bulletPos, pStage->getGroundMap(), true), pStage->getGroundMap());
    CCSprite* bullet = createBulletAnimation(building->getId(), building->getLevel());
    
    bullet->setScale(0.6f);
    
    CCPoint bulletTilePos = MapHelper::groundMapPosToTilePos(bulletPos, pStage->getGroundMap(), true);
    CCPoint bulletMapPos = MapHelper::tilePosToMapPos(bulletTilePos.x, bulletTilePos.y, pStage->getGroundMap());
    CCPoint targetPos = getTargetPosition(bulletMapPos, building->getAttackRange(), degree);
    
    CCPoint normalized = ccpNormalize(ccp(targetPos.x - bulletPos.x, targetPos.y - bulletPos.y));
    float rotateDegree = CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x));
    bullet->setRotation( rotateDegree + 180);
    
    bullet->setPosition(bulletPos);
    pStage->getGroundMap()->addChild(bullet, orderZ);
    
    CCPoint currentMapPos = MapHelper::isoMapPosToOrthMapPos(building->getShootPosition());
    
    const CCPoint& targetMapPos = MapHelper::isoMapPosToOrthMapPos(targetPos);
    
    CCActionInterval* bulletAction = createBulletAction(currentMapPos, targetMapPos, targetPos, m_pSetting->bulletSpeed);
    
    
    bullet->runAction(CCSequence::create(bulletAction,
//                                         CCCallFuncO::create(this, callfuncO_selector(BuildingAttackComponent::damageEnemy), bullet),
                                         CCCallFuncO::create(this, callfuncO_selector(BuildingAttackComponent::removeBullet), bullet), NULL));
    
    bullets->addObject(bullet);
    
    return bullet;
}


void BuildingMultiArrowAttackComponent::processBullets(float dt)
{
    BuildingAttackComponent::processBullets(dt);

    vector<CCNode*> removeBullets;
    removeBullets.reserve(bullets->count());
    
    CCObject* pObj;
    Building* pBuilding = static_cast<Building*>(this->getParent());
    CCARRAY_FOREACH(bullets, pObj)
    {
        CCNode* pBullet = static_cast<CCNode*>(pObj);
        CCPoint tilePos = MapHelper::groundMapPosToTilePos(pBullet->getPosition(), GameScene::getInstance()->sharedGameStage->getGroundMap(), false);
        
        CCArray* pEnemies = GameScene::getInstance()->sharedGameStage->enemies;
        CCObject* pObj;
        
        
        CCARRAY_FOREACH(pEnemies, pObj)
        {
            Enemy* pEnemy = static_cast<Enemy*>(pObj);
            if (!pEnemy->isActive() || pEnemy->isDead())
            {
                continue;
            }
            
            CCPoint enemyTilePos = pEnemy->getCurrentTilePosition();
            if ((int)tilePos.x == (int)enemyTilePos.x && (int)tilePos.y == (int)enemyTilePos.y)
            {
                showHitEffect(pEnemy, pEnemy->getAttackablePosition());

                float damage = pBuilding->getDamage();
                pEnemy->reduceHp(damage);
                
                if (!pEnemy->isDead())
                {
                    pEnemy->actWhenAttacked(static_cast<AttackableObject*>(this->getParent()));
                }
                
                removeBullets.push_back(pBullet);
                break;
            }
        }
    }
    
    for(size_t i=0;i<removeBullets.size();++i)
    {
        if (removeBullets[i])
        {
            CCNode* pBullet = removeBullets[i];
            removeBullet(pBullet);
        }
    }

}
