//
//  BuildingConstructionMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 3/6/13.
//
//

#include "BuildingMenu.h"
#include "GameScene.h"
#include "MapHelper.h"
#include "Player.h"
#include "MessageBox.h"
#include "NpcMessageBox.h"
#include "SimpleAudioEngine.h"
#include "SystemHelper.h"
#include "constant.h"

BuildingMenu* BuildingMenu::create(const cocos2d::CCPoint &tilePos)
{
    BuildingMenu* pMenu = new BuildingMenu();
    if(pMenu && pMenu->init(tilePos))
    {
        pMenu->autorelease();
        
        return pMenu;
    }
    
    CC_SAFE_DELETE(pMenu);
    return NULL;
}

bool BuildingMenu::init(const CCPoint& tilePos)
{
    
    CCSprite* background = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/background.png"));
    this->addChild(background);
    m_pBackground = background;
    
    CCMenu* menu = CCMenu::create();
    menu->setContentSize(background->getContentSize());
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    m_pMenu = menu;

    m_pSnipeWeaponItem = NULL;
    m_pShotWeaponItem = NULL;
    m_pAssistingWeaponItem = NULL;
    m_pMissleWeaponItem = NULL;

    background->addChild(menu);
    
    background->setScale(0.1f);
    background->runAction(CCScaleTo::create(0.2f, 1.0f));

    m_TilePos = tilePos;
    
    resetMenu();
    
    GameScene::getInstance()->sharedGameStage->getBuildingManager()->showBuildingToPlace(0, tilePos);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    return true;
}




void BuildingMenu::selectSnipeWeapon(cocos2d::CCObject *pSender)
{
    selectWeapon(2);
}

void BuildingMenu::selectMissleWeapon(cocos2d::CCObject *pSender)
{
    selectWeapon(1);
}

void BuildingMenu::selectAssistingWeapon(cocos2d::CCObject *pSender)
{
    selectWeapon(3);
}

void BuildingMenu::selectShotWeapon(cocos2d::CCObject *pSender)
{
    selectWeapon(4);
}


void BuildingMenu::resetMenu()
{
    
    float leftX = m_pMenu->getContentSize().width * 0.18;
    float rightX = m_pMenu->getContentSize().width * 0.82;
    float topY = m_pMenu->getContentSize().height * 0.8;
    float bottomY = m_pMenu->getContentSize().height * 0.175;
    
    CCPoint topLeft = ccp(leftX, topY);
    CCPoint topRight = ccp(rightX, topY);
    CCPoint bottomLeft = ccp(leftX, bottomY);
    CCPoint bottomRight = ccp(rightX, bottomY);
    
    if (m_pSnipeWeaponItem)
    {
        m_pSnipeWeaponItem->removeFromParentAndCleanup(true);
    }
    
    m_pSnipeWeaponItem = createWeaponMenuItem(2, topLeft, menu_selector(BuildingMenu::selectSnipeWeapon));
    
    unsigned int flag = Player::getInstance()->getProgressFlags();
    if (!(flag & ENUM_COMPLETE_BASIC_TUTORIAL) &&
        (flag & ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON))
    {
        ( static_cast<CCMenuItemImage*>(m_pSnipeWeaponItem) )->setColor(ccc3(128,128,128));
        m_pSnipeWeaponItem->setTarget(this, NULL);
    }
    else if (!(flag & ENUM_COMPLETE_BASIC_TUTORIAL) &&
             !(flag & ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON))
    {
        // blink for the 1st time

        m_pSnipeWeaponItem->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCTintBy::create(0.2f, 255, 0, 0),
                                                                                             CCTintBy::create(0.2f, 0, 255, 0),
                                                                                             CCTintBy::create(0.2f, 0, 0, 255),
                                                                                             NULL)));
    }
    
    if (m_pMissleWeaponItem)
    {
        m_pMissleWeaponItem->removeFromParentAndCleanup(true);
    }

    m_pMissleWeaponItem = createWeaponMenuItem(1, bottomLeft, menu_selector(BuildingMenu::selectMissleWeapon));

    if (!(flag & ENUM_COMPLETE_BASIC_TUTORIAL) &&
        !(flag & ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON))
    {
        static_cast<CCMenuItemImage*>(m_pMissleWeaponItem)->setColor(ccc3(128,128,128));
        m_pMissleWeaponItem->setTarget(this, NULL);
    }
    else if (!(flag & ENUM_COMPLETE_BASIC_TUTORIAL) &&
             (flag & ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON))
    {
        
        // blink for the 1st time
        m_pMissleWeaponItem->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCTintBy::create(0.2f, 255, 0, 0),
                                                                                             CCTintBy::create(0.2f, 0, 255, 0),
                                                                                             CCTintBy::create(0.2f, 0, 0, 255),
                                                                                             NULL)));
    }
    
    if (m_pAssistingWeaponItem)
    {
        m_pAssistingWeaponItem->removeFromParentAndCleanup(true);
    }
    m_pAssistingWeaponItem = createWeaponMenuItem(3, bottomRight, menu_selector(BuildingMenu::selectAssistingWeapon));

    
    if (m_pShotWeaponItem)
    {
        m_pShotWeaponItem->removeFromParentAndCleanup(true);
    }
    m_pShotWeaponItem = createWeaponMenuItem(4, topRight, menu_selector(BuildingMenu::selectShotWeapon));
    
}

