//
//  GameStage.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/5/12.
//
//

#include "GameStage.h"
#include "MapHelper.h"
#include "DebugLayer.h"
#include "Player.h"
#include "BuildingManager.h"
#include "BuildingMenu.h"
#include "StageShadowLayer.h"
#include "Function.h"
#include "GameScene.h"
#include "GameData.h"
#include "SkillTouchAnimation.h"
#include "SimpleAudioEngine.h"
#include "EnemyCache.h"
#include "SpecialActions.h"
#include "common.h"
#include "NpcMessageBox.h"
#include "AnimationManager.h"
#include "SystemHelper.h"
#include "Npc.h"

#include <unordered_set>

using namespace CocosDenshion;
using namespace std;

#define UNIT_MOVE_DESTINATION_TAG 6001

GameStage* GameStage::create(int mapId)
{
    GameStage* pStage = new GameStage();
    if (pStage && pStage->init(mapId))
    {
        pStage->autorelease();
        return pStage;
    }
    
    CC_SAFE_DELETE(pStage);
    return NULL;
}

bool GameStage::init(int mapId)
{
    if (CCLayer::init())
    {
        m_MapId = mapId;
        m_pSkillTouchAnimation = NULL;
        pBuildingManager = NULL;
        setAccelerometerEnabled(true);
        enemies = nullptr;
        units = nullptr;
        m_Npcs = nullptr;
        pGroundMap = nullptr;
        return true;
    }
    
    return false;
}

void GameStage::clear()
{
    unscheduleAllSelectors();
    unscheduleUpdate();

    if (pBuildingManager)
    {
        delete pBuildingManager;
        pBuildingManager = NULL;
    }
    
    CCNode* pMap = getChildByTag(GroundMapNodeTag);
    if (pMap)
    {
        // check if map has been attached;
        pMap->removeAllChildrenWithCleanup(true);
        this->removeChild(pMap, true);
        pGroundMap->release();
        pGroundMap = NULL;
    }

    this->removeChildByTag(GroundMapBuildingLayerTag, true);
    
    Player::getInstance()->remove();

    if (enemies)
    {
        if (enemies->count() > 0)
        {
            enemies->removeAllObjects();
        }
        delete enemies;
        enemies = NULL;
    }
    
    if (units)
    {
        if (units->count()>0)
        {
            units->removeAllObjects();
        }
        
        delete units;
        units = NULL;
    }
    
    if (m_Npcs)
    {
        if (m_Npcs->count()>0)
        {
            m_Npcs->removeAllObjects();
        }
        
        delete m_Npcs;
        m_Npcs = NULL;
    }
    
    if (m_pSkillTouchAnimation)
    {
        this->removeChild(m_pSkillTouchAnimation, true);
        m_pSkillTouchAnimation = NULL;
    }

    m_CurrentWave = 0;
    
    while (npcMessageQueue.size() > 0)
    {
        npcMessageQueue.pop();
    }
    
    clearStageData();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    
    AnimationManager::getInstance()->removeEnvironmentAnimationFrames();


    
    m_TotalEnemyNumber = 0;
    m_StageReady = false;
    m_Paused = false;
    m_FollowPlayer = false;
    m_IsScrolling = false;
    
    m_MonsterKillerScore = 0;
}

void GameStage::reset()
{
    clear();
    
    srand(time(NULL));
    
    cacheStageData();
    
    AnimationManager::getInstance()->initEnvironmentAnimationFrames();
    
    m_PassedEnemies = 0;
    m_TotalStageRunningTime = 0;
    
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.3f);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.3f);

    GameScene::getInstance()->sharedMainUI->initStageUI();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    if (pGroundMap == NULL)
    {
        char mapName[128] = {0};
        sprintf(mapName, "tilemap/%d.tmx", m_MapId);
        
        pGroundMap = StageMap::create(mapName);
        pGroundMap->retain();
        this->addChild(pGroundMap, -1, GroundMapNodeTag);
    }

    Player::getInstance()->load();
    
    GameScene::getInstance()->sharedMainUI->getStageUI()->updatePlayerMoveMenu();
    
    CCPoint minPos = MapHelper::tilePosToGroundMapPos(pGroundMap->getMapMinTilePosition());
    CCPoint maxPos = MapHelper::tilePosToGroundMapPos(pGroundMap->getMapMaxTilePosition());
    // left bottom is [minPos.x, maxPos.y]
    m_Boundary.setRect(minPos.x, maxPos.y, maxPos.x - minPos.x, -(maxPos.y - minPos.y));
    m_MapCenterPos = ccp(m_Boundary.getMinX() + (m_Boundary.getMaxX() - m_Boundary.getMinX())/2, m_Boundary.getMinY() + (m_Boundary.getMaxY() - m_Boundary.getMinY())/2);
    
    // find the best scale to display the whole map
    CCSize mapSize = pGroundMap->getMapSize();

    CCPoint centerTilePos = ccp(mapSize.width/2, mapSize.height/2);
    pGroundMap->setAnchorPoint(ccp(0.5, 0.5));
    pGroundMap->setPosition(winSize.width/2, winSize.height/2);

    


    float minScaleWidth = winSize.width / m_Boundary.size.width;
    float minScaleHeight = winSize.height / m_Boundary.size.height;
    m_InitialMapScale = MAX(minScaleWidth, minScaleHeight);
    
    m_InitialMapScale = MAX(0.6, m_InitialMapScale);
    
    //    m_MinMapScale  = 0.9;
    //    m_MinMapScale *= Director::getInstance()->getWinSize().height / DESIGN_SCREEN_HEIGHT;
    
    
    pGroundMap->setScale(m_InitialMapScale);
    
    

    
    this->setTouchEnabled(true);
    
    
