//
//  BuildingAttackComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 2/27/13.
//
//

#include "BuildingAttackComponent.h"
#include "GameScene.h"
#include "BuildingSetting.h"
#include "GameData.h"
#include "SimpleAudioEngine.h"
#include "MapHelper.h"
#include "Explosion.h"
#include "AnimationManager.h"
#include "BurningFire.h"

BuildingAttackComponent::~BuildingAttackComponent()
{
    if (bullets)
    {
        delete bullets;
    }

    map<int, CCSpriteFrame*>::iterator spriteIt = m_AnimationFrames.begin();
    while (spriteIt != m_AnimationFrames.end())
    {
        spriteIt->second->release();
        spriteIt++;
    }

    
    map<int, CCFiniteTimeAction*>::iterator actionIt = m_AnimationActions.begin();
    while (actionIt != m_AnimationActions.end())
    {
        actionIt->second->release();
        actionIt++;
    }

    spriteIt = m_IdleAnimationFrames.begin();
    while (spriteIt != m_IdleAnimationFrames.end())
    {
        spriteIt->second->release();
        spriteIt++;
    }
    
    
    actionIt = m_IdleAnimationActions.begin();
    while (actionIt != m_IdleAnimationActions.end())
    {
        actionIt->second->release();
        actionIt++;
    }

    
}

bool BuildingAttackComponent::init()
{
    bullets = new CCArray();
    
    m_Quadrant = 1;
    m_Degree = 111;
    m_waitingForAttack = false;
    m_IdleTime = 0;
    
    return true;
}


void BuildingAttackComponent::setupAnimation(int id)
{
    const BuildingSetting& setting = GameData::getBuildingSetting(id);
    const int MAX_FRAME_NUMBER = 16;
    for(int i = 1;i <= 8; ++i)
    {
        int quadrant = i;
        char buffer[128] = {0};
        
        int mirrored = i;
        
        switch (i)
        {
            case 4:
                mirrored = 2;
                break;
            case 5:
                mirrored = 1;
                break;
            case 8:
                mirrored = 6;
                break;
        }
        
        
        sprintf(buffer, "building_%d_%d_%d.png", id, mirrored, 0);
        
        CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
        if (!spriteFrame)
        {
            break;
        }
        
        spriteFrame->retain();
        m_AnimationFrames[quadrant] = spriteFrame;
        
        CCArray* frames = CCArray::createWithCapacity(2);
        
        
        for(int j=0;j<MAX_FRAME_NUMBER;++j)
        {
            sprintf(buffer, "building_%d_%d_%d.png", id, mirrored, j);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
            if (!frame)
            {
                break;
            }
            
            frames->addObject(frame);
        }
        
        // use the 1st frame as the ending frame
        frames->addObject(spriteFrame);
        
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, setting.attackAnimationTime/frames->count());
        
        CCAnimate* animate = CCAnimate::create(animation);
        animate->retain();
        
        m_AnimationActions[quadrant] = animate;
        
    }

    // process idle animation. copy&paste ... :(
    for(int i = 1;i <= 8; ++i)
    {
        int quadrant = i;
        char buffer[128] = {0};
        
        int mirrored = i;
        
        switch (i)
        {
            case 4:
                mirrored = 2;
                break;
            case 5:
                mirrored = 1;
                break;
            case 8:
                mirrored = 6;
                break;
        }
        
        sprintf(buffer, "building_%d_idle_%d_%d.png", id, mirrored, 0);
        
        CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
        if (!spriteFrame)
        {
            break;
        }
        
        spriteFrame->retain();
        m_IdleAnimationFrames[quadrant] = spriteFrame;
        
        CCArray* frames = CCArray::createWithCapacity(2);
        
        for(int j=0;j<MAX_FRAME_NUMBER;++j)
        {
            sprintf(buffer, "building_%d_idle_%d_%d.png", id, mirrored, j);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
            if (!frame)
            {
                break;
            }
            
            frames->addObject(frame);
        }
        
        // use the 1st frame as the ending frame
        frames->addObject(spriteFrame);
        
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.1f);
        
        CCAnimate* animate = CCAnimate::create(animation);
        animate->retain();
        
        m_IdleAnimationActions[quadrant] = animate;
        
    }

    
    m_pSetting = &setting;
    
    resetAnimation();
}