CCMenuItem* BuildingMenu::createWeaponMenuItem(int weaponId, const cocos2d::CCPoint &pos, SEL_MenuHandler callback)
{
    CCMenuItemImage* weaponItem = NULL;
    
    const MapWinningCondition* pCondition = GameScene::getInstance()->sharedGameStage->getWinningCondition();
    
    bool isRestricted = pCondition->restrictedWeapons.find(weaponId) != pCondition->restrictedWeapons.end();
    
    if (Player::getInstance()->canUseBuilding(weaponId) &&
        !isRestricted)
    {
        char normalImgPath[128] = {0};
        char pressedImgPath[128] = {0};
        char price[128] = {0};
        
        sprintf(normalImgPath, "UI/building/building%d_normal.png", weaponId);
        sprintf(pressedImgPath, "UI/building/building%d_pressed.png", weaponId);
        
        weaponItem = CCMenuItemImage::create(normalImgPath, pressedImgPath, this, callback);
        
        Building* pBuilding = Building::create(weaponId, 0, 0, true);
        sprintf(price, "$%d", pBuilding->getBaseCost());
        CCLabelTTF* priceLabel = CCLabelTTF::create(price, STANDARD_NUMBER_FONT_NAME, 16);
        priceLabel->setAnchorPoint(ccp(0.5, 0));
        priceLabel->setPosition(ccp(weaponItem->getContentSize().width/2, weaponItem->getContentSize().width * 0.1));
        weaponItem->addChild(priceLabel);
    }
    else if (!isRestricted)
    {
        char data[128] = {0};
        sprintf(data, GameData::getText("require_level"), GameData::getBuildingSetting(weaponId).requireLevel);
        weaponItem = CCMenuItemImage::create("UI/building/lock_normal.png", "UI/building/lock_normal.png");
        CCLabelTTF* label = CCLabelTTF::create(data, STANDARD_FONT_NAME, 16);
        label->setPosition(ccp(weaponItem->getContentSize().width/2, weaponItem->getContentSize().height/2));
        weaponItem->addChild(label);
    }
    else
    {
        weaponItem = CCMenuItemImage::create("UI/building/lock_normal.png", "UI/building/lock_normal.png");
        CCLabelTTF* label = CCLabelTTF::create(GameData::getText("weapon_is_restricted"), STANDARD_FONT_NAME, 16);
        label->setPosition(ccp(weaponItem->getContentSize().width/2, weaponItem->getContentSize().height/2));
        weaponItem->addChild(label);
        
    }
    
    weaponItem->setPosition(pos);
    m_pMenu->addChild(weaponItem);
    
    return weaponItem;
}