#ifndef DEBUG
#define DEBUG 0
#endif
    
    pBuildingManager = new BuildingManager(pGroundMap);
   
    enemies = new CCArray();
    
    units = new CCArray();
    
    m_Npcs = new CCDictionary();
    
    GameData::initNpcMessageForMap(m_MapId, &npcMessageQueue);
    
    m_pCurrentMapConfig = GameData::getMapConfig(m_MapId);

    setupWinningCondition();
    
    m_MaxWave = m_pCurrentMapConfig->enemies.size();
    
    // display player
    CCPoint tilePos = pGroundMap->getEndTilePosition();
    Player::getInstance()->showOnMap();

    createEnviromentEffect();
    
    this->scheduleUpdate();

    // stage open animation. Zoom in to player's location, then zoom out to whole map
    pGroundMap->setScale(m_InitialMapScale + 0.4);
    
    CCPoint endTile = pGroundMap->getEndTilePosition();
    CCPoint endMapPos = MapHelper::tilePosToGroundMapPos(endTile);
    CCPoint currentCenterPos = pGroundMap->convertToNodeSpace(ccp(winSize.width/2, winSize.height/2));
    float dx = endMapPos.x - currentCenterPos.x;
    float dy = endMapPos.y - currentCenterPos.y;
    CCPoint currentStageMapPos = pGroundMap->getPosition();
    CCPoint target = ccp(currentStageMapPos.x - dx, currentStageMapPos.y - dy);
    
    pGroundMap->setPosition(target);
    
    alignMapBoundary();
    
    showNextNpcMessage();
    
    
    if (m_pCurrentMapConfig->environment == ENUM_MAP_ENVIRONMENT_RAIN)
    {
        CCParticleSystemQuad* particle = CCParticleRain::create();
        particle->setTotalParticles(300);

        if (particle)
        {
            particle->retain();
            char buffer[128]={0};
            sprintf(buffer, "particle/%s.png", "raindrop");
            
            CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->textureForKey(buffer);
            if (!pTexture)
            {
                pTexture = CCTextureCache::sharedTextureCache()->addImage(buffer);
            }
            
            particle->setTexture( pTexture );
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            particle->setDuration(-1);
            particle->setStartSize(8.0f);
            particle->setGravity(ccp(10, -100));
            particle->setSpeed(200);
            particle->setSpeedVar(30);
            particle->setAutoRemoveOnFinish(true);
            particle->setPosition(ccp(winSize.width/2, winSize.height));
            particle->setPositionType(kCCPositionTypeGrouped);
            GameScene::getInstance()->sharedGameStage->addChild(particle, 128);
            
        }
        
    }
    else if (m_pCurrentMapConfig->environment == ENUM_MAP_ENVIRONMENT_BLOOD_RAIN)
    {
        CCParticleSystemQuad* particle = CCParticleRain::create();
        particle->setTotalParticles(2000);
        
        if (particle)
        {
            particle->retain();
            char buffer[128]={0};
            sprintf(buffer, "particle/%s.png", "base");
            
            CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->textureForKey(buffer);
            if (!pTexture)
            {
                pTexture = CCTextureCache::sharedTextureCache()->addImage(buffer);
            }
            
            particle->setTexture( pTexture );
            particle->setEmissionRate(1000);
            particle->setAngle(0.0);
            particle->setAngleVar(5.0);
            ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
            particle->setBlendFunc(blendFunc);
            particle->setDuration(1.0);
            particle->setEmitterMode(kCCParticleModeGravity);
            
            ccColor4F startColor={1.0, 0.0, 0.0, 1.0};
            particle->setStartColor(startColor);
            particle->setStartColorVar(ccc4f(0, 0, 0, 0));
            
            particle->setEndColor(ccc4f(1,1,1,0));
            particle->setEndColorVar(ccc4f(0,0,0,0));
            
            particle->setStartSize(10);
            particle->setStartSizeVar(5);
            particle->setEndSize(10);
            particle->setEndSizeVar(5);
            
            particle->setGravity(ccp(50, -100));
            particle->setRadialAccel(0);
            particle->setRadialAccelVar(1);
            
            particle->setSpeed(250);
            particle->setSpeedVar(220);
            
            particle->setTangentialAccel(0);
            particle->setTangentialAccelVar(1);
            
            particle->setLife(2.0);
            particle->setLifeVar(15);
            
            particle->setStartSpin(0);
            particle->setStartSpinVar(0);
            particle->setEndSpin(0);
            particle->setEndSpinVar(0);
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            particle->setAutoRemoveOnFinish(true);

            particle->setPosition(ccp(-winSize.width/2, winSize.height));
            particle->setPosVar(ccp(20, 200));
            particle->setDuration(-1);

            GameScene::getInstance()->sharedGameStage->addChild(particle, 128);
            
        }
    }
    else if (m_pCurrentMapConfig->environment == ENUM_MAP_ENVIRONMENT_SANDY_WIND)
    {
        CCParticleSystemQuad* particle = CCParticleRain::create();
        particle->setTotalParticles(2000);
        
        if (particle)
        {
            particle->retain();
            char buffer[128]={0};
            sprintf(buffer, "particle/%s.png", "base");
            
            CCTexture2D* pTexture = CCTextureCache::sharedTextureCache()->textureForKey(buffer);
            if (!pTexture)
            {
                pTexture = CCTextureCache::sharedTextureCache()->addImage(buffer);
            }
            
            particle->setTexture( pTexture );
            particle->setEmissionRate(1000);
            particle->setAngle(0.0);
            particle->setAngleVar(5.0);
            ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
            particle->setBlendFunc(blendFunc);
            particle->setDuration(1.0);
            particle->setEmitterMode(kCCParticleModeGravity);
            
            ccColor4F startColor={1.0f, 0.72f, 0.44f, 1.0f};
            particle->setStartColor(startColor);
            particle->setStartColorVar(ccc4f(0, 0, 0, 0));
            
            particle->setEndColor(ccc4f(1,1,1,0));
            particle->setEndColorVar(ccc4f(0,0,0,0));
            
            particle->setStartSize(10);
            particle->setStartSizeVar(5);
            particle->setEndSize(10);
            particle->setEndSizeVar(5);
            
            particle->setGravity(ccp(50, -100));
            particle->setRadialAccel(0);
            particle->setRadialAccelVar(1);
            
            particle->setSpeed(250);
            particle->setSpeedVar(220);
            
            particle->setTangentialAccel(0);
            particle->setTangentialAccelVar(1);
            
            particle->setLife(2.0);
            particle->setLifeVar(15);
            
            particle->setStartSpin(0);
            particle->setStartSpinVar(0);
            particle->setEndSpin(0);
            particle->setEndSpinVar(0);
            
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            particle->setAutoRemoveOnFinish(true);
            
            particle->setPosition(ccp(-winSize.width/2, winSize.height));
            particle->setPosVar(ccp(20, 200));
            particle->setDuration(-1);
            
            GameScene::getInstance()->sharedGameStage->addChild(particle, 128);
            
        }
    }

    
    //    start();
}

void GameStage::alignMapBoundary()
{
    if (isOutOfMapBoundary())
    {
        const int offset=0;
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint topLeft = pGroundMap->convertToNodeSpace(ccp(0,winSize.height));
        CCPoint bottomRight = pGroundMap->convertToNodeSpace(ccp(winSize.width, 0));
        
        CCPoint newPos = pGroundMap->getPosition();
        
        if (m_Boundary.getMaxX() <= bottomRight.x)
        {
            //            offset.x = bottomRight.x - m_Boundary.getMaxX() -5;
            newPos.x += ceil((bottomRight.x - m_Boundary.getMaxX()+offset));
        }
        else if (m_Boundary.getMinX()>= topLeft.x)
        {
            //            offset.x = topLeft.x - m_Boundary.getMinX() + 5;
            newPos.x -= ceil((m_Boundary.getMinX() - topLeft.x + offset));
        }
        
        if (m_Boundary.getMinY() >= bottomRight.y)
        {
            //            offset.y = bottomRight.y - m_Boundary.getMinY() - 5;
            newPos.y -= ceil(m_Boundary.getMinY() - bottomRight.y + offset);
        }
        else if (m_Boundary.getMaxY() <= topLeft.y)
        {
            //            offset.y = topLeft.y - m_Boundary.getMaxY() + 5;
            newPos.y += ceil(topLeft.y - m_Boundary.getMaxY() + offset);
        }
        
        //        CCPoint newPos = ccpAdd(target, offset);
        
        if (newPos.x != pGroundMap->getPosition().x || newPos.y != pGroundMap->getPosition().y)
        {
            
            pGroundMap->setPosition(ccp((int)newPos.x, (int)newPos.y));

        }
        

    }
}

void GameStage::showNextNpcMessage()
{
    if (!Player::getInstance()->hasCompletedMap(m_MapId))
    {
        // show opening NPC message
        if (npcMessageQueue.size() > 0)
        {
            MapNpcMessage message = npcMessageQueue.front();
            
            if (message.wave <= m_CurrentWave)
            {
                npcMessageQueue.pop();
                StageUI* pStageUI = GameScene::getInstance()->sharedMainUI->getStageUI();
                NpcMessageBox* npcMessageBox = pStageUI->getNpcMessageBox();
                npcMessageBox->showMessage(message.npcImage.c_str(), message.message.c_str());
                npcMessageBox->setCallback(CCCallFunc::create(this, callfunc_selector(GameStage::showNextNpcMessage)));
                
                if (message.tileX != 0 && message.tileY != 0)
                {

                    NPC* npc = NPC::create(4);
                    pGroundMap->addChild(npc);
                    npc->showOnMap(ccp(message.tileX, message.tileY));
                    
                    m_Npcs->setObject(npc, npc->getId());
                    
                }
                
                return;
            }
        }
        
    }

	schedule(schedule_selector(GameStage::showOpeningAnimation), 0.1f);
}

