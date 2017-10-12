//
//  GameScene.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/13/12.
//
//

#include "GameScene.h"
#include "Player.h"
#include "GameData.h"
#include "Enemy.h"
#include "ExplosionCache.h"
#include "StartScreenBuilderLoader.h"
#include "StageOpeningAnimationLoader.h"
#include "PerformEffectAnimation.h"
#include "AnimationManager.h"
#include "SimpleAudioEngine.h"
#include "QuickBattleStage.h"
#include "CompletedStage.h"
#include "MainUI.h"
#include "MessageBox.h"
#include "cocos-ext.h"
#include <map>
#include "EndLessGameStage.h"

using namespace cocos2d::extension;

GameScene* GameScene::getInstance()
{
    static GameScene* gameScene;
    
    if (!gameScene)
    {
        if (gameScene)
        {
            delete gameScene;
        }
        
        gameScene = new GameScene();
    }
    
    return gameScene;
}

GameScene::GameScene()
	: m_DataLoaded(false), sharedScene(nullptr), sharedGameStage(nullptr), sharedMainUI(nullptr)
{
    sharedGameStage = nullptr;

    sharedScene = CCScene::create();
    sharedScene->retain();
    
    sharedMainUI = MainUI::create();
    sharedScene->addChild(sharedMainUI, 1);
}

GameScene::~GameScene()
{
    
}

void GameScene::loadData()
{
    if (m_DataLoaded)
    {
        return;
    }
    
    GameData::init();
    
    CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    map<int, BuildingSetting>::const_iterator buildingIt = GameData::getBuildingSettings().begin();
    while(buildingIt != GameData::getBuildingSettings().end())
    {
        char plistPath[128] = {0};
        char texturePath[128] = {0};

        if (buildingIt->first != 0)
        {
            sprintf(plistPath, "building_animation/%d/animation.plist", buildingIt->first);
            sprintf(texturePath, "building_animation/%d/animation.pvr.ccz", buildingIt->first);
            frameCache->addSpriteFramesWithFile(plistPath, texturePath);
        }

        buildingIt++;
    }
    
    vector<Magic*> magics = GameData::getAllMagic();
    
    for (const Magic* pMagic : magics)
    {
        char plistPath[128] = {0};
        char texturePath[128] = {0};
        
        sprintf(plistPath, "effects/magic%d.plist", pMagic->id);
        sprintf(texturePath, "effects/magic%d.pvr.ccz", pMagic->id);
        frameCache->addSpriteFramesWithFile(plistPath, texturePath);
    }

    frameCache->addSpriteFramesWithFile("bullet/animation.plist", "bullet/animation.pvr.ccz");
    frameCache->addSpriteFramesWithFile("player/animation.plist", "player/animation.pvr.ccz");
    frameCache->addSpriteFramesWithFile("environment/property/tile_animation.plist", "environment/property/tile_animation.pvr.ccz");
    
    frameCache->addSpriteFramesWithFile("effects/animation.plist", "effects/animation.pvr.ccz");
    
    frameCache->addSpriteFramesWithFile("effects/stage_animation/animation.plist", "effects/stage_animation/animation.pvr.ccz");
    frameCache->addSpriteFramesWithFile("action_effect/animation.plist", "action_effect/animation.pvr.ccz");

    frameCache->addSpriteFramesWithFile("npc/animation.plist", "npc/animation.pvr.ccz");
    
    


    PerformEffectAnimation::loadEffects();
    

    Enemy::initCache();
    // FIXME
    ExplosionCache::getInstance()->init();

    if (!AnimationManager::getInstance())
    {
        return;
    }
    

    m_DataLoaded = true;
}

void GameScene::setupPlayScreen(int mapId, bool isNormalStage)
{
    if (!sharedGameStage)
    {
        if (mapId < 100)
        {
            sharedGameStage = QuickBattleStage::create(mapId);
            
        }
        else
        {
            string mapInt = GameData::getMapConfig(mapId)->introduction;
            if(mapInt == string("EndLess") || mapInt == string("无尽")){
                sharedGameStage = EndLessGameStage::create(mapId);
            }else{
                sharedGameStage = isNormalStage ? GameStage::create(mapId) : CompletedStage::create(mapId);
            }
        }

        sharedScene->addChild(sharedGameStage, 0);
    }
    
    if (!sharedMainUI)
    {
        sharedMainUI = MainUI::create();
        sharedScene->addChild(sharedMainUI, 1);
    }
    
    
    sharedGameStage->reset();
}


void GameScene::clearStage()
{
    if (sharedGameStage)
    {
        sharedGameStage->clear();
        sharedGameStage->removeFromParentAndCleanup(true);
        sharedGameStage = NULL;
    }

}

