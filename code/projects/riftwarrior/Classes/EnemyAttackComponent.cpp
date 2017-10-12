    //
//  EnemyAttackComponent.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 12/17/12.
//
//

#include "EnemyAttackComponent.h"
#include "GameScene.h"
#include "GameData.h"
#include "SimpleAudioEngine.h"
#include "AnimationManager.h"
#include "MapHelper.h"
#include "Function.h"
#include "Explosion.h"
#include "Player.h"
#include "SmartAnimation.h"
#include "MapWinningCondition.h"


EnemyAttackComponent::~EnemyAttackComponent()
{
    if (m_Bullets)
    {
        delete m_Bullets;
    }
    
    if (m_pHitSprite)
    {
        m_pHitSprite->stopAllActions();
        if (m_pHitSprite->getParent() != NULL)
        {
            m_pHitSprite->removeFromParentAndCleanup(true);
        }
        
        m_pHitSprite->release();
    }
    
    if (m_pHitAction)
    {
        m_pHitAction->release();
    }
}

bool EnemyAttackComponent::init()
{
    m_Bullets = new CCArray();
    
    m_Quadrant = 0;
    m_attackMoveFinished = true;
    setupAnimation(1);
    selectedTarget = NULL;
    
    m_RandSeed = rand();
    m_IdleTime = 10;
    //    this->scheduleUpdate();
    return true;
}

void EnemyAttackComponent::setupAnimation(int id)
{
    // setup hit effect animation
    /*
    m_pHitSprite = CCSprite::createWithSpriteFrameName("hit_effect_1_1.png");
    m_pHitSprite->retain();
    m_pHitSprite->setOpacity(128);
    
    CCArray* frames = CCArray::createWithCapacity(2);
    
    char buffer[128] = {0};
    const int maxFrameNumber = 8;
    
    for(int i=0;i<maxFrameNumber;++i)
    {
        sprintf(buffer, "hit_effect_%d_%d.png", 1,i+1);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buffer);
        if (!frame)
        {
            break;
        }
        
        frames->addObject(frame);
    }
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(frames, 0.1f/frames->count());
    
    CCAnimate* animate = CCAnimate::create(animation);
    animate->retain();
    
    m_pHitAction = animate;
    */
}

bool EnemyAttackComponent::trySetTarget(AttackableObject *pTarget)
{
    if (!m_attackMoveFinished || selectedTarget)
    {
        return false;
    }
    
    Enemy* enemy = static_cast<Enemy*>(getParent());
    
    if (pTarget == Player::getInstance() && enemy->getCurrentTeam() == ENUM_TEAM_PLAYER)
    {
        return false;
    }
    
    if (pTarget == GameScene::getInstance()->sharedGameStage->getComputerPlayer() && enemy->getCurrentTeam() == ENUM_TEAM_COMPUTER)
    {
        return false;
    }
    
    if (!pTarget->isActive() ||
        pTarget->isDead() ||
        !isWithinAttackRange(pTarget->getAttackableMapPosition()))
    {
        return false;
    }
    
    
    selectedTarget = pTarget;
    selectedTarget->retain();
//    Enemy* enemy = static_cast<Enemy*>(this->getParent());
//    enemy->getAnimationSprite()->stopAllActions();
    return true;
}

bool EnemyAttackComponent::hasTarget()
{
    return selectedTarget != NULL;
}