void GameStage::showOpeningAnimation()
{
    static int intervalCounter = 0;
    
    intervalCounter++;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    Player* pPlayer = Player::getInstance();
    
    if (!pPlayer->hasCompletedTutorial() &&
        !(pPlayer->getProgressFlags() & ENUM_FIRST_TIME_ON_STAGE))
    {
        // special case for tutorial. Otherwise tap will be blocked
        m_StageReady = true;

        if (((pPlayer->getProgressFlags() & ENUM_FIRST_TIME_MOVE_PLAYER) == 0) &&
            pPlayer->isStanding() &&
            !pPlayer->isAtTile(pGroundMap->getFirstTimeMoveTarget()))
        {
            GameScene::getInstance()->sharedMainUI->getStageUI()->showTutorialInstruction(ENUM_FIRST_TIME_MOVE_PLAYER);
            moveToWithinBoundary(
				pGroundMap->getFirstTimeMoveTarget(), 
				CCCallFunc::create(this, callfunc_selector(GameStage::showOpeningAnimation)));
            return;
        }
        else if (((pPlayer->getProgressFlags() & ENUM_FIRST_TIME_MOVE_PLAYER) == 0) &&
                 pPlayer->isStanding() &&
                 pPlayer->isAtTile(pGroundMap->getFirstTimeMoveTarget()))
        {
            GameScene::getInstance()->sharedMainUI->getStageUI()->hideNpcMessage();
            GameScene::getInstance()->sharedMainUI->getStageUI()->hideMessageBox();
            
            CCPoint currentCenterPos = pGroundMap->convertToNodeSpace(ccp(winSize.width/2, winSize.height/2));
            
            pGroundMap->runAction(CCSequence::create(CCScaleTo::create(1, m_InitialMapScale + 0.1),
                                                     CCDelayTime::create(0.5),
                                                     CCScaleTo::create(1, m_InitialMapScale+0.3),
                                                     NULL));
            
            CCPoint startTile = pGroundMap->getStartTilePositions()[0];
            CCPoint startPos = MapHelper::tilePosToGroundMapPos(startTile);
            float dx = startPos.x - currentCenterPos.x;
            float dy = startPos.y - currentCenterPos.y;
            
            CCPoint currentStageMapPos = pGroundMap->getPosition();
            CCPoint target = ccp(currentStageMapPos.x - dx, currentStageMapPos.y - dy);
            pGroundMap->runAction(CCSequence::create(CCDelayTime::create(1.5),
                                                     CCMoveToWithBoundary::create(2, target, m_Boundary),
//                                                     CCCallFunc::create(this, callfunc_selector(GameStage::start)),
                                                     NULL));
            Player::getInstance()->setProgressFlag(ENUM_FIRST_TIME_MOVE_PLAYER);
            
            GameScene::getInstance()->sharedMainUI->getStageUI()->removeInstructions();
            GameScene::getInstance()->sharedMainUI->getStageUI()->removeArrow();
            GameScene::getInstance()->sharedMainUI->getStageUI()->showFirstTimeOnStageDone();

        }
        else if (!pPlayer->isStanding())
        {
            if (intervalCounter==20)
            {
                pPlayer->showMessage(GameData::getText("tutorial_message_1"));
            }
            else if (intervalCounter==50)
            {
                GameScene::getInstance()->sharedMainUI->getStageUI()->hideNpcMessage();
            }
            else if (intervalCounter == 60)
            {
                pPlayer->showMessage(GameData::getText("tutorial_message_2"));
            }
            else if (intervalCounter==100)
            {
                GameScene::getInstance()->sharedMainUI->getStageUI()->hideNpcMessage();
            }
            
        }
        
    }
    else
    {
        unschedule(schedule_selector(GameStage::showOpeningAnimation));
        
        GameScene::getInstance()->sharedMainUI->getStageUI()->hideMessageBox();
        
         quickZoomOut(CCCallFunc::create(this, callfunc_selector(GameStage::showWinningCondition)));
    }
    
    
    return;

}

void GameStage::quickZoomOut(CCCallFunc* callback)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCPoint currentCenterPos = pGroundMap->convertToNodeSpace(ccp(winSize.width/2, winSize.height/2));
    
    pGroundMap->runAction(CCSequence::create(CCScaleTo::create(1.5, m_InitialMapScale + 0.05),
                                             NULL));
    
    CCSize mapSize = pGroundMap->getMapSize();
    
    CCPoint centerTilePos = ccp(mapSize.width/2, mapSize.height/2);
    
    CCPoint centerMapPos = MapHelper::tilePosToGroundMapPos(centerTilePos);
    float dx = centerMapPos.x - currentCenterPos.x;
    float dy = centerMapPos.y - currentCenterPos.y;
    
    CCPoint currentStageMapPos = pGroundMap->getPosition();
    CCPoint target = ccp(currentStageMapPos.x - dx, currentStageMapPos.y - dy);
    
    if (callback)
    {
        pGroundMap->runAction(CCSequence::create(CCMoveToWithBoundary::create(1.5, target, m_Boundary),
                                                 CCCallFunc::create(this, callfunc_selector(GameStage::alignMapBoundary)),
                                                 callback,
                                                 NULL));
    }
    else
    {
        pGroundMap->runAction(CCSequence::create(CCMoveToWithBoundary::create(1.5, target, m_Boundary),
                                                 CCCallFunc::create(this, callfunc_selector(GameStage::alignMapBoundary)),
                                                 NULL));
    }

}

void GameStage::setupWinningCondition()
{
    const MapWinningCondition* predefined = GameData::getMapWinningCondition(m_MapId);
    
    if (!Player::getInstance()->hasCompletedMap(m_MapId))
    {
        m_CurrentWinningCondition = *predefined;
        return;
    }
    
    m_CurrentWinningCondition.killPlayer = true;
    m_CurrentWinningCondition.killNpcId = 0;
    m_CurrentWinningCondition.minKilledEnemies = 0;
    m_CurrentWinningCondition.maxPassEnemies = 0;
    m_CurrentWinningCondition.maxTime = 0;

}

void GameStage::showWinningCondition()
{
    if (isOutOfMapBoundary())
    {
        CCLog("out of map boundary");
        alignMapBoundary();
    }
    
    GameScene::getInstance()->sharedMainUI->getStageUI()->showWinningCondition();
}

// move to tilePos and try to place tilePos at center
void GameStage::moveToWithinBoundary(const cocos2d::CCPoint &tilePos, CCCallFunc* pCallback)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    
    CCPoint currentCenterPos = pGroundMap->convertToNodeSpace(ccp(winSize.width/2, winSize.height/2));
    CCPoint destination = MapHelper::tilePosToGroundMapPos(tilePos);
    float dx = destination.x - currentCenterPos.x;
    float dy = destination.y - currentCenterPos.y;
    
    CCPoint currentStageMapPos = pGroundMap->getPosition();
    CCPoint target = ccp(currentStageMapPos.x - dx, currentStageMapPos.y - dy);
    
    if (pCallback)
    {
        pGroundMap->runAction(CCSequence::create(CCMoveToWithBoundary::create(5.0, target, getMapBoundary()),
                                                 pCallback,
                                                 NULL));
    }
    else
    {
        pGroundMap->runAction(CCSequence::create(CCMoveToWithBoundary::create(5.0, target, getMapBoundary()),
                                                 NULL));
    }

}

