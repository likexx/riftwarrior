//
//  WorldMap.cpp
//  tdgame
//
//  Created by Like Zhang on 3/2/13.
//
//

#include "WorldMap.h"
#include "GameScene.h"
#include "SkillTreeMenu.h"
#include "SimplePlayerView.h"
#include "WorldmapGameMenu.h"
#include "GameData.h"
#include "Player.h"
#include "IAPMenu.h"
#include "SaleStoreMenu.h"
#include "SimpleAudioEngine.h"
#include "LoadingScreen.h"
#include "SystemHelper.h"
#include "constant.h"
#include "AdHelper.h"

using namespace CocosDenshion;


#define STAGE_MARK_TAG_START 10000

#define STAGE_INTRODUCTION_TAG 20000
#define PLAYER_STATUS_TAG 20001

#define SKILL_TREE_MENU_LABEL_TAG 30001
#define WORLD_MAP_GAME_MENU_LABEL_TAG 30002

#define NEXT_MAP_ARROW_TAG 40001

bool WorldMap::init()
{

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    setContentSize(winSize);
    
    CCSprite* background = CCSprite::create("UI/map_background.png");
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    
    background->setScaleX(winSize.width/background->getContentSize().width);
    background->setScaleY(winSize.height/background->getContentSize().height);
    
    this->addChild(background);
    
    CCNode* pPlayerView = SimplePlayerView::create();
    addChild(pPlayerView, 0, PLAYER_STATUS_TAG);
    Function::scaleNodeForScreen(pPlayerView, ccp(0,1), ccp(0, winSize.height));
    
    
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    menu->setContentSize(background->getContentSize());

    background->addChild(menu);

    Player* player = Player::getInstance();
    
    const map<int, MapConfig>& mapConfigTable = GameData::getAllMapConfigs();
    
    map<int, MapConfig>::const_iterator it = mapConfigTable.begin();
    
    // first, show all completed maps
    bool hasNextUnfinishedMap = false;
    
    int count = 0;
    while (it != mapConfigTable.end())
    {
        const MapConfig& config = it->second;
        
        if ( (config.requireMapId == 0 || player->hasCompletedMap(config.requireMapId)) &&
            (config.hideAfter == 0 || !player->hasCompletedMap(config.hideAfter)))
        {
            if (CCMenuItem* pMark = createStageMapMark(config.name.c_str(), ccp(config.worldmapX, config.worldmapY), config.mapId))
            {
                menu->addChild(pMark, 0, STAGE_MARK_TAG_START + config.mapId);
                
                if (player->hasCompletedMap(config.requireMapId) && !player->hasCompletedMap(config.mapId))
                {
                    CCSprite* arrowDown = CCSprite::create("UI/arrow_down.png");
                    arrowDown->setAnchorPoint(ccp(0.5,0.3));
                    CCPoint targetPos = pMark->getPosition();
                    targetPos.y += arrowDown->getTextureRect().size.height/2;
                    
                    arrowDown->setPosition(targetPos);
                    
                    CCPoint moveToPos = ccp(targetPos.x, targetPos.y + arrowDown->getTextureRect().size.height/2);
                    moveToPos.x = targetPos.x;
                    arrowDown->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCMoveTo::create(0.5, moveToPos),
                                                                                                       CCMoveTo::create(0.5, targetPos),
                                                                                                       NULL)));
                    background->addChild(arrowDown, 0, NEXT_MAP_ARROW_TAG);
                    
                    hasNextUnfinishedMap = true;

                }
                
                // show star level for completed maps
                int score = player->getMapScore(config.mapId);
                if (score > 0)
                {
                    int n = 3;
                    
                    if (score < 70)
                    {
                        n = 1;
                    }
                    else if (score < 90)
                    {
                        n = 2;
                    }
                    
                    for(int i=0;i<n;++i)
                    {
                        CCSprite* star = CCSprite::create("UI/building/lv.png");
                        star->setPosition(ccp(star->getContentSize().width * i, pMark->getContentSize().height));
                        star->setAnchorPoint(ccp(0, 0.5));
                        pMark->addChild(star);
                        
                    }
                }
            }
        }
        
        ++count;
        ++it;
    }
    
    m_pBackground = background;
    

    m_pWorldmapGameMenu = WorldmapGameMenu::create();
    
    addChild(m_pWorldmapGameMenu,3);
    
    //m_pWorldmapGameMenu->setRotation(7.5);
    m_pWorldmapGameMenu->setPosition(ccp(winSize.width, winSize.height));
    m_pWorldmapGameMenu->setAnchorPoint(ccp(1,1));
    
    m_worldMenu = CCMenu::create();
    m_worldMenu->setPosition(ccp(0, 0));
    addChild(m_worldMenu,2);

    
    m_pStageIntroductionNode = NULL;
    
    SystemHelper::loadServerMessageAsync();
    
    // FIXME
    //SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/worldmap.mp3", true);
    
    if (Player::getInstance()->hasCompletedTutorial())
    {
        SystemHelper::showGoogleBar();
    }
    
    AdHelper::showAd();
    
    return true;
}