AttackableObject* EnemyAttackComponent::findTarget()
{
    
    AttackableObject* target = NULL;
    
    Enemy* enemy = static_cast<Enemy*>(this->getParent());
    CCPoint currentMapPos = MapHelper::isoMapPosToOrthMapPos(enemy->getShootPosition());
    
    float rangePowered = pow(enemy->getAttackRange(), 2);

    const MapWinningCondition* pWinningCondition = GameScene::getInstance()->sharedGameStage->getWinningCondition();
    
    if (pWinningCondition->killPlayer)
    {
        AttackableObject* pPlayer = static_cast<AttackableObject*>(Player::getInstance());
        if (!pPlayer->isDead() &&
            AttackComponent::isWithinAttackRange(currentMapPos, pPlayer->getAttackableMapPosition(), enemy->getAttackRange(), rangePowered))
        {
            return pPlayer;
        }
    }
    
    if (pWinningCondition->killNpcId > 0)
    {
        AttackableObject* pNpc = GameScene::getInstance()->sharedGameStage->getNpc(pWinningCondition->killNpcId);
        
        if (pNpc->isActive() &&
            !pNpc->isDead() &&
            pNpc->getId() == pWinningCondition->killNpcId &&
            AttackComponent::isWithinAttackRange(currentMapPos, pNpc->getAttackableMapPosition(), enemy->getAttackRange(), rangePowered))
        {
            return pNpc;
        }
    }
    
    /*
    if (GameScene::getInstance()->sharedGameStage->getComputerPlayer())
    {
        AttackableObject* pComputerPlayer = GameScene::getInstance()->sharedGameStage->getComputerPlayer();
        if (enemy != pComputerPlayer)
        {
            if (isWithinAttackRange(pComputerPlayer->getAttackableMapPosition()))
            {
                return pComputerPlayer;
            }

        }
    }
    
    */
    
    if (Building* pPlayerBase = GameScene::getInstance()->sharedGameStage->getPlayerBase())
    {
        if (isWithinAttackRange(pPlayerBase->getAttackableMapPosition()))
        {
            return pPlayerBase;
        }
    }
    
    CCObject* pTarget = NULL;
    CCArray* units = GameScene::getInstance()->sharedGameStage->units;
    CCARRAY_FOREACH(units, pTarget)
    {
        Unit* unit = static_cast<Unit*>(pTarget);
        
        if (!unit->isActive() || unit->isDead())
        {
            continue;
        }
        
        CCPoint targetMapPos = unit->getCurrentMapPosition();
        
        if (isWithinAttackRange(targetMapPos))
        {
            return unit;
        }
        
    }
    
    int possibility = enemy->getAttackRange() < MapHelper::MAP_TILE_LENGTH * 1.5 ? 25 : 15;
    int seconds = time(NULL);
    
    if (((m_RandSeed + seconds)%100)<possibility)
    {
        CCPoint currentPos = enemy->getCurrentMapPosition();
        CCPoint currentTile = enemy->getCurrentTilePosition();
        
        float range = enemy->getAttackRange();
        int tileRange = 1 + range/MapHelper::MAP_TILE_LENGTH;
        
        BuildingManager *pBuildingManager = GameScene::getInstance()->sharedGameStage->getBuildingManager();
    
        vector<Building*> attackableBuildings;
        for (int i = currentTile.y - tileRange;i<=currentTile.y + tileRange;++i)
        {
            for (int j=currentTile.x - tileRange; j<currentTile.x + tileRange;++j)
            {
                Building* pBuilding = pBuildingManager->getBuilding(j, i);
                if (!pBuilding || GameData::getBuildingSetting(pBuilding->getId()).type == ENUM_BUILDING_ASSIST)
                {
                    continue;
                }
                
                if (isWithinAttackRange(pBuilding->getAttackableMapPosition()))
                {
                    attackableBuildings.push_back(pBuilding);
                }
            }
        }
        
        if (attackableBuildings.size()>0)
        {
            return attackableBuildings[rand()%attackableBuildings.size()];
        }

    }
    


    return target;
}