void GameStage::checkFollowPlayer()
{
    m_FollowPlayer = Player::getInstance()->isForceToMove();
}

void GameStage::scrollToPlayer(float time)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCPoint currentCenterPos = pGroundMap->convertToNodeSpace(ccp(winSize.width/2, winSize.height/2));
    CCPoint destination = Player::getInstance()->getAnimation()->getPosition();
    int dx = destination.x - currentCenterPos.x;
    int dy = destination.y - currentCenterPos.y;
    
    if (abs(dx)<1 && abs(dy)<1)
    {
        return;
    }
    
    CCPoint currentStageMapPos = pGroundMap->getPosition();
    CCPoint target = ccp((int)currentStageMapPos.x - dx, (int)currentStageMapPos.y - dy);

    Player::getInstance()->disableMove();
    m_FollowPlayer = false;
    pGroundMap->runAction(CCSequence::create(CCMoveToWithBoundary::create(time, target, getMapBoundary()),
                                             CCCallFunc::create(this, callfunc_selector(GameStage::checkFollowPlayer)),
                                             CCCallFunc::create(Player::getInstance(), callfunc_selector(Player::enableMove)),
                                             NULL));
    
    
    
}

void GameStage::followPlayer()
{
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCPoint currentCenterPos = pGroundMap->convertToNodeSpace(ccp(winSize.width/2, winSize.height/2));
    CCPoint destination = Player::getInstance()->getAnimation()->getPosition();
    int dx = destination.x - currentCenterPos.x;
    int dy = destination.y - currentCenterPos.y;
    
    int distance = abs(dx) + abs(dy);

    if (distance<1)
    {
        return;
    }
    
    CCPoint currentStageMapPos = pGroundMap->getPosition();
    CCPoint target = ccp((int)currentStageMapPos.x - dx, (int)currentStageMapPos.y - dy);
    
    pGroundMap->setPosition(target);
    
    alignMapBoundary();

}


void GameStage::setAnchorPointToStartPos()
{
    CCPoint startTile = pGroundMap->getStartTilePositions()[0];
    CCPoint startMapPos = MapHelper::tilePosToGroundMapPos(startTile);
    CCSize groundMapSize = pGroundMap->getContentSize();
    
    CCPoint anchorPoint = ccp(startMapPos.x/groundMapSize.width, startMapPos.y/groundMapSize.height);
    pGroundMap->setAnchorPoint(anchorPoint);
}

void GameStage::resetAnchorPoint()
{
    pGroundMap->setAnchorPoint(ccp(0.5,0.5));
}


void GameStage::createEnviromentEffect()
{
    
    const MapConfig* pMapConfig = GameData::getMapConfig(m_MapId);
    
    if (pMapConfig->environment == ENUM_MAP_ENVIRONMENT_UNDERSEA)
    {
        int number = 7 + rand()%5;
        
        for(int i=0;i<number;++i)
        {
            CCSmartSprite* bubble = CCSmartSprite::create("environment/undersea_bubble.png");
            bubble->setAnchorPoint(ccp(0.5, 0));
            bubble->setScale(0.1 + (float)(rand()%50)/100.0f);
            CCPoint moveFrom = ccp(m_Boundary.getMinX() + rand()%(int)(m_Boundary.getMaxX()-m_Boundary.getMinX()), m_Boundary.getMinY());
            CCPoint moveTo = ccp(moveFrom.x, m_Boundary.getMaxY());
            
            float duration = 5 + rand()%5;
            float delay = 2+rand()%5;
            
            bubble->runAction((CCActionInterval*)CCSequence::create(CCDelayTime::create(delay),
                                                                    CCMoveTo::create(0, moveFrom),
                                                                    CCMoveTo::create(duration, moveTo),
                                                                    CCCallFunc::create(bubble, callfunc_selector(CCSmartSprite::removeSelf)),
                                                                    NULL
                                                                    ));
            bubble->runAction((CCActionInterval*)CCSequence::create(CCDelayTime::create(delay),
                                                                    CCFadeIn::create(0.5),
                                                                    CCDelayTime::create(duration-1.5),
                                                                    CCFadeOut::create(1),
                                                                    NULL
                                                                    ));
            
            pGroundMap->addChild(bubble);
        }
    }
    
    runAction((CCActionInterval*)(CCSequence::create(CCDelayTime::create(rand()%10),
                                                     CCCallFunc::create(this, callfunc_selector(GameStage::createEnviromentEffect)),
                                                     NULL
                                                     )));
    
}

void GameStage::start()
{
    //flurry
    char eventName[128] = {0};
    sprintf(eventName, "play_map_%d", m_MapId);
    
    CCDictionary* params = CCDictionary::create();
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getLevel()), "player_level");
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getMoney()), "player_money");
    SystemHelper::logEvent(eventName, params, true);
    

    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(m_pCurrentMapConfig->music.c_str(), true);

    GameScene::getInstance()->sharedMainUI->getStageUI()->reset();
    
    GameScene::getInstance()->sharedMainUI->getStageUI()->showGameStatus();
    
    // FIXME: Added back from Like's comments removal change. Not verify the line below yet.
    SimpleAudioEngine::sharedEngine()->playEffect("sound/wave2.wav", true);
    
    m_TotalStageRunningTime = 0;
    
    if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_ON_STAGE))
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showTutorialInstruction(ENUM_FIRST_TIME_ON_STAGE);
    }
    else if (m_MapId == 2 &&
             !Player::getInstance()->hasCompletedMap(m_MapId))
    {
        MessageBox* pMessageBox = GameScene::getInstance()->sharedMainUI->getStageUI()->getMessageBox();
        pMessageBox->showTextMessage(GameData::getText("level_up_instruction"), true);
        pMessageBox->setCallback(CCCallFunc::create(this, callfunc_selector(GameStage::loadEnemies)));
        
    }
    else
    {
        scheduleOnce(schedule_selector(GameStage::loadEnemies), 1);
    }
    
    if (m_CurrentWinningCondition.killNpcId > 0)
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showNpcStatusMenu(m_CurrentWinningCondition.killNpcId);
    }

    
    
    m_StageReady = true;
    
    Player::getInstance()->updateNearbyBuildableTiles();
}