void GameScene::loadStageMap(int mapId, bool isNormalStage)
{
    clearStage();

    setupPlayScreen(mapId, isNormalStage);
    
    char key[64]= {0};
    sprintf(key, "map_%d", mapId);
    
    
//    CCDirector::sharedDirector()->pushScene(sharedScene);

        // only load OP for the 1st map in each stage
    loadStageOpeningAnimation(mapId);
}

void GameScene::exitStageMap()
{
    StageUI* pStageUI = sharedMainUI->getStageUI();
    if (pStageUI)
    {
        pStageUI->removeArrow();
        pStageUI->removeInstructions();

        if (pStageUI->hasMessageBox())
        {
            MessageBox* pMessageBox = pStageUI->getMessageBox();
            pMessageBox->close();
        }
    }
    
    Player::getInstance()->load();
    
//    Player::getInstance()->save();
    
    int mapId = sharedGameStage->getMapId();
    
    sharedGameStage->clear();
    sharedGameStage->removeFromParentAndCleanup(true);
    sharedGameStage = NULL;

//    loadStageEndingAnimation(mapId);
    char key[64]= {0};
    sprintf(key, "map_%d", mapId);
    
    sharedMainUI->showWorldMap();
}

void GameScene::loadStageEndingAnimation(int mapId)
{
    char buffer[128] = {0};
    sprintf(buffer, "ccb/stage_end_%d.ccbi", mapId);
    
    loadStageAnimation(mapId, false, buffer);
}

void GameScene::loadStageOpeningAnimation(int mapId)
{
    if (Player::getInstance()->hasCompletedMap(mapId))
    {
        if (CCDirector::sharedDirector()->getRunningScene() != sharedScene)
        {
            CCDirector::sharedDirector()->popScene();
        }

        return;
    }
    
    char buffer[128] = {0};
    sprintf(buffer, "ccb/stage_%d.ccbi", mapId);
    
    loadStageAnimation(mapId, true, buffer);
    
}

void GameScene::loadStageAnimation(int mapId, bool isOpening, const char *pPath)
{
    CCScene* scene = NULL;
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(pPath);
    
    if (!CCFileUtils::sharedFileUtils()->isFileExist(pPath))
    {
        if (CCDirector::sharedDirector()->getRunningScene() != sharedScene)
        {
            CCDirector::sharedDirector()->popScene();
        }
        
        return;
    }
    
    CCNodeLoaderLibrary *ccNodeLoaderLibrary=cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    ccNodeLoaderLibrary->registerCCNodeLoader("StageOpeningAnimation", StageOpeningAnimationLoader::loader());
    
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();
    

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //    CCNode * node = ccbReader->readNodeGraphFromFile(path, this);
    
    CCNode* node = ccbReader->readNodeGraphFromFile(path.c_str(), this, winSize);
    
    StageOpeningAnimation* pStageOp = static_cast<StageOpeningAnimation*>(node);
    
    pStageOp->setOpening(isOpening);
    
    
    CCLayer* pBackground = pStageOp->background;
    if(pBackground)
    {
        float scaleX = winSize.width/pBackground->getContentSize().width;
        float scaleY = winSize.height/pBackground->getContentSize().height;
        
        pBackground->setScale(scaleX);
        pBackground->setScaleY(scaleY);
    }
    
    // special case for different stage
    if (mapId == 1 && isOpening)
    {
        scheduleOnce(schedule_selector(GameScene::playStage1Sound), 7.8f);
    }
    
    scene = CCScene::create();
    if(node != NULL) {
        scene->addChild(node);
    }
    
    CCDirector::sharedDirector()->replaceScene(scene);
}


void GameScene::playStage1Sound()
{
    unschedule(schedule_selector(GameScene::playStage1Sound));
    // FIXME
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/building_5_bullet_explosion.mp3", true);
}

void GameScene::cameraShot()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/place_ok.wav");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor* shot = CCLayerColor::create(ccc4(255, 255, 255, 255));
    shot->cocos2d::CCNode::setContentSize(winSize);
    CCActionInterval *clear = dynamic_cast<CCActionInterval *>(CCCallFuncN::create(this, callfuncN_selector(GameScene::removeShotLayer)));
    shot->runAction(CCSequence::create(CCFadeTo::create(0.3f, 0),clear,NULL));
    StageUI* ui = GameScene::getInstance()->sharedMainUI->getStageUI();
    ui->getChildByTag(OPENINGWINDOW_TAG)->addChild(shot, 128);
}

void GameScene::removeShotLayer(CCNode* pSender)
{
    pSender->removeFromParentAndCleanup(true);
}