void BuildingMenu::selectWeapon(int id)
{
    if (GameScene::getInstance()->sharedMainUI->getStageUI()->hasMessageBox())
    {
        return;
    }

    
    GameScene::getInstance()->sharedGameStage->removeUnplacedBuilding();
    GameScene::getInstance()->sharedGameStage->getBuildingManager()->showBuildingToPlace(id, m_TilePos);

    resetMenu();
    
    CCSprite* pWeaponIntroductionBackground = CCSprite::create("UI/building/introduction_background.png");

    CCLabelTTF* pWeaponName = CCLabelTTF::create("", "Arial Bold", 20);
    pWeaponName->setPosition(ccp(pWeaponIntroductionBackground->getContentSize().width/2, pWeaponIntroductionBackground->getContentSize().height * 0.8));

    CCLabelTTF* pWeaponIntroduction = CCLabelTTF::create("", STANDARD_NUMBER_FONT_NAME, 18);
    pWeaponIntroduction->setPosition(ccp(pWeaponIntroductionBackground->getContentSize().width/2, pWeaponIntroductionBackground->getContentSize().height * 0.4));
    pWeaponIntroduction->setHorizontalAlignment(kCCTextAlignmentLeft);
    pWeaponIntroductionBackground->addChild(pWeaponIntroduction);
    pWeaponIntroductionBackground->addChild(pWeaponName);
    
    CCMenuItemImage* pOkIcon = CCMenuItemImage::create("UI/ok.png", "UI/ok_pressed.png");
    
    m_pMenu->addChild(pOkIcon);
    
    m_pBackground->addChild(pWeaponIntroductionBackground);

    const BuildingSetting& setting = GameData::getBuildingSetting(id);
    Building* pTempBuilding = Building::create(id, 0, 0, true);
    
    int cost = pTempBuilding->getBaseCost();
    int playerMoney = Player::getInstance()->getMoney();
    
    
    CCPoint centerWolrdPos = pWeaponIntroductionBackground->getParent()->convertToWorldSpace(pWeaponIntroductionBackground->getPosition());
    CCPoint topLeftWolrdPos;
    CCPoint bottomRightWorldPos;
    
    bool buildable = playerMoney >= cost;
    CCMenuItem** ppItem = nullptr;
    char data[512]= {0};
    
    CCString* pTitleLabel = NULL;
    
    pTitleLabel = CCString::createWithFormat("%s", setting.name.c_str());
    sprintf(data, GameData::getText("building_info"),
            setting.introduction.c_str(),
            pTempBuilding->getDamage()/(setting.attackAnimationTime + setting.attackInterval),
            (int)pTempBuilding->getMaxHp());

    CCPoint topPos = ccp(m_pBackground->getContentSize().width/2, m_pBackground->getContentSize().height + pWeaponIntroductionBackground->getContentSize().height/2);
    CCPoint bottomPos = ccp(m_pBackground->getContentSize().width/2, -pWeaponIntroductionBackground->getContentSize().height/2);
    
    switch (id)
    {
        case 1:
            if (buildable)
            {
                pOkIcon->setTarget(this, menu_selector(BuildingMenu::buildMissleWeapon));
            }
            ppItem = &m_pMissleWeaponItem;
            

            pWeaponIntroductionBackground->setPosition(bottomPos);
            

            break;
        case 2:
            if (buildable)
            {
                pOkIcon->setTarget(this, menu_selector(BuildingMenu::buildSnipeWeapon));
            }
            ppItem = &m_pSnipeWeaponItem;

            pWeaponIntroductionBackground->setPosition(topPos);

            break;
        case 3:
            if (buildable)
            {
                pOkIcon->setTarget(this, menu_selector(BuildingMenu::buildAssistingWeapon));
            }
            ppItem = &m_pAssistingWeaponItem;

            pWeaponIntroductionBackground->setPosition(bottomPos);

            break;
        case 4:
            if (buildable)
            {
                pOkIcon->setTarget(this, menu_selector(BuildingMenu::buildShotWeapon));
            }
            ppItem = &m_pShotWeaponItem;
            
            pWeaponIntroductionBackground->setPosition(topPos);
            
            break;
            
    }
    
    if (!buildable)
    {
        // not enough money
        pWeaponIntroduction->setColor(ccc3(255,0,0));
    }
    else
    {
        pWeaponIntroduction->setColor(ccc3(255,255,255));
    }
    
    pWeaponIntroduction->setString(data);
    pWeaponName->setString(pTitleLabel->getCString());
    pWeaponName->setColor(ccc3(0,255,0));
    
    
    //adjust introduction position
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    
	if (ppItem != nullptr)
	{
		CCPoint pos = (*ppItem)->getPosition();
		(*ppItem)->removeFromParentAndCleanup(true);
		pOkIcon->setPosition(pos);

		*ppItem = pOkIcon;
	}
}