void GameStage::cacheStageData()
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    char plistPath[128]= {0};
    char pvrPath[128] = {0};
    sprintf(plistPath, "environment/map%d.plist", m_MapId);
    sprintf(pvrPath, "environment/map%d.pvr.ccz", m_MapId);

    if (CCFileUtils::sharedFileUtils()->isFileExist(plistPath))
    {
        frameCache->addSpriteFramesWithFile(plistPath, pvrPath);
    }

    
    vector< string > enemyValues = GameData::getMapConfig(m_MapId)->enemies;

    for (const string& enemies : enemyValues)
    {
        queue<pair<int, int>> enemyIds = parseEnemies(enemies);
        unordered_set<int> idTable;
        
        while (!enemyIds.empty())
        {
            ++m_TotalEnemyNumber;
            auto enemy = enemyIds.front();
            enemyIds.pop();
            int id = enemy.first;
            if (idTable.find(id) != idTable.end())
            {
                continue;
            }
            sprintf(plistPath, "enemy/%d/animation.plist",id);
            sprintf(pvrPath, "enemy/%d/animation.pvr.ccz",id);
            frameCache->addSpriteFramesWithFile(plistPath, pvrPath);

            char path[128] = {0};
            sprintf(path, "sound/enemy_%d_attack.mp3", id);
            // FIXME
            //CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(path);
            sprintf(path, "sound/enemy_%d_hit.mp3", id);
            // FIXME
            //CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(path);

            idTable.insert(id);
        }
    }

    sprintf(plistPath, "environment/map%d.plist", m_MapId);
    sprintf(pvrPath, "environment/map%d.pvr.ccz", m_MapId);
    
    sprintf(plistPath, "environment/map%d.plist", m_MapId);
    if (CCFileUtils::sharedFileUtils()->isFileExist(plistPath))
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath, pvrPath);
    }
    

    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/hit_building.wav");
}

void GameStage::clearStageData()
{
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    char plistPath[128]= {0};
    char pvrPath[128] = {0};
//    sprintf(plistPath, "environment/map%d.plist", m_MapId);
//    sprintf(pvrPath, "environment/map%d.pvr.ccz", m_MapId);
    
//    frameCache->removeSpriteFramesFromFile(plistPath);

    
    vector< string > enemyValues = GameData::getMapConfig(m_MapId)->enemies;
    
    for (const string& enemies : enemyValues)
    {
        auto enemyIds = parseEnemies(enemies);
        unordered_set<int> idTable;
        
        while (!enemyIds.empty())
        {
            auto enemy = enemyIds.front();
            enemyIds.pop();
            int id = enemy.first;
            if (idTable.find(id) != idTable.end())
            {
                continue;
            }

            sprintf(plistPath, "enemy/%d/animation.plist",id);
            sprintf(pvrPath, "enemy/%d/animation.pvr.ccz",id);
            frameCache->removeSpriteFramesFromFile(plistPath);
            
            char path[128] = {0};
            sprintf(path, "sound/enemy_%d_attack.wav", id);
            CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(path);
            sprintf(path, "sound/enemy_%d_hit.wav", id);
            CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(path);

            
            idTable.insert(id);
        }
    }
    
    
    sprintf(plistPath, "environment/map%d.plist", m_MapId);
    if (CCFileUtils::sharedFileUtils()->isFileExist(plistPath))
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(plistPath);
    }

    

    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect("sound/hit_building.wav");
    
}

void GameStage::loadEnemies()
{
    EnemyCache::clear();
    
    
    if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_PLACE_SNIPE_WEAPON) &&
        m_CurrentWave == 1)
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showTutorialInstruction(ENUM_FIRST_TIME_PLACE_SNIPE_WEAPON);
        return;
    }
    else if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON) &&
             m_CurrentWave == 2)
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showTutorialInstruction(ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON);
        return;
    }
    else if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_UPGRADE_WEAPON) &&
             m_CurrentWave >= 3 &&
             m_CurrentWave < m_MaxWave)
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showTutorialInstruction(ENUM_FIRST_TIME_UPGRADE_WEAPON);
        // continue to load enemies
    }
    else if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_UPGRADE_NEXT_GENERATION_WEAPON) &&
             m_CurrentWave >= 2 &&
             m_CurrentWave < m_MaxWave &&
             Player::getInstance()->canUseBuilding(6))  // building 6 is firegun
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->showTutorialInstruction(ENUM_FIRST_TIME_UPGRADE_NEXT_GENERATION_WEAPON);
        // continue to load enemies
    }


    m_CurrentWave++;
    
    loadCurrentWaveData();
    
}

void GameStage::completeStage()
{
    m_StageReady = false;
    pause();
    
    Player::getInstance()->completeMap(m_MapId);
    
    // save user data for each wave
    Player::getInstance()->save();
    
    unscheduleUpdate();
    
    GameScene::getInstance()->sharedMainUI->getStageUI()->showWinningMenu();
}

AttackableObject* GameStage::getComputerPlayer()
{
    return NULL;
}

Building* GameStage::getPlayerBase()
{
    return NULL;
}

Building* GameStage::getComputerBase()
{
    return NULL;
}

queue< pair<int, int> > GameStage::parseEnemies(string enemies)
{
    queue< pair<int, int> > result;
    
    // enemies is for the current wave. defined like "2.1:3, 3.2:1"...
    for (const string& valuePair : Function::splitString(enemies, ","))
    {
        // process "(*)enemy:number" , with * it is a boss
        vector<string> value = Function::splitString(valuePair, ":");
        
        int number = value.size() > 1 ? atoi(value[1].c_str()) : 1; // if not value defined after ":", it's 1 by default;
        
        vector<string> enemyDefinition = Function::splitString(value[0], ".");
        
        int level = enemyDefinition.size() > 1 ? atoi(enemyDefinition[1].c_str()) : 1;
        
        int enemyId = 0;
        
        string v1 = enemyDefinition[0];
        if (v1[0] == '*')
        {
            v1 = enemyDefinition[0].substr(1, string::npos);
            enemyId = atoi(v1.c_str());
            number = 1;  // it's a boss, only 1 boss can exist
            
            if (level == 0)
            {
                level = 1;
            }
            
            level = -level; // for boss, using negative value to indicate
        }
        else
        {
            enemyId = atoi(enemyDefinition[0].c_str());
        }
        
        
        for(int j=0;j<number;j++)
        {
            result.push(make_pair(enemyId, level));
        }
    }
    
    return result;
}

void GameStage::loadCurrentWaveData()
{

    const MapConfig* pMapConfig = GameData::getMapConfig(m_MapId);

    // show opening NPC message
    if (!Player::getInstance()->hasCompletedMap(m_MapId))
    {
        while (npcMessageQueue.size() > 0)
        {
            MapNpcMessage message = npcMessageQueue.front();
            
            
            
            // make sure it's for the ending scene
            if (message.wave <=m_CurrentWave || (isCompleted() && message.wave > m_MaxWave))
            {
                npcMessageQueue.pop();
                
                StageUI* pStageUI = GameScene::getInstance()->sharedMainUI->getStageUI();
                pStageUI->showNpcMessage(message.npcImage.c_str(), message.message.c_str());
                pStageUI->getNpcMessageBox()->setCallback(CCCallFunc::create(this, callfunc_selector(GameStage::loadCurrentWaveData)));
                
                
                return;
            }
            else if (m_CurrentWave < message.wave)
            {
                break;
            }
            
        }
        
    }

    if (isCompleted())
    {
        // current map is cleared
        if (!Player::getInstance()->hasCompletedMap(m_MapId))
        {
            
            // check if matching the winning condition
            if (m_CurrentWinningCondition.minKilledEnemies > 0)
            {
                if (Player::getInstance()->getKillCount() < m_CurrentWinningCondition.minKilledEnemies)
                {
                    GameScene::getInstance()->sharedMainUI->getStageUI()->showGameOverMenu();
                    
                    return;
                }
            }
            
        }
        
        completeStage();

        return;
    }

    
    GameScene::getInstance()->sharedMainUI->getStageUI()->updateStageWave(m_CurrentWave, m_MaxWave);

    vector<CCPoint> entrance = pGroundMap->getStartTilePositions();

    string enemies = pMapConfig->enemies[m_CurrentWave - 1];
    auto enemyIds = parseEnemies(enemies);

    //pick up usable enemies
    float spawnInterval = 0.9f;

    if (m_CurrentWave <= m_MaxWave / 3)
    {
        spawnInterval = 1;
    }
    else if (m_CurrentWave > m_MaxWave - 2)
    {
        spawnInterval = 0.8f;
    }
    
    
    float startTime = 2;
    float delay = 0;
    
    
    if (!Player::getInstance()->hasCompletedTutorial())
    {
        startTime = 1;
    }
    
    int count = 0;
    while(!enemyIds.empty())
    {
        auto data = enemyIds.front();
        enemyIds.pop();
        
        int id = data.first;
        int level = data.second;
        
        if (level == 0)
        {
            // there is no level 0 enemy
            level = 1;
        }

        Enemy* enemy = Enemy::create(id, entrance[0]);
        
        enemy->init(level);
        
        pGroundMap->addChild(enemy);
        this->enemies->addObject(enemy);
        
        delay = startTime + spawnInterval*count;
        enemy->scheduleOnce(schedule_selector(Enemy::activate), delay);
        
        ++count;
    }
    

    
}