void BuildingAttackComponent::playAttackAnimation()
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
    
    
    CCFiniteTimeAction* actions = CCSequence::create(m_AnimationActions[m_Quadrant],
                                                     CCCallFunc::create(this, callfunc_selector(BuildingAttackComponent::startAttack)),
                                                     CCCallFunc::create(this, callfunc_selector(BuildingAttackComponent::resetAnimation)),
                                                     NULL);
    
    pRotatingPart->runAction(actions);
    
    char buffer[128] = {0};
    sprintf(buffer, "sound/building_%d_fire.mp3", building->getId());
    // FIXME
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(buffer);

    //    pRotatingPart->runAction(m_AnimationActions[m_Quadrant]);
    //    pRotatingPart->scheduleUpdate();
}

void BuildingAttackComponent::resetAnimation()
{
    Building* building = static_cast<Building*>(this->getParent());
    CCSprite* pRotatingPart = building->getRotatingPart();
    pRotatingPart->stopAllActions();
//    pRotatingPart->setRotation(0);
    
    CCPoint prevAnchorPoint = pRotatingPart->getAnchorPoint();
    CCPoint prevPos = pRotatingPart->getPosition();
    float prevScale = pRotatingPart->getScale();

    if (m_IdleAnimationFrames.find(m_Quadrant) != m_IdleAnimationFrames.end())
    {
        pRotatingPart->initWithSpriteFrame(m_IdleAnimationFrames[m_Quadrant]);
        pRotatingPart->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(m_IdleAnimationActions[m_Quadrant],
                                                                                               NULL)));
    }
    else
    {
        pRotatingPart->initWithSpriteFrame(m_AnimationFrames[m_Quadrant]);
    }
    
    
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
    

    pRotatingPart->setAnchorPoint(prevAnchorPoint);
    pRotatingPart->setPosition(prevPos);
    pRotatingPart->setScale(prevScale);
    
    //    pRotatingPart->initWithTexture(CCTextureCache::sharedTextureCache()->addImage("building_image/1.png"));
//    m_Quadrant = 1;
}

void BuildingAttackComponent::removeTarget()
{
    if (selectedTarget)
    {
        selectedTarget->release();
        selectedTarget = NULL;
    }
    m_waitingForAttack = false;
    m_IdleTime = 0;
}

