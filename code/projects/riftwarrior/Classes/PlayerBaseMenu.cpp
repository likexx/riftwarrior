//
//  PlayerBaseMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 5/9/13.
//
//

#include "PlayerBaseMenu.h"
#include "Building.h"
#include "GameScene.h"
#include "Player.h"
#include "IAPMenu.h"
#include "Unit.h"
#include "constant.h"

PlayerBaseMenu* PlayerBaseMenu::create(Building* pBuilding)
{
    PlayerBaseMenu* pMenu = new PlayerBaseMenu();
    if (pMenu && pMenu->init(pBuilding))
    {
        pMenu->autorelease();
        return pMenu;
    }
    
    CC_SAFE_DELETE(pMenu);
    
    return NULL;
}

bool PlayerBaseMenu::init(Building* pBuilding)
{
    m_pBuilding = pBuilding;
    
    resetMenu();
    
    //    reloadData();
    
    //    this->setPosition(ccp(-120, -120));
    
    return true;
}

void PlayerBaseMenu::resetMenu()
{
    if (!Player::getInstance()->hasCompletedTutorial())
    {
        return;
    }
    
    this->removeAllChildrenWithCleanup(true);
    m_pMenu = NULL;
    m_pInfantryIcon = NULL;
    m_pTankIcon = NULL;
    
    CCSprite* background = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/background.png"));
    this->addChild(background);
    
    CCMenu* menu = CCMenu::create();
    menu->setContentSize(background->getContentSize());
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    
    m_pMenu = menu;
    

    
    float leftX = menu->getContentSize().width * 0.075;
    float rightX = menu->getContentSize().width * 0.925;
    float topY = menu->getContentSize().height * 0.775;
    float bottomY = menu->getContentSize().height * 0.025;
    
    CCPoint topLeft = ccp(leftX, topY);
    CCPoint topRight = ccp(rightX, topY);
    CCPoint bottomLeft = ccp(leftX, bottomY);
    CCPoint bottomRight = ccp(rightX, bottomY);
    CCPoint topCenter = ccp(menu->getContentSize().width/2, menu->getContentSize().height * 0.9);
    CCPoint bottomCenter = ccp(menu->getContentSize().width/2, menu->getContentSize().height * 0.025);
    
    if (m_pBuilding->getLevel() < 4)
    {
        // add icon to upgrade to next level

        CCPoint pos = topCenter;
        if (m_pBuilding->getLevel() == 3)
        {
            pos = topRight;
        }
        
        createBaseUpgradeIcon(pos);
        
    }
    

    // add infantry icon
    createUnitUpgradeIcon(101, m_pBuilding->getLevel() == 1 ? bottomCenter : bottomLeft, menu_selector(PlayerBaseMenu::upgradeInfantry));

    
    if (m_pBuilding->getLevel() >= 2)
    {
        createUnitUpgradeIcon(102, bottomRight, menu_selector(PlayerBaseMenu::upgradeMobileRocket));

    }
    
    
    if (m_pBuilding->getLevel() >= 3)
    {
        createUnitUpgradeIcon(103, topLeft, menu_selector(PlayerBaseMenu::upgradeRobot));

    }
    
    
    if (m_pBuilding->getLevel() == 4)
    {
        createUnitUpgradeIcon(104, topRight, menu_selector(PlayerBaseMenu::upgradeTank));
    }

    background->addChild(menu);
    m_pBackground = background;
    
    background->setScale(0.1f);
    background->runAction(CCScaleTo::create(0.2f, 1.0f));
    
    setVisible(true);
}

void PlayerBaseMenu::createBaseUpgradeIcon(const CCPoint& pos)
{
    createIcon("UI/building/upgrade_normal.png", "UI/building/upgrade_pressed.png", 0, 0, pos, menu_selector(PlayerBaseMenu::upgrade));
}