void GameStage::pause()
{
    if (isPaused())
    {
        return;
    }
    
    pauseSchedulerAndActions();
    Player::getInstance()->pauseSchedulerAndActions();
    

    CCArray* buildings = pBuildingManager->getAllBuildings();

    int size = buildings->count();
    for(int i=0;i<size;++i)
    {
        Building* pTarget = static_cast<Building*>(buildings->objectAtIndex(i));
        pTarget->pauseSchedulerAndActions();
    }
    
    pauseAllEnemies();
    
    m_Paused = true;

}


void GameStage::resume()
{
    resumeSchedulerAndActions();
    
    Player::getInstance()->resumeSchedulerAndActions();
    
    CCArray* buildings = pBuildingManager->getAllBuildings();
    
    int size = buildings->count();
    for(int i=0;i<size;++i)
    {
        Building* pTarget = static_cast<Building*>(buildings->objectAtIndex(i));
        pTarget->resumeSchedulerAndActions();
    }
    
    resumeAllEnemies();
    
    m_Paused = false;
}

void GameStage::pauseAllEnemies()
{
    int size = enemies->count();
    for(int i=0;i<size;++i)
    {
        Enemy* pEnemy = static_cast<Enemy*>(enemies->objectAtIndex(i));
        pEnemy->pauseSchedulerAndActions();
    }
}

void GameStage::resumeAllEnemies()
{
    int size = enemies->count();
    for(int i=0;i<size;++i)
    {
        Enemy* pEnemy = static_cast<Enemy*>(enemies->objectAtIndex(i));
        pEnemy->resumeSchedulerAndActions();
    }
}


vector<int> GameStage::getCurrentStageEnemies()
{
    vector<string> enemyValues = GameData::getMapConfig(m_MapId)->enemies;
    
    vector<int> result;
    unordered_set<int> idTable;
    
    for (const string& enemies : enemyValues)
    {
        auto enemyIds = parseEnemies(enemies);
        
        while (!enemyIds.empty())
        {
            auto identifier = enemyIds.front();
            enemyIds.pop();
            int id = identifier.first;
            if (idTable.find(id) != idTable.end())
            {
                continue;
            }

            idTable.insert(id);
            result.push_back(id);
        }
    }
    
    return result;

}

void GameStage::removeEnemy(Enemy* pEnemy)
{
    enemies->removeObject(pEnemy);

    if (enemies->count()==0)
    {
        
        // we need to find if the current level ends (reaching max waves)
        // if not ends (not reaching max waves), 
        /*
        const MapConfig* pConfig = GameData::getMapConfig(m_MapId);
        
        if (!pConfig)
        {
            // cannot find level config, end current level
            return;
        }
        */
        
        loadEnemies();
        
    }

}


void GameStage::removeUnit(Unit* pUnit)
{
    units->removeObject(pUnit);

    
}



bool GameStage::hasEnemy(CCObject* pEnemy)
{
    return enemies->containsObject(pEnemy);
}

StageMap* GameStage::getGroundMap()
{
    return pGroundMap;
}

BuildingManager* GameStage::getBuildingManager()
{
    /*
    BuildingLayer* pBuildingLayer = static_cast<BuildingLayer*>(this->getChildByTag(GroundMapBuildingLayerTag));
    return pBuildingLayer;
     */
    return pBuildingManager;
}

/*
void GameStage::showCircle(cocos2d::CCPoint pos, float radius)
{
    Function::drawSolidCircle(pos, radius, CC_DEGREES_TO_RADIANS(0), 100, ccc4f(0.2, 0.2, 0.8, 0.3));

}
*/

void GameStage::centerMapOnTileCoord(CCPoint tilePos)
{
    CCTMXLayer* pLayer = pGroundMap->layerNamed("ground");
    tilePos.y-=1;
    
    CCPoint scrollPos = pLayer->positionAt(tilePos);
    
    scrollPos = ccpMult(scrollPos, -1);
    scrollPos = ccpAdd(scrollPos, screenCenter);
    
    CCAction* move = CCMoveTo::create(0.1f, scrollPos);
    
    pGroundMap->stopAllActions();
    pGroundMap->runAction(move);
}

CCPoint GameStage::locationFromTouch(cocos2d::CCTouch *pTouch)
{
    CCPoint touchLocation = pTouch->getLocationInView();
    return CCDirector::sharedDirector()->convertToGL(touchLocation);
}

CCPoint GameStage::locationFromTouches(CCSet* pTouches) {
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    return this->locationFromTouch(pTouch);
}


void GameStage::ccTouchesBegan(cocos2d::CCSet *pTouches, CCEvent* pEvent)
{
    
    
    if (CCDirector::sharedDirector()->isPaused())
    {
        return;
    }
    
    if (!m_StageReady || isCompleted() || isPaused())
    {
        return;
    }

    CCPoint touchPos = this->locationFromTouches(pTouches);
    
//    Player::getInstance()->setPerfomingSkillId(2);
    int performingSkillId = Player::getInstance()->getPerformingSkillId();
    
    if (performingSkillId > 0 && m_pSkillTouchAnimation)
    {
        m_pSkillTouchAnimation->touchBegan(touchPos);
        return;
    }
    
    m_TouchMoveLocation = CCPointZero;
    m_TouchStartLocation = touchPos;
    m_TouchStartTime = m_TotalStageRunningTime;
        
    pBuildingManager->unselectAllBuildings();   
//    GameScene::getInstance()->sharedMainUI->getStageUI()->removeInstructions();
    
}