void EnemyAttackComponent::update(float dt)
{
    CCPoint targetPos = CCPointZero;
    

    Enemy* currentOwner = static_cast<Enemy*>(getParent());
    if (!currentOwner->ableToAttack())
    {
        return;
    }
    
    if (m_attackMoveFinished && m_IdleTime < currentOwner->getEnemySetting()->attackDelay)
    {
        m_IdleTime += dt;
        return;
    }
    
    if (selectedTarget)
    {
        if (!selectedTarget->isActive() ||
            selectedTarget->isDead() ||
            !isWithinAttackRange(selectedTarget->getAttackableMapPosition()))
        {
           
            currentOwner->finishAttack();
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

bool EnemyAttackComponent::isWithinAttackRange(const cocos2d::CCPoint &targetMapPos)
{
    CCTMXTiledMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    Enemy* enemy = static_cast<Enemy*>(this->getParent());
    float attackRange = enemy->getAttackRange();
    
    CCPoint currentMapPos = MapHelper::isoMapPosToOrthMapPos(enemy->getSpritePosition());
    
    // if next to the next tile, return true
    /*
    CCPoint targetTilePos = MapHelper::mapPosToTilePos(targetMapPos, pMap);
    CCPoint currentTilePos = MapHelper::mapPosToTilePos(currentMapPos, pMap);
    if (abs((int)targetTilePos.x - (int)currentTilePos.x) <= 1 &&
        abs((int)targetTilePos.y - (int)currentTilePos.y) <=1)
    {
        return true;
    }
    */
    
//    const CCPoint& targetMapPos = MapHelper::isoMapPosToOrthMapPos(target);
 
    return AttackComponent::isWithinAttackRange(currentMapPos, targetMapPos, attackRange, attackRange * attackRange);
    
}


void EnemyAttackComponent::aimAtTarget(const cocos2d::CCPoint &target)
{
    /*
     if (rotatable)
     {
     const CCPoint& pos = getFireGroundPosition();
     CCPoint normalized = ccpNormalize(ccp(target.x - pos.x, target.y - pos.y));
     this->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x))+90);
     }
     */
//    shoot();

    if (!m_attackMoveFinished)
    {
        return;
    }
    
    AttackableObject* pOwner = static_cast<AttackableObject*>(this->getParent());

    const CCPoint& pos = pOwner->getShootPosition();
    
    float dx = target.x - pos.x;
    float dy = target.y - pos.y;
    CCPoint normalized = ccpNormalize(ccp(dx, dy));
    int quadrant = 0;
    
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
    
    
    if (m_Quadrant != quadrant)
    {
        m_Quadrant = quadrant;
    }
    

    m_attackMoveFinished = false;
    startAttack();
//    this->scheduleOnce(schedule_selector(EnemyAttackComponent::shoot), pEnemy->getAnimation()->getAnimationInterval());
}

void EnemyAttackComponent::startAttack()
{
    m_attackMoveFinished = true;
    float idleTime = m_IdleTime;
    m_IdleTime = 0;

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
    
    Enemy* pEnemy = static_cast<Enemy*>(this->getParent());
    
    if (selectedTarget->isDead())
    {
        Enemy* pEnemy = static_cast<Enemy*>(this->getParent());
        pEnemy->finishAttack();
        return;
    }
    
    m_attackMoveFinished = false;
    m_IdleTime = idleTime;
    
    EnemyAnimation* pAnimation = pEnemy->getAnimation();
    
    CCCallFuncO* callback = NULL;
    
    switch (pEnemy->getEnemySetting()->attackType) {
        case ENUM_ENEMY_ATTACK_MELEE:
        case ENUM_ENEMY_ATTACK_HALF_CIRCLE_MELEE:
        case ENUM_ENEMY_ATTACK_ROUND_AREA:
        case ENUM_ENEMY_ATTACK_SHOCK_WAVE:
            callback = CCCallFuncO::create(this, callfuncO_selector(EnemyAttackComponent::hurtTarget), NULL);;
            break;
        case ENUM_ENEMY_ATTACK_THROW_STONE:
        case ENUM_ENEMY_ATTACK_THROW_SPEAR:
        case ENUM_ENEMY_ATTACK_ARROW:
        case ENUM_ENEMY_ATTACK_SNAKE_VENOM:
        case ENUM_ENEMY_ATTACK_SPIDER_WEB:
            callback = CCCallFuncO::create(this, callfuncO_selector(EnemyAttackComponent::shootBullet), NULL);
            break;
        default:
            break;
    }

    
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

    pAnimation->setAnimationWithCallback(animationType, callback, 0);
    
    pAnimation->runAction(CCSequence::create(CCDelayTime::create(0.1f),
                                             CCCallFunc::create(this, callfunc_selector(EnemyAttackComponent::playAttackSound)),
                                             NULL));
    
    
    // play sound
    if (pEnemy->getEnemySetting()->attackType == ENUM_ENEMY_ATTACK_SHOCK_WAVE)
    {
        // FIXME
        //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/shockwave.mp3");
    }
    
//    hurtTarget(NULL);
}



void EnemyAttackComponent::playAttackSound()
{
    Enemy* pEnemy = static_cast<Enemy*>(getParent());
    char soundPath[128] = {0};
    sprintf(soundPath, "sound/enemy_%d_attack.mp3", pEnemy->getId());
    // FIXME
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundPath, false);
}

void EnemyAttackComponent::shootBullet(void* pObject)
{

    if (!selectedTarget)
    {
        return;
    }

    Enemy* pEnemy = static_cast<Enemy*>(this->getParent());
    
    if (pEnemy->isDead() || !pEnemy->isActive())
    {
        return;
    }
    
    CCPoint bulletPos = getBulletPositon();
    
    CCPoint targetPos = selectedTarget->getAttackablePosition();

//    CCNode* bullet = CCSprite::createWithTexture(Function::tryGetCachedTexture(bulletName));
    ENUM_BULLET_ANIMATION bulletAnimationType;
    float scale = 1;
    switch (pEnemy->getEnemySetting()->attackType) {
        case ENUM_ENEMY_ATTACK_THROW_STONE:
            bulletAnimationType = ENUM_BULLET_ANIMATION_ROCK;
            scale = 0.6f;
            break;
        case ENUM_ENEMY_ATTACK_THROW_SPEAR:
            bulletAnimationType = ENUM_BULLET_ANIMATION_SPEAR;
            break;
        case ENUM_ENEMY_ATTACK_ARROW:
            bulletAnimationType = ENUM_BULLET_ANIMATION_ARROW;
            scale = 0.25f;
            break;
        case ENUM_ENEMY_ATTACK_SNAKE_VENOM:
            bulletAnimationType = ENUM_BULLET_ANIMATION_SNAKE_VENOM;
            scale = 0.5f;
            break;
        case ENUM_ENEMY_ATTACK_SPIDER_WEB:
            bulletAnimationType = ENUM_BULLET_ANIMATION_SPIDER_WEB;
            scale = 0.3f;
            break;
        default:
            break;
    }
    
    CCNode* bullet = AnimationManager::getInstance()->createBulletAnimation(bulletAnimationType);
    bullet->setScale(scale);
    bullet->setPosition(bulletPos);
    
    CCPoint targetMapPos = selectedTarget->getAttackableMapPosition();
    CCPoint currentMapPos = pEnemy->getCurrentMapPosition();
    float dx = targetMapPos.x - currentMapPos.x;
    float dy = targetMapPos.y - currentMapPos.y;
    float distance = sqrt(dx*dx + dy*dy);
    
     //    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("shoot.wav");
    
    float duration = distance/pEnemy->getEnemySetting()->attackSpeed;

    
    CCPoint normalized = ccpNormalize(ccp(targetPos.x - bulletPos.x, targetPos.y - bulletPos.y));
    bullet->setRotation(CC_RADIANS_TO_DEGREES(atan2(normalized.y, -normalized.x))+180);
    
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(bullet);

    CCActionInterval* action = createAction(duration, targetPos);
    
    bullet->runAction(CCSequence::create(action, CCCallFuncO::create(this, callfuncO_selector(EnemyAttackComponent::hurtTarget), bullet), CCCallFuncO::create(this, callfuncO_selector(EnemyAttackComponent::removeBullet), bullet), NULL));
     
    
    m_Bullets->addObject(bullet);
    
    if (pEnemy->getEnemySetting()->attackDelay > 0)
    {
        m_attackMoveFinished = true;
        m_IdleTime = 0;
        pEnemy->resetIdleAnimation();
    }
        
}

CCActionInterval* EnemyAttackComponent::createAction(float duration, const CCPoint& targetPos)
{
    
    CCActionInterval*  action = CCMoveTo::create(duration, targetPos);

    return action;
}

CCPoint EnemyAttackComponent::getBulletPositon()
{
    Enemy* pEnemy = static_cast<Enemy*>(this->getParent());
    
    CCPoint bulletPos = pEnemy->getShootPosition();
    
    return bulletPos;
}


void EnemyAttackComponent::showExplosion(CCNode* pBullet)
{
    /*
    CCParticleSystemQuad* particle = NULL;

    particle = CCParticleSun::create();

    if (particle)
    {
        particle->retain();
        GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(particle, 10);

        char buffer[128]={0};
        sprintf(buffer, "particle/fire.png");
        particle->setTexture( Function::tryGetCachedTexture(buffer) );
        particle->setDuration(0.5);
        particle->setScale(4);
        particle->setAutoRemoveOnFinish(true);
        particle->setPositionType(kCCPositionTypeGrouped);
        particle->setPosition(selectedTarget->getAttackablePosition());
        
    }
    */
}

void EnemyAttackComponent::hurtTarget(CCNode* pBullet)
{
    Enemy* pEnemy = static_cast<Enemy*>(this->getParent());
    
    if (!pEnemy)
    {
        return;
    }
    
    if (pBullet)
    {
        pBullet->stopAllActions();
    }
    
    char hitSoundName[128] = {0};
    sprintf(hitSoundName, "sound/enemy_%d_hit.wav", pEnemy->getId());
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(hitSoundName);
    
    switch (pEnemy->getEnemySetting()->attackType)
    {
        case ENUM_ENEMY_ATTACK_HALF_CIRCLE_MELEE:
            makeHalfCircleMeleeDamage();
            break;
        case ENUM_ENEMY_ATTACK_ROUND_AREA:
        {
            CCPoint targetPos = pEnemy->getAttackablePosition();
            if (selectedTarget)
            {
                targetPos = selectedTarget->getAttackablePosition();
            }
            
            if (pEnemy->getEnemySetting()->magicType == 1)
            {
                // special case for dragon king
                CCSmartSprite* pMagicAnimSprite = AnimationManager::getInstance()->createMagicAnimation(pEnemy->getEnemySetting()->magicType, 2);
                
                pMagicAnimSprite->setAnchorPoint(ccp(0.5,0.2));
                
                pMagicAnimSprite->setPosition(targetPos);
                
                GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(pMagicAnimSprite);
                
                scheduleOnce(schedule_selector(EnemyAttackComponent::makeRoundAreaDamage), 1.0);
            }
            else
            {
                makeRoundAreaDamage();
            }
        }
            break;
        case ENUM_ENEMY_ATTACK_SHOCK_WAVE:
        {
            CCSmartSprite* pAttackActAnimation = AnimationManager::getInstance()->getEnemySpecialAttackAnimation(pEnemy->getEnemySetting()->attackType);
            
            if (pAttackActAnimation)
            {
                EnemyAnimation* pAnimation = pEnemy->getAnimation();
                pAnimation->addChild(pAttackActAnimation);
            }

            makeRoundAreaDamage();
        }
            break;
            
        case ENUM_ENEMY_ATTACK_SNAKE_VENOM:
            makeSnakeVenomDamage(pBullet);
            break;
        case ENUM_ENEMY_ATTACK_SPIDER_WEB:
        {
            makeSingleTargetDamage(pBullet);
            
            /*
            if (selectedTarget)
            {
                selectedTarget->setStatus(ENUM_BUFF_STATUS_SPIDER_WEB, 1, 5);
            }
            */
            // for spider, just attack once with web
//            removeTarget();
        }
            
        default:
            makeSingleTargetDamage(pBullet);
            break;
    }

    
    if (pEnemy->getEnemySetting()->attackDelay == 0 ||
        pEnemy->getEnemySetting()->attackSpeed == 0)
    {
        // no delay attack or immediate attack are both melee attacks
        m_attackMoveFinished = true;
        m_IdleTime = 0;
        
        pEnemy->resetIdleAnimation();
    }

}

bool EnemyAttackComponent::isWithinRange(const cocos2d::CCPoint &centerMapPos, const cocos2d::CCPoint &targetMapPos)
{
    Enemy* enemy = static_cast<Enemy*>(this->getParent());
    float attackRange = enemy->getAttackRange();
    
    return AttackComponent::isWithinAttackRange(centerMapPos, targetMapPos, attackRange, attackRange * attackRange);
}

void EnemyAttackComponent::makeSingleTargetDamage(CCNode* pBullet)
{
    Enemy* pEnemy = static_cast<Enemy*>(this->getParent());
    if (!selectedTarget)
    {
        pEnemy->finishAttack();
        return;
    }
    
    if ((pBullet && bulletHitTarget(pBullet->getPosition(), selectedTarget->getAttackablePosition())) || !pBullet)
    {
        //            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/hit_building.wav");
        
        showExplosion(pBullet);
        
        /*
         if (m_pHitSprite->getParent() != NULL)
         {
         m_pHitSprite->removeFromParentAndCleanup(true);
         }
         
         selectedTarget->addChild(m_pHitSprite);
         m_pHitSprite->runAction(m_pHitAction);
         */
        
        selectedTarget->reduceHp(pEnemy->getDamage());
        
    }
    
    if (selectedTarget->isDead())
    {
        
        selectedTarget->destroy();
        
        pEnemy->finishAttack();

    }

}


void EnemyAttackComponent::makeSnakeVenomDamage(cocos2d::CCNode *pBullet)
{
    
    if (selectedTarget)
    {
        Enemy* pEnemy = static_cast<Enemy*>(getParent());
        selectedTarget->setStatus(ENUM_BUFF_STATUS_POISON, pEnemy->getDamage()/5, 2);
    }
    
    makeSingleTargetDamage(pBullet);
}

void EnemyAttackComponent::makeRoundAreaDamage()
{
    Enemy* pEnemy = static_cast<Enemy*>(this->getParent());
    
    CCPoint targetMapPos = pEnemy->getCurrentMapPosition();
    CCPoint targetTilePos = pEnemy->getCurrentTilePosition();
    
    if (selectedTarget)
    {
        if (selectedTarget == Player::getInstance())
        {
            targetTilePos = Player::getInstance()->getCurrentMapTile();
            targetMapPos = Player::getInstance()->getAttackableMapPosition();
        }
        else
        {
            // target is building
            Building* pBuilding = static_cast<Building*>(selectedTarget);
            targetTilePos = pBuilding->getOriginTilePosition();
            targetMapPos = pBuilding->getAttackableMapPosition();
        }
    }
    
    float range = pEnemy->getAttackRange();
    int tileRange = 1 + range/MapHelper::MAP_TILE_LENGTH;

    
    BuildingManager *pBuildingManager = GameScene::getInstance()->sharedGameStage->getBuildingManager();

    for (int i = targetTilePos.y-tileRange;i<=targetTilePos.y+tileRange;++i)
    {
        for (int j=targetTilePos.x-tileRange; j<targetTilePos.x+tileRange;++j)
        {
            Building* pBuilding = pBuildingManager->getBuilding(j, i);
            if (!pBuilding)
            {
                continue;
            }
            
            if (isWithinRange(targetMapPos, pBuilding->getAttackableMapPosition()))
            {
                pBuilding->reduceHp(pEnemy->getDamage());
                if (pBuilding->isDead())
                {
                    pBuilding->destroy();
                }
            }
        }
    }
    
    if (isWithinAttackRange(Player::getInstance()->getAttackableMapPosition()))
    {
        Player::getInstance()->reduceHp(pEnemy->getDamage());
        if (Player::getInstance()->isDead())
        {
            Player::getInstance()->destroy();
        }
    }

    
    if (selectedTarget && selectedTarget->isDead())
    {
        selectedTarget->destroy();
        
        pEnemy->finishAttack();
    }
    else if (!selectedTarget)
    {
        pEnemy->finishAttack();
    }
}

void EnemyAttackComponent::makeHalfCircleMeleeDamage()
{
    Enemy* pEnemy = static_cast<Enemy*>(this->getParent());
    CCPoint currentTile = pEnemy->getCurrentTilePosition();
    
    int x = currentTile.x;
    int y = currentTile.y;
    
    vector< vector<CCPoint> > neighbors;

    neighbors.resize(5);
    for(int i=0; i<5; ++i)
    {
        neighbors[i].resize(5);
    }
    
    for (int i=0;i<5;++i)
    {
        for (int j=0;j<5;++j)
        {
            
            neighbors[i][j] = ccp(x + j - 2, y+ i - 2);
            
        }
    }
    
 
    vector<CCPoint> attackableTiles;
    attackableTiles.reserve(25);

    switch(m_Quadrant)
    {
        case 1:
        {
            for(int i=0;i<5;++i)
            {
                for (int j=2;j<5;++j)
                {
                    attackableTiles.push_back(neighbors[i][j]);
                }
            }
        }
            break;
        case 2:
        {
            for(int i=0;i<5;++i)
            {
                for (int j=i;j<5;++j)
                {
                    attackableTiles.push_back(neighbors[i][j]);
                }
            }
        }
            break;
        case 3:
        {
            for(int i=0;i<3;++i)
            {
                for (int j=0;j<5;++j)
                {
                    attackableTiles.push_back(neighbors[i][j]);
                }
            }
        }
            break;
        case 4:
        {
            for(int i=0;i<5;++i)
            {
                for (int j=0;j<5-i;++j)
                {
                    attackableTiles.push_back(neighbors[i][j]);
                }
            }
        }
            break;
        case 5:
        {
            for(int i=0;i<5;++i)
            {
                for (int j=0;j<3;++j)
                {
                    attackableTiles.push_back(neighbors[i][j]);
                }
            }
        }
            break;
        case 6:
        {
            for(int i=0;i<5;++i)
            {
                for (int j=0;j<i+1;++j)
                {
                    attackableTiles.push_back(neighbors[i][j]);
                }
            }
        }
            break;
        case 7:
        {
            for(int i=2;i<5;++i)
            {
                for (int j=0;j<5;++j)
                {
                    attackableTiles.push_back(neighbors[i][j]);
                }
            }
        }
            break;
        case 8:
        {
            for(int i=0;i<5;++i)
            {
                for (int j=4;j>=4-i;--j)
                {
                    attackableTiles.push_back(neighbors[i][j]);
                }
            }
        }
            break;
    }

    
 
    Player* pPlayer = Player::getInstance();

	for (const CCPoint& attackTile : attackableTiles)
    {
        // check if player is at the tile
        if ((int)pPlayer->getCurrentMapTile().x == (int)attackTile.x &&
            (int)pPlayer->getCurrentMapTile().y == (int)attackTile.y)
        {
            pPlayer->reduceHp(pEnemy->getDamage());
            if (pPlayer->isDead())
            {
                
                pPlayer->destroy();
            }
            
        }

        // check player buildings
        Building* pBuilding = GameScene::getInstance()->sharedGameStage->getBuildingManager()->getBuilding((int)attackTile.x, (int)attackTile.y);
        if (pBuilding && pBuilding->isPlayerBuilding())
        {
            pBuilding->reduceHp(pEnemy->getDamage());
            if (pBuilding->isDead())
            {
                pBuilding->destroy();
            }
        }
        
        
        
        // attack player units
        CCArray* targets = pEnemy->getCurrentTeam() == ENUM_TEAM_COMPUTER ? GameScene::getInstance()->sharedGameStage->units :
                                                                            GameScene::getInstance()->sharedGameStage->enemies;
        
        
        CCObject* pTarget = NULL;
        CCARRAY_FOREACH(targets, pTarget)
        {
            Enemy* enemy = static_cast<Enemy*>(pTarget);
            
            if (!enemy->isActive() || enemy->isDead())
            {
                continue;
            }
            
            CCPoint enemyTile = enemy->getCurrentTilePosition();
            
            if ((int)enemyTile.x == (int)attackTile.x && (int)enemyTile.y == (int)attackTile.y)
            {
                enemy->reduceHp(pEnemy->getDamage());
                
            }
        }
        
        
    }
    
    if (selectedTarget && selectedTarget->isDead())
    {
        selectedTarget->destroy();
        
        pEnemy->finishAttack();
    }
    else if (!selectedTarget)
    {
        pEnemy->finishAttack();
    }

    
}

bool EnemyAttackComponent::bulletHitTarget(const cocos2d::CCPoint &bulletPos, const cocos2d::CCPoint &targetPos)
{
    float xdiff = bulletPos.x - targetPos.x;
    float ydiff = bulletPos.y - targetPos.y;
    float distance = sqrt(xdiff*xdiff + ydiff*ydiff);
    
    CCSize tileSize = GameScene::getInstance()->sharedGameStage->getGroundMap()->getTileSize();
    
    return distance <= tileSize.height / 2;
    
}

void EnemyAttackComponent::removeBullet(CCNode* pBullet)
{
    pBullet->removeFromParentAndCleanup(true);
    
    m_Bullets->removeObject(pBullet);
}


void EnemyAttackComponent::removeTarget()
{
    {
        m_attackMoveFinished = true;
        if (selectedTarget)
        {
            selectedTarget->release();
            selectedTarget = NULL;
        }
        
        m_IdleTime = 0;
    }

}