void BuildingAttackComponent::update(float dt)
{
    if (GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }
    
    Building* pBuilding = static_cast<Building*>(getParent());
    
    processBullets(dt);

    if (m_waitingForAttack || m_IdleTime < m_pSetting->attackInterval)
    {
        m_IdleTime += dt;
        return;
    }

    if (!pBuilding->ableToAttack())
    {
        return;
    }
    
    CCPoint targetPos = CCPointZero;
    
    if (selectedTarget)
    {
        Enemy* pEnemy = static_cast<Enemy*>(selectedTarget);
        
        if (selectedTarget->isActive() &&
            !selectedTarget->isDead() &&
            isWithinAttackRange(pEnemy->getCurrentMapPosition()))
        {
            targetPos = selectedTarget->getAttackablePosition();
        }
        else
        {
            removeTarget();
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
        aimAtTarget(targetPos);
    }
    /*
    else if (m_Quadrant != 0)
    {
        resetAnimation();
    }
    */
    
}

AttackableObject* BuildingAttackComponent::findTarget()
{
    CCArray* enemies = GameScene::getInstance()->sharedGameStage->enemies;
    
    Building* building = static_cast<Building*>(this->getParent());
    
    if (GameScene::getInstance()->sharedGameStage->getComputerPlayer())
    {
        AttackableObject* pComputerPlayer = GameScene::getInstance()->sharedGameStage->getComputerPlayer();
        if (!pComputerPlayer->isDead() && isWithinAttackRange(pComputerPlayer->getAttackableMapPosition()))
        {
            return pComputerPlayer;
        }
    }
    
    
    CCObject* pTarget = NULL;
    CCARRAY_FOREACH(enemies, pTarget)
    {
        Enemy* enemy = static_cast<Enemy*>(pTarget);
        
        if (!enemy->isActive() ||
            enemy->isDead() ||
            (enemy->getEnemySetting()->type == ENUM_ENEMY_TYPE_AIR && !m_pSetting->airCapable))
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

bool BuildingAttackComponent::isWithinAttackRange(const cocos2d::CCPoint &targetMapPos)
{
    Building* building = static_cast<Building*>(this->getParent());

    CCPoint currentBuildingMapPos = building->getAttackableMapPosition();
    int range = building->getAttackRange();
    
    return AttackComponent::isWithinAttackRange(currentBuildingMapPos, targetMapPos, range, range*range);
    
}


void BuildingAttackComponent::aimAtTarget(const cocos2d::CCPoint &target)
{
    if (m_waitingForAttack)
    {
        return;
    }
    
    Building* building = static_cast<Building*>(this->getParent());
    CCSprite* pRotatingPart = building->getRotatingPart();
    /*
    if (pRotatingPart->numberOfRunningActions() > 0)
    {
        // still performing attacking animation, don't do new one
        return;
    }
    */
    
    m_AimedPosition = target;
    
    const CCPoint& pos = building->getFireGroundPosition();
    
    float dx = target.x - pos.x;
    float dy = target.y - pos.y;
    CCPoint normalized = ccpNormalize(ccp(dx, dy));
    int quadrant = 0;
    float compensation = 0;
    
    float degree = CC_RADIANS_TO_DEGREES(atan2(normalized.y, normalized.x));
    
    if (degree >= -22.5 && degree < 22.5)
    {
        quadrant = 1;
    }
    else
    {
        if (degree < 0)
        {
            degree += 360;
        }
        
        quadrant = (degree - 22.5) / 45 + 2;
    }
    
    
    compensation = (quadrant - 1) * 45;
    
    if (m_Quadrant != quadrant && m_AnimationActions.find(quadrant) != m_AnimationActions.end())
    {
        m_Quadrant = quadrant;
        
        //        switchAnimation(quadrant);
    }
    
    m_waitingForAttack = true;

    playAttackAnimation();

    //shootAtEnemy();
//    playAnimation();
}

CCSprite* BuildingAttackComponent::createBulletAnimation(int buildingId, int level)
{
    
    ENUM_BULLET_ANIMATION bulletType = m_pSetting->attackBullet;
    
    CCSprite* pBullet =  AnimationManager::getInstance()->createBulletAnimation(bulletType);
    return pBullet;
}


CCPoint BuildingAttackComponent::calculateFirePosition(float *xOffsets, float *yOffsets)
{
    Building* building = static_cast<Building*>(this->getParent());
    CCSprite* pRotatingSprite = building->getRotatingPart();

    return Function::calculateFirePosition(pRotatingSprite, m_Quadrant, xOffsets, yOffsets);
    
}

void BuildingAttackComponent::startAttack()
{
    m_waitingForAttack = false;
    m_IdleTime = 0;
    /*
    if (!selectedTarget)
    {
        return;
    }
    */
    Enemy* pEnemy = static_cast<Enemy*>(selectedTarget);
    
    CCPoint targetPos = m_AimedPosition;
    
    /*
    CCPoint targetPos = selectedTarget->getAttackablePosition();
    
    if (!isWithinAttackRange(pEnemy->getCurrentMapPosition()))
    {
        return;
    }
     */
    
//    m_waitingForAttack = true;
    
    if (m_pSetting->bulletSpeed == 0)
    {
        // hit immediately!
        damageEnemy(NULL);
        return;
    }
    
    
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    
    Building* building = static_cast<Building*>(this->getParent());
//    CCPoint bulletPos = building->getShootPosition();
  
    CCPoint bulletPos = getFirePosition();
    
    float orderZ = MapHelper::calculateOrderZ(MapHelper::groundMapPosToTilePos(bulletPos, pStage->getGroundMap(), true), pStage->getGroundMap());
    CCSprite* bullet = createBulletAnimation(building->getId(), building->getLevel());

    bullet->setScale(0.6f);
    
    CCPoint normalized = ccpNormalize(ccp(targetPos.x - bulletPos.x, targetPos.y - bulletPos.y));
    float rotateDegree = CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x));
    bullet->setRotation( rotateDegree + 180);
    
    bullet->setPosition(bulletPos);
    pStage->getGroundMap()->addChild(bullet, orderZ);
    
    
    CCPoint currentMapPos = MapHelper::isoMapPosToOrthMapPos(building->getShootPosition());
    
    const CCPoint& targetMapPos = MapHelper::isoMapPosToOrthMapPos(targetPos);
    
    CCActionInterval* bulletAction = createBulletAction(currentMapPos, targetMapPos, targetPos, m_pSetting->bulletSpeed);
    
    bullet->runAction(CCSequence::create(bulletAction, CCCallFuncO::create(this, callfuncO_selector(BuildingAttackComponent::damageEnemy), bullet), CCCallFuncO::create(this, callfuncO_selector(BuildingAttackComponent::removeBullet), bullet), NULL));
    
    bullets->addObject(bullet);
    
}

void BuildingAttackComponent::showHitEffect(AttackableObject* pTarget, CCPoint targetPos)
{
    
    switch(m_pSetting->attackType)
    {
        case ENUM_BUILDING_ATTACK_THROW_STONE:
            Explosion::create("hit_effect_dusty", targetPos);
            break;
        case ENUM_BUILDING_ATTACK_CANNON:
            Explosion::create("hit_effect_explosion1", targetPos);
            break;
        default:
        {
            //            Explosion::create("hit_effect_blood1", targetPos);
            CCSmartSprite* sprite = CCSmartSprite::createWithSpriteFrameName("hit_effect_blood1.png");
            sprite->setAnchorPoint(ccp(0.5, 0.4));
            //            sprite->setPosition(targetPos);
            sprite->setScale(0.2f);
            sprite->setOpacity(0.6f);
            sprite->runAction(CCSequence::create(CCScaleTo::create(0.2f, 1.0f),
                                                 CCCallFunc::create(sprite, callfunc_selector(CCSmartSprite::removeSelf)),
                                                 NULL));
            sprite->runAction(CCFadeTo::create(0.1f, 255));
            
            
            Enemy* pEnemy = static_cast<Enemy*>(pTarget);
            
            if (!pEnemy->getAnimation()->getCurrentSprite())
            {
                break;
            }
            
            
            sprite->setPosition(pEnemy->getAnimation()->getCurrentSprite()->getPosition());
            
            pEnemy->getAnimation()->addChild(sprite);
            
            //            GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(sprite);
        }
            break;
    }


    
}

void BuildingAttackComponent::processBullets(float dt)
{
    // just for fun: show lignting effect for bullet:
    /*
    CCObject* pObj;
    CCARRAY_FOREACH(bullets, pObj)
    {
        CCNode* pBullet = static_cast<CCNode*>(pObj);
        CCPoint tilePos = MapHelper::groundMapPosToTilePos(pBullet->getPosition(), GameScene::getInstance()->sharedGameStage->getGroundMap(), false);
        GameScene::getInstance()->sharedGameStage->lightOnTile(tilePos.x, tilePos.y, 1);
    }
     */
}

void BuildingAttackComponent::damageEnemy(CCNode* pBullet)
{
    
    Building* pBuilding = static_cast<Building*>(this->getParent());
    if (!pBuilding || pBuilding->isDead())
    {
        return;
    }
    
    if (pBullet)
    {
        char buffer[128] = {0};
        sprintf(buffer, "sound/building_%d_bullet_explosion.mp3", pBuilding->getId());
        // FIXME
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(buffer);
    }
    
    switch (pBuilding->getDamageType())
    {
        case ENUM_DAMAGE_SINGLE_TILE:
            makeSingleTileDamage(pBullet);
            break;
        case ENUM_DAMAGE_SINGLE_ENEMY:
            makeSingleEnemyDamage(pBullet);
            break;
        case ENUM_DAMAGE_BURNING_FIRE:
            makeBurningFire(pBullet);
            break;
    }
    
    
}


void BuildingAttackComponent::makeBurningFire(cocos2d::CCNode *pBullet)
{
    makeSingleTileDamage(pBullet);
    
    Building* pBuilding = static_cast<Building*>(this->getParent());
    CCPoint tilePos = MapHelper::groundMapPosToTilePos(pBullet->getPosition(), GameScene::getInstance()->sharedGameStage->getGroundMap(), false);
    CCPoint pos = MapHelper::tilePosToGroundMapPos(ccp(tilePos.x+0.5, tilePos.y + 0.5));
    BurningFire* fire = BurningFire::create(pBuilding->getId(), pBuilding->getLevel());
    fire->setMapPos(MapHelper::tilePosToMapPos(tilePos.x+0.5, tilePos.y+0.5, GameScene::getInstance()->sharedGameStage->getGroundMap()));
    fire->setPosition(pos);
//    float orderZ = MapHelper::calculateOrderZ(ccp(tilePos.x-1, tilePos.y-1), GameScene::getInstance()->sharedGameStage->getGroundMap());
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(fire, 0);
    
    /*
    // when bullet hits, the enemies in the tile will counter attack (but they won't if get hurted by the burning fire itself
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
            pEnemy->actWhenAttacked(static_cast<AttackableObject*>(this->getParent()));
            
        }
    }
    */
    
}

void BuildingAttackComponent::makeSingleEnemyDamage(CCNode* pBullet)
{
    if (!selectedTarget)
    {
        if (pBullet)
        {
            pBullet->stopAllActions();
        }
        return;
    }
    
    if (!selectedTarget->isActive() && selectedTarget->isDead())
    {
        removeTarget();
        return;
    }
    
    if (!pBullet || bulletHitTarget(pBullet->getPosition(), selectedTarget->getAttackablePosition()))
    {
        Building* building = static_cast<Building*>(this->getParent());
        
        selectedTarget->reduceHp(building->getDamage());
        showHitEffect(selectedTarget, selectedTarget->getAttackablePosition());
        
    }
    
    selectedTarget->actWhenAttacked(static_cast<AttackableObject*>(this->getParent()));

}

void BuildingAttackComponent::makeSingleTileDamage(CCNode* pBullet)
{
    CCPoint tilePos = MapHelper::groundMapPosToTilePos(pBullet->getPosition(), GameScene::getInstance()->sharedGameStage->getGroundMap(), false);
    CCArray* pEnemies = GameScene::getInstance()->sharedGameStage->enemies;
    CCObject* pObj;
    Building* pBuilding = static_cast<Building*>(this->getParent());

    CCPoint explosionPos = MapHelper::tilePosToGroundMapPos(ccp((float)tilePos.x + 0.5, (float)tilePos.y + 0.5));
    
    switch (pBuilding->getId())
    {
        case 1:
            Explosion::create("hit_effect_dusty", explosionPos);
            break;
        case 5:
            Explosion::create("hit_effect_explosion1", explosionPos);
            break;
        case 11:
            Explosion::create("hit_effect_rocket_explosion", explosionPos);
        default:
            break;
    }
    

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
            pEnemy->reduceHp(pBuilding->getDamage());
            
            if (pEnemy->isDead())
            {
                pEnemy->setKilledByExplosion(true);
            }
            else
            {
                pEnemy->actWhenAttacked(static_cast<AttackableObject*>(this->getParent()));
                
                if (rand()%100 < 60)
                {
                    pEnemy->setStatus(ENUM_BUFF_STATUS_DIZZY, pBuilding->getLevel(), 1 + pBuilding->getLevel() * 0.5);
                }

            }
            
        }
    }
}