void GameStage::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    
    if (!m_StageReady)
    {
        return;
    }

    
    if (GameScene::getInstance()->sharedMainUI->getStageUI()->isInBuildingMenu())
    {
        return;
    }

    
    CCPoint location = this->locationFromTouches(pTouches);


    int performingSkillId = Player::getInstance()->getPerformingSkillId();

    if (performingSkillId > 0 && m_pSkillTouchAnimation)
    {
        m_pSkillTouchAnimation->touchMoved(location);
        return;
    }

    
    if (pTouches->count() > 1)
    {
        // scale map
        touchMoveDx = 0;
        touchMoveDy = 0;
        m_TouchStartTime = 0; // disbale single tap event, so menu won't be triggered
        
        m_TouchMoveLocation = CCPointZero;
        
        CCSetIterator it;

        it = pTouches->begin();
        CCTouch* pTouch1 = (CCTouch*)(*it);
        it++;
        CCTouch* pTouch2 = (CCTouch*)(*it);
        
        CCPoint p = ccpSub(pTouch1->getLocation(), pTouch2->getLocation());
        float distance = p.x * p.x + p.y * p.y;
        
        if (touchInitialDistance == 0)
        {
            touchInitialDistance = distance;
            CCLog("Initial distance: %f", touchInitialDistance);
            return;
        }

        float oldScale = this->getScale();

        CCLog("%f", distance);
        if (distance > touchInitialDistance)
        {
            zoomIn();
        }
        else if (distance < touchInitialDistance)
        {
            zoomOut();
        }
        
        if (isOutOfMapBoundary())
        {
            this->setScale(oldScale);
        }
        
        return;
    }

    if (m_TouchMoveLocation.x == 0 && m_TouchMoveLocation.y == 0)
    {
        // first time touch
        touchMoveDy = 0;
        touchMoveDx = 0;
        m_TouchMoveLocation = location;
        return;
    }
    
    // single touch move, move map
    float dx = location.x - m_TouchMoveLocation.x;
    float dy = location.y - m_TouchMoveLocation.y;
    float distance = pow(dx, 2) + pow(dy, 2);
    
    
    if (distance>10)
    {
        touchMoveDx = dx;
        touchMoveDy = dy;
        
        m_TouchMoveLocation = location;

    }
    

    
}

void GameStage::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    
    
    if (!m_StageReady)
    {
        return;
    }


    touchInitialDistance = 0;
    touchMoveDx = 0;
    touchMoveDy = 0;
    m_TouchMoveLocation = CCPointZero;
    
    CCPoint touchPos = this->locationFromTouches(pTouches);
    
    
    int performingSkillId = Player::getInstance()->getPerformingSkillId();
    
    if (performingSkillId > 0 && m_pSkillTouchAnimation)
    {
        m_pSkillTouchAnimation->touchEnded(locationFromTouches(pTouches));
        return;
    }


    
    if (!pBuildingManager->hasUnplacedBuilding() &&
        !GameScene::getInstance()->sharedMainUI->getStageUI()->isInBuildingMenu() &&
        abs((int)m_TouchStartLocation.x - (int)touchPos.x)<50 &&
        abs((int)m_TouchStartLocation.y - (int)touchPos.y)<50)
    {
         GameScene::getInstance()->sharedMainUI->getStageUI()->onUserTapOnMap(m_TouchStartLocation);
    }
    else
    {
        GameScene::getInstance()->sharedMainUI->getStageUI()->removeBuildingMenu();
        removeUnplacedBuilding();

    }


}

void GameStage::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}


void GameStage::zoomIn()
{
    const float maxScale = 3.0f;
    float oldScale = pGroundMap->getScale();
    if (oldScale >= maxScale)
    {
        return;
    }
    
    float scale = oldScale + 0.025;
    if (scale > maxScale)
    {
        scale = maxScale;
    }
    pGroundMap->setScale(scale);
}

void GameStage::zoomOut()
{
    float minScale = m_InitialMapScale;
    
    float oldScale = pGroundMap->getScale();
    if (oldScale <= minScale)
    {
        return;
    }
    
    float scale = oldScale - 0.05;
    if (scale < minScale)
    {
        return;
    }
    
    pGroundMap->setScale(scale);
    
    // move map towards center
    
    // 1. get center screeen position
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint centerScreenPosition = ccp(winSize.width/2, winSize.height/2);
    /*
     CCSize mapSize = pGroundMap->getMapSize();
     CCPoint centerMapPosition = MapHelper::tilePosToGroundMapPos(ccp((mapSize.width+1)/2, (mapSize.height+1)/2));
     */
    CCSize mapSize = pGroundMap->getContentSize();
    //    CCPoint centerMapPosition = ccp((mapSize.width+1)/2, (mapSize.height+1)/2);
    CCPoint centerMapPosition = m_MapCenterPos;
    CCPoint diff = ccpSub(centerScreenPosition, pGroundMap->convertToWorldSpace(centerMapPosition));
    
    if (diff.x != 0 && diff.y != 0)
    {
        CCPoint currentMapPosition = pGroundMap->getPosition();
        
        CCPoint newPos = ccpAdd(currentMapPosition, ccp(diff.x/10, diff.y/10));
        
        pGroundMap->setPosition(newPos);
    }
    
    alignMapBoundary();
    
}


void GameStage::removeBuilding(Building *pBuilding)
{
    // do nothing, just a place holder for additional cleanup. Quick battle uses this for base cleanup
}

void GameStage::showBuildingToPlace(const CCPoint& tilePos)
{

    
    CCPoint mapPos = MapHelper::tilePosToGroundMapPos(tilePos);
    CCSprite* tempBuilding = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/tile_mask.png"));
    tempBuilding->setAnchorPoint(ccp(0.5,1));
    tempBuilding->setPosition(ccp((int)mapPos.x, (int)mapPos.y));
    pGroundMap->addChild(tempBuilding, 128, TEMP_BUILDING_TAG);
}

void GameStage::placePlayerMoveMark(const cocos2d::CCPoint &groundMapPos)
{
//    CCPoint mapPos = MapHelper::tilePosToGroundMapPos(ccp(tilePos.x+0.5, tilePos.y+0.5));
    
    CCSprite* pMarkAnimation = AnimationManager::getInstance()->getPlayerSetDestinationAnimation();
    pMarkAnimation->setPosition(groundMapPos);
    pGroundMap->addChild(pMarkAnimation);

}

void GameStage::placeUnitMoveMark(const cocos2d::CCPoint &groundMapPos)
{
    CCSprite* pMarkAnimation = AnimationManager::getInstance()->getUnitSetDestinationAnimation();
    pMarkAnimation->setPosition(groundMapPos);
    pGroundMap->addChild(pMarkAnimation);
}

void GameStage::hidePlayerMoveMark()
{
    CCSprite* pMark = static_cast<CCSprite*>(pGroundMap->getChildByTag(PLAYER_MOVE_DESTINATION_MARK_TAG));
    
    if (pMark && pMark->isVisible())
    {
        pMark->setVisible(false);
    }
}

void GameStage::removeUnplacedBuilding()
{

    pBuildingManager->removeBuildingToPlace();
    pGroundMap->removeChildByTag(TEMP_BUILDING_TAG, true);
}


void GameStage::updateTileMapPosition()
{
    if (touchInitialDistance != 0)
    {
        // zooming, don't move map
        return;
    }
    
    if (touchMoveDx == 0 && touchMoveDy == 0)
    {
        return;
    }

    
    // align boundary first
    alignMapBoundary();
    
    CCPoint oldPos = pGroundMap->getPosition();
    
    CCPoint newPos = oldPos;
    
    newPos.x += touchMoveDx;
    pGroundMap->setPosition(newPos);
    if (isOutOfMapBoundary())
    {
        newPos.x = oldPos.x;
    }

    newPos.y += touchMoveDy;
    pGroundMap->setPosition(newPos);
    
    if (isOutOfMapBoundary())
    {
        newPos.y = oldPos.y;
    }
    
    pGroundMap->setPosition(newPos);
    

    
    touchMoveDx = 0;
    touchMoveDy = 0;

}

bool GameStage::isOutOfMapBoundary()
{
    
    CCSize mapSize = pGroundMap->getMapSize();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    bool reachingBoundary = false;
    
    for (float y = 0; y < winSize.height ; y+=winSize.height-1)
    {
        for(float x = 0; x < winSize.width ; x+=winSize.width-1)
        {
            CCPoint testPos = pGroundMap->convertToNodeSpace(ccp(x,y));
            
            if (!m_Boundary.containsPoint(testPos))
            {
                reachingBoundary = true;
                break;
            }


            
        }
    }
    
    return reachingBoundary;
}