CCMenuItem* WorldMap::createStageMapMark(const char* name, const CCPoint& pos, int mapId)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    char* markPath = NULL;
    string introduction = GameData::getMapConfig(mapId)->introduction;
    if(mapId >= 1000){
        markPath = const_cast<char *>("UI/Endless_mark.png");
    }else{
        markPath = const_cast<char *>(Player::getInstance()->hasCompletedMap(mapId) ? "UI/worldmap/finished_map_mark.png" : "UI/worldmap_mark.png");
    }
    CCMenuItemImage* item = CCMenuItemImage::create(markPath, markPath, this, menu_selector(WorldMap::showStageInformation));
    
    item->setPosition(pos);
    item->setAnchorPoint(ccp(0.5,0.3));
    
    if (!Player::getInstance()->hasCompletedMap(mapId))
    {
        item->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCFadeTo::create(0.75, 64),
                                                                                      CCFadeTo::create(0.75, 255),
                                                                                      NULL)));
    }
    

    
    return item;
}


void WorldMap::showStageInformation(cocos2d::CCObject *pSender)
{
    if (m_pWorldmapGameMenu->isInUse())
    {
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    
    if (m_pStageIntroductionNode)
    {
        return;
    }
    
    CCMenuItem* pItem = static_cast<CCMenuItem*>(pSender);
    int mapId = pItem->getTag() - STAGE_MARK_TAG_START;

    CCSize winSize = m_pBackground->getContentSize();



    CCSprite* background = CCSprite::create("UI/map_select_bg.png");
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    background->setAnchorPoint(ccp(0.5, 0.5));

    // create tmx map and render it to texture
    char buffer[128] = {0};
    sprintf(buffer, "tilemap/%d.tmx", mapId);
    CCTMXTiledMap* pTiledMap = CCTMXTiledMap::create(buffer);
    
    const char* layersToRemove[] = {"road", "building_area", "shadow","property"};
    size_t size = sizeof(layersToRemove)/sizeof(layersToRemove[0]);
    for(size_t i=0;i<size;++i)
    {
        CCNode* pLayer = pTiledMap->layerNamed(layersToRemove[i]);
        if (pLayer)
        {
            pTiledMap->removeChild(pLayer, true);
        }
    }
    CCSize mapSize = pTiledMap->getContentSize();
    CCLog("%f, %f", mapSize.width, mapSize.height);
    
    CCLayer* tempMapLayer = CCLayer::create();
    tempMapLayer->setContentSize(CCSizeMake(1024, 1024));
    pTiledMap->ignoreAnchorPointForPosition(false);
    pTiledMap->setAnchorPoint(ccp(0.5, 0.5));
    pTiledMap->setPosition(512, 512);
    tempMapLayer->addChild(pTiledMap);
    pTiledMap->setScale(1024/pTiledMap->getContentSize().width);
    
    CCRenderTexture* rt = CCRenderTexture::create(1024, 1024);

    rt->beginWithClear(0, 0, 0, 0);
    
    tempMapLayer->visit();

    
    rt->end();
    
    CCTexture2D* pTexture = rt->getSprite()->getTexture();
    CCSize textureSize = pTexture->getContentSize();
    
    CCRect backgroundRect = CCRectMake(textureSize.width*0.35, textureSize.height*0.4, textureSize.width * 0.3, textureSize.height* 0.2);
    //CCRect backgroundRect = CCRectMake(textureSize.width*0.2, textureSize.height*0.2, textureSize.width, textureSize.height);
    CCSprite* backgroundMap = CCSprite::createWithTexture(pTexture, backgroundRect);
    backgroundMap->setFlipY(true);
    

    
    backgroundMap->setScale(background->getContentSize().width*0.6/backgroundMap->getContentSize().width);
//    backgroundMap->setScaleY(background->getContentSize().height*0.6/backgroundMap->getContentSize().height);
    backgroundMap->setAnchorPoint(ccp(0, 0));    
    backgroundMap->setPosition(ccp(background->getContentSize().width * 0.08, background->getContentSize().height * 0.12));
//    backgroundMap->setColor(ccc3(128,128,128));
    background->addChild(backgroundMap);
    
    
    const MapConfig* pConfig = GameData::getMapConfig(mapId);
    

    
    float labelXForec = 0.8055f,labelYForec = 0.655f;
    //const float labelDistance = 0.1f;
    
    CCLabelTTF* infoLabel = CCLabelTTF::create(pConfig->introduction.c_str(), STANDARD_NUMBER_FONT_NAME, 24);
    infoLabel->setColor(ccc3(43,28,19));
    infoLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    infoLabel->setPosition(ccp(background->getContentSize().width * labelXForec, background->getContentSize().height * labelYForec));
    background->addChild(infoLabel);
    
     
    CCLabelTTF* nameLabel = CCLabelTTF::create(pConfig->name.c_str(), STANDARD_BOLD_FONT_NAME, 27);
    nameLabel->setColor(ccc3(255,255,255));
    nameLabel->setPosition(ccp(background->getContentSize().width * 0.5, background->getContentSize().height * 0.865));
    background->addChild(nameLabel);
    
    m_pBackground->addChild(background, 0, STAGE_INTRODUCTION_TAG);
    
//    m_pBackground->setColor(ccc3(64, 64, 64));
    
    CCMenu* menu = CCMenu::create();
    menu->setPosition(ccp(0, 0));
    menu->setAnchorPoint(ccp(0,0));
    menu->setContentSize(background->getContentSize());
    
    CCMenuItemImage* btn = NULL;
    CCLabelTTF* label;
    
    float btnXForce = 0.8055f, btnYForec = 0.185f;
    const float yDistance = 0.17f;
    
    
    bool requirePurchase = false;
    
    if (Player::getInstance()->hasCompletedMap(mapId))
    {
        btn = CCMenuItemImage::create("UI/greenbutton_nomal.png", "UI/greenbutton_pressed.png");
        btn->setPosition(ccp(background->getContentSize().width * btnXForce, background->getContentSize().height * (btnYForec + yDistance * 2)));
        
        label = CCLabelTTF::create(GameData::getText("boss_mode"), STANDARD_FONT_NAME, 20);
        label->setPosition(ccp(btn->getContentSize().width/2, btn->getContentSize().height/2));
        
        if (Player::getInstance()->getMapScore(mapId) >=90)
        {
            label->setColor(ccc3(255,255,0));
            btn->setTarget(this, menu_selector(WorldMap::enterBossStage));
            
        }
        else
        {
            char buffer[256] = {0};
            sprintf(buffer, "%s\n(%s)", GameData::getText("boss_mode"), GameData::getText("boss_unlock_requirement"));
            label->setString(buffer);
            label->setFontSize(14);
        }
        
        btn->addChild(label);
        menu->addChild(btn, 0, STAGE_INTRODUCTION_TAG + mapId);
        
    }
    else if (GameData::getMapConfig(mapId)->price > 0 && !Player::getInstance()->hasPurchasedMap(mapId))
    {
        char buffer[256] = {0};
        
        infoLabel->setString(GameData::getText("unlock_stage"));
        
        CCSprite* pSprite = CCSprite::create("gold.png");
        pSprite->setAnchorPoint(ccp(0,0.5));
        pSprite->setPosition(ccp(infoLabel->getPosition().x - infoLabel->getContentSize().width/2, infoLabel->getPosition().y - infoLabel->getContentSize().height/2 - pSprite->getContentSize().height/2));
        background->addChild(pSprite);
        
        sprintf(buffer, "%d", GameData::getMapConfig(mapId)->price);
        
        CCLabelTTF* mapPriceLabel = CCLabelTTF::create(buffer, STANDARD_NUMBER_FONT_NAME, 16);
        mapPriceLabel->setColor(ccc3(43,28,19));
        mapPriceLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
        mapPriceLabel->setPosition(ccp(pSprite->getPosition().x + pSprite->getContentSize().width + mapPriceLabel->getContentSize().width/2 + 10, pSprite->getPosition().y));
        background->addChild(mapPriceLabel);
        
        requirePurchase = true;
        
    }
        

    //    btnOk->setScale(background->getContentSize().width * 0.25 / btnOk->getContentSize().width);
    btn = CCMenuItemImage::create("UI/greenbutton_nomal.png", "UI/greenbutton_pressed.png");
    btn->setPosition(ccp(background->getContentSize().width * btnXForce, background->getContentSize().height * (btnYForec + yDistance * 1)));

    if (!requirePurchase)
    {
        string introduction = GameData::getMapConfig(mapId)->introduction;
        if(mapId >= 1000){
           label = CCLabelTTF::create(GameData::getText("endless_mode"), STANDARD_FONT_NAME, 20);
        }else{
            label = CCLabelTTF::create(GameData::getText("normal_mode"), STANDARD_FONT_NAME, 20);
        }
        btn->setTarget(this, menu_selector(WorldMap::enterStage));
    }
    else
    {
        label = CCLabelTTF::create(GameData::getText("buy"), STANDARD_FONT_NAME, 20);
        btn->setTarget(this, menu_selector(WorldMap::purchaseMap));
    }
    
    label->setPosition(ccp(btn->getContentSize().width/2, btn->getContentSize().height/2));
    label->setColor(ccc3(255,255,255));
    btn->addChild(label);
    menu->addChild(btn,0,STAGE_INTRODUCTION_TAG + mapId);

    
    btn = CCMenuItemImage::create("UI/redbutton_nomal.png", "UI/redbutton_pressed.png", this, menu_selector(WorldMap::removeStageInformation));
    btn->setPosition(ccp(background->getContentSize().width * btnXForce, background->getContentSize().height * btnYForec));
    label = CCLabelTTF::create(GameData::getText("close"), STANDARD_FONT_NAME, 20);
    label->setPosition(ccp(btn->getContentSize().width/2, btn->getContentSize().height/2));
    label->setColor(ccc3(255,255,255));
    btn->addChild(label);
    menu->addChild(btn);
    
    background->addChild(menu);
    
    m_pStageIntroductionNode = background;
    
}

void WorldMap::removeStageInformation(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    if (m_pStageIntroductionNode)
    {
        m_pStageIntroductionNode->removeFromParentAndCleanup(true);
        m_pStageIntroductionNode = NULL;
    }
}


void WorldMap::showServerMessage(const char *value)
{
    m_pWorldmapGameMenu->showServerMessage(value);
}


void WorldMap::purchaseMap(cocos2d::CCObject *pSender)
{
    CCMenuItem* pItem = static_cast<CCMenuItem*>(pSender);
    int mapId = pItem->getTag() - STAGE_INTRODUCTION_TAG;
    
    int price = GameData::getMapConfig(mapId)->price;
    
    if (Player::getInstance()->getMoney() >= price)
    {
        // set map as bought and reduce gems, then save
        Player::getInstance()->purchaseMap(mapId);
        
        enterStage(pSender);
    }
    else
    {
        // open iap menu
//        IAPMenu::create(-256);
        if (GameScene::getInstance()->sharedMainUI->getChildByTag(SALE_STORE_TAG))
        {
            return;
        }
        
        
        SaleStore* store = SaleStore::create();
        store->switchToCoinStore();
        GameScene::getInstance()->sharedMainUI->addChild(store, 0, SALE_STORE_TAG);

    }
    
}

void WorldMap::enterStage(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/choosemap.wav");

    m_pBackground->removeChildByTag(NEXT_MAP_ARROW_TAG, true);
    
    CCMenuItem* pItem = static_cast<CCMenuItem*>(pSender);
    int mapId = pItem->getTag() - STAGE_INTRODUCTION_TAG;
    
    LoadingScreen* pScreen = LoadingScreen::create(mapId);
    pScreen->setNormalStage(true);
    CCDirector::sharedDirector()->pushScene(pScreen);
    
    SystemHelper::removeGoogleBar();
    
//    GameScene::getInstance()->loadStageMap(mapId);
}

void WorldMap::enterBossStage(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/choosemap.wav");

    m_pBackground->removeChildByTag(NEXT_MAP_ARROW_TAG, true);
    
    CCMenuItem* pItem = static_cast<CCMenuItem*>(pSender);
    int mapId = pItem->getTag() - STAGE_INTRODUCTION_TAG;
    
    LoadingScreen* pScreen = LoadingScreen::create(mapId);
    pScreen->setNormalStage(false);
    CCDirector::sharedDirector()->pushScene(pScreen);
    //    GameScene::getInstance()->loadStageMap(mapId);
}

void WorldMap::showMainUI()
{
    CCScene* pScene = GameScene::getInstance();
    Player::getInstance()->save();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, pScene));
}

void WorldMap::update(float dt)
{
    
}