bool BuildingAttackComponent::bulletHitTarget(const cocos2d::CCPoint &bulletPos, const cocos2d::CCPoint &targetPos)
{
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint tilePosBullet = MapHelper::groundMapPosToTilePos(bulletPos, pMap, true);
    CCPoint tilePosTarget = MapHelper::groundMapPosToTilePos(targetPos, pMap, true);
    
    CCPoint diff = ccpSub(tilePosBullet, tilePosTarget);
    
    diff.x = diff.x < 0 ? -diff.x : diff.x;
    diff.y = diff.y < 0 ? -diff.y : diff.y;
    
    // enemy is hit if distance is less than 1 tile
    return diff.x < 1.0f && diff.y < 1.0f;
    
    /*
     float xdiff = bulletPos.x - targetPos.x;
     float ydiff = bulletPos.y - targetPos.y;
     float distance = sqrt(xdiff*xdiff + ydiff*ydiff);
     
     CCSize tileSize = GameScene::getInstance()->sharedGameStage->getGroundMap()->getTileSize();
     
     return distance <= tileSize.height / 2;
     */
}

void BuildingAttackComponent::removeBullet(CCNode* pBullet)
{
    if (!pBullet)
    {
        return;
    }

    if (pBullet->getUserData())
    {
        delete pBullet->getUserData();
        pBullet->setUserData(NULL);
    }
    
    pBullet->stopAllActions();
    GameStage* pStage = GameScene::getInstance()->sharedGameStage;
    pStage->getGroundMap()->removeChild(pBullet, true);
    
    bullets->removeObject(pBullet);
}