void BuildingMenu::buildWeapon(int id)
{
    if (GameScene::getInstance()->sharedMainUI->getStageUI()->hasMessageBox())
    {
        return;
    }
    
    const BuildingSetting& pSetting = GameData::getBuildingSetting(id);
    
    Player* pPlayer = Player::getInstance();
    
    if (pPlayer->getMoney() < pSetting.baseCost)
    {
        return;
    }

    BuildingManager* pManager = GameScene::getInstance()->sharedGameStage->getBuildingManager();
    
    
    if (!pManager->isPlaceable(m_TilePos.x, m_TilePos.y, id))
    {
        return;
    }
    
    GameScene::getInstance()->sharedGameStage->removeUnplacedBuilding();
    
    if (GameScene::getInstance()->sharedGameStage->getBuildingManager()->placeBuilding(m_TilePos.x, m_TilePos.y, id))
    {
        pPlayer->updateMoney(-pSetting.baseCost);
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/place_ok.wav");
        
        if (!(Player::getInstance()->getProgressFlags() & ENUM_COMPLETE_BASIC_TUTORIAL))
        {
            if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON) &&
                (Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_PLACE_SNIPE_WEAPON))
            {
                GameScene::getInstance()->sharedMainUI->getStageUI()->removeArrow();
                GameScene::getInstance()->sharedMainUI->getStageUI()->removeInstructions();
                GameScene::getInstance()->sharedMainUI->getStageUI()->hideMessageBox();
                
                NpcMessageBox* pMessageBox = GameScene::getInstance()->sharedMainUI->getStageUI()->getNpcMessageBox();
//                Player::getInstance()->showMessage(GameData::getText("first_light_weapon_ready"));
                
                pMessageBox->showMessage("npc/player_talk.png",GameData::getText("first_light_weapon_ready"));
                pMessageBox->setCallback(CCCallFunc::create(GameScene::getInstance()->sharedGameStage, callfunc_selector(GameStage::loadEnemies)));
            }
            else if (Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON)
            {
                GameScene::getInstance()->sharedMainUI->getStageUI()->removeArrow();
                GameScene::getInstance()->sharedMainUI->getStageUI()->removeInstructions();
                GameScene::getInstance()->sharedMainUI->getStageUI()->hideMessageBox();
                
                MessageBox* pMessageBox = GameScene::getInstance()->sharedMainUI->getStageUI()->getMessageBox();
                pMessageBox->showTextMessage(GameData::getText("first_missle_weapon_ready"), false);
                pMessageBox->setCallback(CCCallFunc::create(GameScene::getInstance()->sharedGameStage, callfunc_selector(GameStage::loadEnemies)));
                Player::getInstance()->setProgressFlag(ENUM_COMPLETE_BASIC_TUTORIAL);
                
                // flurry
                SystemHelper::logEvent("finish_tutorial");
            }
            
        }
        
        this->removeFromParentAndCleanup(true);
        
    }
    
 }

void BuildingMenu::buildAssistingWeapon(cocos2d::CCObject *pSender)
{
    buildWeapon(3);
}

void BuildingMenu::buildSnipeWeapon(cocos2d::CCObject *pSender)
{
    buildWeapon(2);
}

void BuildingMenu::buildMissleWeapon(cocos2d::CCObject *pSender)
{
    buildWeapon(1);
    
}

void BuildingMenu::buildShotWeapon(cocos2d::CCObject *pSender)
{
    buildWeapon(4);
}