bool GameStage::isNearMapBoundary()
{
    
    CCSize mapSize = pGroundMap->getMapSize();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    bool nearBoundary = false;
    
    CCRect smallerBoundary = CCRectMake(m_Boundary.getMinX()+10, m_Boundary.getMinY()+10, m_Boundary.size.width-20, m_Boundary.size.height-20);
    
    for (float y = 0; y < winSize.height ; y+=winSize.height-1)
    {
        for(float x = 0; x < winSize.width ; x+=winSize.width-1)
        {
            CCPoint testPos = pGroundMap->convertToNodeSpace(ccp(x,y));
            
            if (!smallerBoundary.containsPoint(testPos))
            {
                nearBoundary = true;
                break;
            }

            
        }
    }
    
    return nearBoundary;
}

void GameStage::updateShadowLayer()
{
    CCTMXLayer* pLayer = pGroundMap->getShadowLayer();
 
    if (!pLayer)
    {
        return;
    }
    
	for (const CCPoint& shadowTile : pGroundMap->getShadowTiles())
    {
        CCSprite* sprite = pLayer->tileAt(shadowTile);
        sprite->setOpacity(255);
    }
    
    if (pBuildingManager->hasUnplacedBuilding())
    {
        Building* pBuilding = pBuildingManager->getUnplacedBuilding();
        CCPoint tilePos = pBuilding->getOriginTilePosition();
        int range = pBuilding->getAttackRange()/MapHelper::MAP_TILE_LENGTH;
        lightOnTile(tilePos.x, tilePos.y,range);
        
    }
    
    CCObject* pObj;
    CCArray* buildings = pBuildingManager->getAllBuildings();
    CCARRAY_FOREACH(buildings, pObj)
    {
        Building* pBuilding = static_cast<Building*>(pObj);
        CCPoint tilePos = pBuilding->getOriginTilePosition();
        lightOnTile(tilePos.x, tilePos.y, pBuilding->getAttackRange()/MapHelper::MAP_TILE_LENGTH);
    }
    
    // update player neighbors
    CCPoint playerPos = Player::getInstance()->getCurrentMapTile();
    lightOnTile(playerPos.x, playerPos.y, 2);
    
	for (const CCPoint& startTile : pGroundMap->getStartTilePositions())
    {
        lightOnTile(startTile.x, startTile.y, 2);
    }

}


bool GameStage::matchWinningCondition()
{
    Player* pPlayer = Player::getInstance();
    
    if (m_CurrentWinningCondition.killPlayer && Player::getInstance()->isDead())
    {
        return false;
    }
    
    if (m_CurrentWinningCondition.killNpcId && getNpc(m_CurrentWinningCondition.killNpcId)->isDead())
    {
        return false;
    }
    
    if (m_CurrentWinningCondition.maxPassEnemies > 0  && getPassedEnemies() > m_CurrentWinningCondition.maxPassEnemies)
    {
        return false;
    }
    
    if (m_CurrentWinningCondition.minKilledEnemies > 0 && pPlayer->getKillCount() < m_CurrentWinningCondition.minKilledEnemies)
    {
        return false;
    }
    
    if (m_CurrentWinningCondition.maxTime > 0 && m_TotalStageRunningTime > m_CurrentWinningCondition.maxTime)
    {
        return false;
    }
    
    return true;
}

void GameStage::update(float delta)
{
    if (isPaused())
    {
        return;
    }
    
    if (m_CurrentWinningCondition.killNpcId>0)
    {
        AttackableObject* npc = getNpc(m_CurrentWinningCondition.killNpcId);
        if (npc->isDead())
        {
            npc->remove();
            GameScene::getInstance()->sharedMainUI->getStageUI()->showGameOverMenu();
            return;
        }
    }
    
    if (!Player::getInstance()->hasCompletedMap(m_MapId))
    {
        if ( (m_CurrentWinningCondition.maxPassEnemies > 0 && m_PassedEnemies >= m_CurrentWinningCondition.maxPassEnemies) ||
             (m_CurrentWinningCondition.maxTime > 0 && m_TotalStageRunningTime > m_CurrentWinningCondition.maxTime)
            )
        {
            GameScene::getInstance()->sharedMainUI->getStageUI()->showGameOverMenu();
            return;
            
        }
    }

    m_TotalStageRunningTime += delta;
    
    updateTileMapPosition();
    
    pBuildingManager->updateBuildingToPlace();

    
    updateShadowLayer();

    if (m_FollowPlayer)
    {
        followPlayer();
    }

    return;
 }



void GameStage::lightOnTile(int x, int y, int range)
{
    
    CCSize mapSize = pGroundMap->getMapSize();
    CCTMXLayer* pLayer = pGroundMap->getShadowLayer();
    
    if (!pLayer)
    {
        return;
    }
    
    CCPoint center = ccp(x,y);

    int step = 160 / (range + 1);
    
    for (int i = y-range; i<=y+range;++i)
    {
        for (int j=x-range;j<=x+range;++j)
        {
            
            CCPoint pos = ccp(j,i);
            
            if (i<0 || i>=mapSize.height || j<0 || j>=mapSize.width)
            {
                continue;
            }
            
            CCSprite* sprite = pLayer->tileAt(pos);
            if (!sprite)
            {
                continue;
            }

            int dx = abs(j-x);
            int dy = abs(i-y);

            int originalValue = sprite->getOpacity();

            int newValue = originalValue - 160 + dx*step + dy*step;
            
            if (newValue > 255)
            {
                newValue = 255;
            }
            
            if (newValue < 0)
            {
                newValue = 0;
            }
            
            sprite->setOpacity(newValue);

        }
    }
}

int GameStage::getDynamicOpacityValue(int value, const CCPoint& current, const CCPoint& center, int initValue, int delta)
{
    int opacity = value;
    
    int i = current.y;
    int j = current.x;
    int x = center.x;
    int y = center.y;
    
    if (opacity>0)
    {
        if (i == y && j==x)
        {
            opacity -= 96;
        }
        else if ((i==y && abs(j-x)==1) || (j==x && abs(i-y)==1))
        {
            opacity -= 80;
        }
        else if (abs(i-y)==1 && abs(j-x)==1)
        {
            opacity -= 64;
        }
        else if ((i==y && abs(j-x)==2) || (j==x && abs(i-y)==2))
        {
            opacity -= 48;
        }
        else
        {
            opacity -= 32;
        }
    }
    if (opacity > 255)
    {
        opacity = 255;
    }
    if (opacity < 0)
    {
        opacity = 0;
    }
    
    return opacity;
}



void GameStage::draw()
{

    CCLayer::draw();
    
}


void GameStage::didAccelerate(CCAcceleration* pAccelerationValue)
{
    // no acceleration skill for now
    return;
    
    if (Player::getInstance()->getPerformingSkillId() != 3)
    {
        return;
    }

    float dx = 0;
    float dy = 0;
    const float sensitivity = 0.1f;
    const float speed = 1.0f;

    if (pAccelerationValue->x > sensitivity || pAccelerationValue->x < -sensitivity)
    {
        dx = pAccelerationValue->x * speed;
    }
    
    if (pAccelerationValue->y > sensitivity || pAccelerationValue->y < -sensitivity)
    {
        dy = pAccelerationValue->y * speed;
    }

    
    if (dx != 0 || dy != 0)
    {
        m_pSkillTouchAnimation->setAnimationOffset(dx, dy);
    }
}