void PlayerBaseMenu::createUnitUpgradeIcon(int unitId, const cocos2d::CCPoint &pos, SEL_MenuHandler callback)
{
    char path1[128] = {0};
    char path2[128] = {0};
    
    sprintf(path1, "UI/building/unit%d_normal.png", unitId);
    sprintf(path2, "UI/building/unit%d_pressed.png", unitId);
    
    createIcon(path1, path2, 100, 2, pos, callback);
    
}

void PlayerBaseMenu::createIcon(const char *pImagePath, const char *pPressedImagePath, int coinCost, int gemCost, const cocos2d::CCPoint &pos, SEL_MenuHandler callback)
{
    CCSprite* image = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage(pImagePath));
    CCSprite* imagePressed = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage(pPressedImagePath));
    CCMenuItemSprite* icon = CCMenuItemSprite::create(image, imagePressed);
    
    icon->setPosition(pos);
    
    char data[128] = {0};
    sprintf(data, "%d", coinCost);
    
    CCLabelTTF* priceLabel = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 16);
    priceLabel->setPosition(ccp(icon->getContentSize().width/2, icon->getContentSize().height * 0.2));
    
    
    icon->addChild(priceLabel);
    
    CCLabelTTF* gemCostLabel = addGemCostLabel(icon, gemCost);
    
    if (coinCost <= Player::getInstance()->getMoney()&&
        Player::getInstance()->getGems() >= gemCost)
    {
        icon->setTarget(this, callback);
    }
    else
    {
        priceLabel->setColor(ccc3(255,0,0));
        gemCostLabel->setColor(ccc3(255, 0, 0));
    }
    
    m_pMenu->addChild(icon);
}

CCLabelTTF* PlayerBaseMenu::addGemCostLabel(CCNode* parent, int gemCost)
{
    char data[128] = {0};
    
    CCSprite* gemImg = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/gem.png"));
    gemImg->setScale(0.5);
    gemImg->setAnchorPoint(ccp(1,0.5));
    
    sprintf(data, "%d", gemCost);
    CCLabelTTF* gemCostLabel = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 16);
    gemCostLabel->setAnchorPoint(ccp(0, 0.5));
    
    CCLayerColor* gemBackground = CCLayerColor::create(ccc4(1,1,1,255), 64, gemCostLabel->getContentSize().height);
    gemBackground->setAnchorPoint(ccp(0.5, 1));
    gemBackground->setPosition(ccp(parent->getContentSize().width/8, -gemBackground->getContentSize().height/2));
    
    gemCostLabel->setPosition(ccp(gemBackground->getContentSize().width/2, gemBackground->getContentSize().height/2));
    gemImg->setPosition(ccp(gemBackground->getContentSize().width/2, gemBackground->getContentSize().height/2));
    
    gemBackground->addChild(gemImg);
    gemBackground->addChild(gemCostLabel);
    
    parent->addChild(gemBackground);
    
    return gemCostLabel;
}

void PlayerBaseMenu::setText(const char *pText)
{
    CCLabelTTF* label = CCLabelTTF::create(pText, STANDARD_NUMBER_FONT_NAME, 18);
    label->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
    addChild(label);
}


void PlayerBaseMenu::upgrade(cocos2d::CCObject *pSender)
{
    
    int cost = m_pBuilding->getUpgradeCost();
    if (Player::getInstance()->getMoney() < cost &&
        Player::getInstance()->getGems() < m_pBuilding->getUpgradeGemCost())
    {
        return;
    }
    
    Player::getInstance()->updateMoney(-cost);
    Player::getInstance()->updateGems(-m_pBuilding->getUpgradeGemCost());
    
    removeFromParentAndCleanup(true);
    
    m_pBuilding->upgradeLevel();
    
    m_pBuilding->showUpgradeAnimation();
    
}


void PlayerBaseMenu::upgradeInfantry(cocos2d::CCObject *pSender)
{
    
}

void PlayerBaseMenu::upgradeMobileRocket(cocos2d::CCObject *pSender)
{
    
}

void PlayerBaseMenu::upgradeRobot(cocos2d::CCObject *pSender)
{
    
}


void PlayerBaseMenu::upgradeTank(cocos2d::CCObject *pSender)
{
    
}



