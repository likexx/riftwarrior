//
//  BuildingUpgradeMenu.cpp
//  ike-tdgame
//
//  Created by Like Zhang on 11/28/12.
//
//

#include "BuildingUpgradeMenu.h"
#include "Building.h"
#include "GameScene.h"
#include "Player.h"
#include "IAPMenu.h"
#include "SimpleAudioEngine.h"
#include "constant.h"

BuildingUpgradeMenu* BuildingUpgradeMenu::create(Building* pBuilding)
{
    BuildingUpgradeMenu* pMenu = new BuildingUpgradeMenu();
    if (pMenu && pMenu->init(pBuilding))
    {
        pMenu->autorelease();
        return pMenu;
    }
    
    CC_SAFE_DELETE(pMenu);

    return NULL;
}

bool BuildingUpgradeMenu::init(Building* pBuilding)
{
    m_pBuilding = pBuilding;
    
    resetMenu();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

//    reloadData();
    
//    this->setPosition(ccp(-120, -120));
    
    return true;
}

void BuildingUpgradeMenu::resetMenu()
{
    if (!Player::getInstance()->hasCompletedTutorial())
    {
        return;
    }
    
    this->removeAllChildrenWithCleanup(true);
    m_pMenu = NULL;
    m_pUpgradeIcon = NULL;
    m_pUpgradeToNewBuildingIcon = NULL;
    
    char data[128] = {0};

    CCSprite* background = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/background.png"));
    this->addChild(background);
    
    CCMenu* menu = CCMenu::create();
    menu->setContentSize(background->getContentSize());
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    
    
    m_pMenu = menu;

    float leftX = m_pMenu->getContentSize().width * 0.18;
    float rightX = m_pMenu->getContentSize().width * 0.82;
    float topY = m_pMenu->getContentSize().height * 0.8;
    float bottomY = m_pMenu->getContentSize().height * 0.175;
    
    CCPoint topLeft = ccp(leftX, topY);
    CCPoint topRight = ccp(rightX, topY);
    CCPoint bottomLeft = ccp(leftX, bottomY);
    CCPoint bottomRight = ccp(rightX, bottomY);

    
    const BuildingSetting* pNextBuilding = GameData::getNextBuildingSetting(m_pBuilding->getId());
    
    bool firstTimeUpgrade = !(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_UPGRADE_WEAPON);
    
    
    if (m_pBuilding->getLevel() < m_pBuilding->getMaxLevel())
    {
        // add icon to upgrade to next level
        CCSprite* upgradeImage = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/upgrade_normal.png"));
        CCSprite* upgradeImagePressed = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/upgrade_pressed.png"));
        CCMenuItemSprite* upgradeIcon = CCMenuItemSprite::create(upgradeImage, upgradeImagePressed);
        upgradeIcon->setPosition(topLeft);
        
        sprintf(data, "%d", m_pBuilding->getUpgradeCost());

        CCLabelTTF* priceLabel = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 16);
        priceLabel->setPosition(ccp(upgradeIcon->getContentSize().width/2, upgradeIcon->getContentSize().height * 0.2));
        
        
        upgradeIcon->addChild(priceLabel);
        
        
//        CCLabelTTF* gemCostLabel = addGemCostLabel(upgradeIcon, m_pBuilding->getUpgradeGemCost());
        
        if (m_pBuilding->getUpgradeCost() <= Player::getInstance()->getMoney()&&
            Player::getInstance()->getGems() >= m_pBuilding->getUpgradeGemCost())
        {
            upgradeIcon->setTarget(this, menu_selector(BuildingUpgradeMenu::onUpgrade));
        }
        else
        {
            priceLabel->setColor(ccc3(255,0,0));
//            gemCostLabel->setColor(ccc3(255, 0, 0));
        }
        
        menu->addChild(upgradeIcon);
        
        m_pUpgradeIcon = upgradeIcon;

    }
    else if (m_pBuilding->getLevel() >= m_pBuilding->getMaxLevel())
    {
        // add repair icon
        CCSprite* repairImage = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/repair_normal.png"));
        CCSprite* repairPressed = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/repair_pressed.png"));
        CCMenuItemSprite* repairIcon = CCMenuItemSprite::create(repairImage, repairPressed);
        repairIcon->setPosition(topLeft);
        
        CCLabelTTF* label;
        
        sprintf(data, GameData::getText("building_upgrade_repair_label"), (int)(m_pBuilding->getRepairCost()));
        label = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 16);

        if (m_pBuilding->getRepairCost()<=Player::getInstance()->getMoney())
        {
            // each building has 1 chance to be repaired without using gems
            repairIcon->setTarget(this, menu_selector(BuildingUpgradeMenu::onRepair));
        }
        else
        {
            if (m_pBuilding->getRepairGemCost() == 0)
            {
                // cannot be repaired with gems
                label->setColor(ccc3(255,0,0));
            }
            else
            {
                // use gems to repair
//                CCLabelTTF* gemCostLabel = addGemCostLabel(repairIcon, m_pBuilding->getRepairGemCost());
                
                if (Player::getInstance()->getGems() < m_pBuilding->getRepairGemCost())
                {
//                    gemCostLabel->setColor(ccc3(255, 0, 0));
                }
                else
                {
                    repairIcon->setTarget(this, menu_selector(BuildingUpgradeMenu::onRepair));
                }
                
            }

        }
        
        label->setPosition(ccp(repairIcon->getContentSize().width/2, repairIcon->getContentSize().height * 0.1));
        label->setAnchorPoint(ccp(0.5,0));
        repairIcon->addChild(label);
        
        m_pUpgradeIcon = repairIcon;
        
        menu->addChild(repairIcon);
        
    }

    
    // add sell icon
    CCSprite* sellImage = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/sell_normal.png"));
    CCMenuItemSprite* sellIcon = CCMenuItemSprite::create(sellImage, sellImage, this, menu_selector(BuildingUpgradeMenu::onSell));
    sellIcon->setPosition(bottomLeft);
    if (firstTimeUpgrade)
    {
        sellIcon->setTarget(this, NULL);
        sellIcon->setColor(ccc3(1,1,1));
    }
    
    menu->addChild(sellIcon);
    sprintf(data, "$%d", getSellPrice());
    
    CCLabelTTF* label = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 16);
    label->setPosition(ccp(sellIcon->getContentSize().width/2, sellIcon->getContentSize().height * 0.2));
    sellIcon->addChild(label);

    // add icon for new building
    if (!firstTimeUpgrade && pNextBuilding && Player::getInstance()->canUseBuilding(pNextBuilding->id))
    {
        
        CCSprite* newBuildingImage = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage(CCString::createWithFormat("UI/building/building%d_normal.png", pNextBuilding->id)->getCString()));
        CCSprite* newBuildingImagePressed = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage(CCString::createWithFormat("UI/building/building%d_pressed.png", pNextBuilding->id)->getCString()));
        CCMenuItemSprite* newBuildingIcon = CCMenuItemSprite::create(newBuildingImage, newBuildingImagePressed);
        newBuildingIcon->setPosition(topRight);
        
        const BuildingSetting* pNextBuildingSetting = GameData::getNextBuildingSetting(m_pBuilding->getId());
        Building* pNextBuilding = Building::create(pNextBuildingSetting->id, 0, 0, true);
        int cost = pNextBuilding->getBaseCost();
        
        
        sprintf(data, "$%d", cost);
        CCLabelTTF* priceLabel = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 16);
        newBuildingIcon->addChild(priceLabel);
        

//        CCLabelTTF* gemLabel = addGemCostLabel(newBuildingIcon, pNextBuilding->getGemCost());
        
        
        if (cost <= Player::getInstance()->getMoney() &&
            Player::getInstance()->getGems() >= pNextBuilding->getGemCost())
        {
            newBuildingIcon->setTarget(this, menu_selector(BuildingUpgradeMenu::selectUpgradeToNewBuilding));
        }
        else
        {
            priceLabel->setColor(ccc3(255,0,0));
//            gemLabel->setColor(ccc3(255,0,0));
        }
 
        priceLabel->setPosition(ccp(newBuildingIcon->getContentSize().width/2, newBuildingIcon->getContentSize().height * 0.2));
 
        menu->addChild(newBuildingIcon);
        
        m_pUpgradeToNewBuildingIcon = newBuildingIcon;
    }
    else if (pNextBuilding)
    {
        // has next new building but cannot use
        CCSprite* lockImage = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/building/lock_normal.png"));
        
        CCMenuItemSprite* lockIcon = CCMenuItemSprite::create(lockImage, lockImage);
        lockIcon->setPosition(topRight);
        
        CCLabelTTF* priceLabel = CCLabelTTF::create(CCString::createWithFormat(GameData::getText("new_building_require_level"), pNextBuilding->requireLevel)->getCString(), STANDARD_NUMBER_FONT_NAME, 16);
        priceLabel->setPosition(ccp(lockIcon->getContentSize().width/2, lockIcon->getContentSize().height/2));
        lockIcon->addChild(priceLabel);
        
        
        menu->addChild(lockIcon);
        
        if (firstTimeUpgrade)
        {
            lockIcon->setTarget(this, NULL);
            lockIcon->setColor(ccc3(1,1,1));
        }
        
        m_pUpgradeToNewBuildingIcon = lockIcon;
        
    }
    
    // add button for IAP
    CCMenuItemImage* iapBtn = CCMenuItemImage::create("UI/building/gem_icon.png", "UI/building/gem_icon.png");
    iapBtn->setPosition(bottomRight);
    iapBtn->setTarget(this, menu_selector(BuildingUpgradeMenu::openIAPMenu));
    if (firstTimeUpgrade)
    {
        iapBtn->setTarget(this, NULL);
        iapBtn->setColor(ccc3(1,1,1));
    }
    
    menu->addChild(iapBtn);

        
    background->addChild(menu);
    m_pBackground = background;
    
    background->setScale(0.1f);
    background->runAction(CCScaleTo::create(0.2f, 1.0f));

    setVisible(true);
}

CCLabelTTF* BuildingUpgradeMenu::addGemCostLabel(CCNode* parent, int gemCost)
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

void BuildingUpgradeMenu::setText(const char *pText)
{
    CCLabelTTF* label = CCLabelTTF::create(pText, STANDARD_NUMBER_FONT_NAME, 18);
    label->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
    addChild(label);
}

void BuildingUpgradeMenu::onSell(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    CCPoint tilePos = m_pBuilding->getOriginTilePosition();
    
    

    int soldPrice = getSellPrice();
    
    removeFromParentAndCleanup(true);
    
    m_pBuilding->remove();
    

    Player::getInstance()->updateMoney(soldPrice);
}

void BuildingUpgradeMenu::selectUpgrade(cocos2d::CCObject *pSender)
{
    resetMenu();
    
    const BuildingSetting& setting = GameData::getBuildingSetting(m_pBuilding->getId());
    int cost = m_pBuilding->getUpgradeCost();
    int playerMoney = Player::getInstance()->getMoney();
    bool buildable = playerMoney >= cost;
    
    // show next level introduction
    CCSprite* pWeaponIntroductionBackground = CCSprite::create("UI/building/introduction_background.png");
    CCLabelTTF* pWeaponIntroduction = CCLabelTTF::create("", STANDARD_NUMBER_FONT_NAME, 20);
    pWeaponIntroduction->setPosition(ccp(pWeaponIntroductionBackground->getContentSize().width/2, pWeaponIntroductionBackground->getContentSize().height/2));
    pWeaponIntroduction->setHorizontalAlignment(kCCTextAlignmentLeft);
    pWeaponIntroductionBackground->addChild(pWeaponIntroduction);

    Building* pNextBuilding = Building::create(m_pBuilding->getId(), 0, 0, true);  // it should be auto released

    while(pNextBuilding->getLevel() != m_pBuilding->getLevel() + 1)
    {
        pNextBuilding->upgradeLevel();
    }
    
    char data[512] = {0};
    sprintf(data, GameData::getText("building_upgrade_info"),
            setting.name.c_str(),
            setting.introduction.c_str(),
            m_pBuilding->getDamage(),
            pNextBuilding->getDamage(),
            (int)m_pBuilding->getMaxHp(),
            (int)pNextBuilding->getMaxHp(),
            (int)m_pBuilding->getAttackRange(),
            (int)pNextBuilding->getAttackRange(),
            cost,
            buildable ? "" : GameData::getText("not_enough_money"));

    pWeaponIntroduction->setString(data);

    
    if (!buildable)
    {
        pWeaponIntroduction->setColor(ccc3(255,0,0));
    }
    
    m_pBackground->addChild(pWeaponIntroductionBackground);
    pWeaponIntroductionBackground->setAnchorPoint(ccp(0.5, 0));
 
    // by default, on the top
    pWeaponIntroductionBackground->setPosition(ccp(m_pBackground->getContentSize().width/2, m_pBackground->getContentSize().height));

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint backgroundWorldPos = m_pBackground->getParent()->convertToWorldSpace(m_pBackground->getPosition());
    if (backgroundWorldPos.y + m_pBackground->getContentSize().height/2 + pWeaponIntroductionBackground->getContentSize().height >= winSize.height)
    {
        // over the top
        pWeaponIntroductionBackground->setPosition(ccp(pWeaponIntroductionBackground->getPosition().x, -m_pBackground->getContentSize().height));
    }
    if (backgroundWorldPos.x + m_pBackground->getContentSize().width/2 + pWeaponIntroductionBackground->getContentSize().width >= winSize.width)
    {
        // over the right side
        pWeaponIntroductionBackground->setPosition(ccp(-pWeaponIntroductionBackground->getContentSize().width/2, pWeaponIntroductionBackground->getPosition().y));
    }
    else if (backgroundWorldPos.x - m_pBackground->getContentSize().width/2 - pWeaponIntroductionBackground->getContentSize().width <= 0)
    {
        // over the left side
        pWeaponIntroductionBackground->setPosition(ccp(pWeaponIntroductionBackground->getContentSize().width/2 + m_pBackground->getContentSize().width/2, pWeaponIntroductionBackground->getPosition().y));
    }
    
    if (buildable)
    {
        CCMenuItemImage* pOkIcon = CCMenuItemImage::create("UI/ok.png", "UI/ok_pressed.png");
        pOkIcon->setPosition(m_pUpgradeIcon->getPosition());
        pOkIcon->setTarget(this, menu_selector(BuildingUpgradeMenu::onUpgrade));
        m_pUpgradeIcon->removeFromParentAndCleanup(true);
        m_pMenu->addChild(pOkIcon);
    }

}

void BuildingUpgradeMenu::onUpgrade(cocos2d::CCObject *pSender)
{

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

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
    
    if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_UPGRADE_WEAPON))
    {
        Player::getInstance()->setProgressFlag(ENUM_FIRST_TIME_UPGRADE_WEAPON);
        GameScene::getInstance()->sharedMainUI->getStageUI()->removeArrow();
        GameScene::getInstance()->sharedMainUI->getStageUI()->removeInstructions();
        
    }
    
}


void BuildingUpgradeMenu::selectUpgradeToNewBuilding(cocos2d::CCObject *pSender)
{
//    resetMenu();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/select.wav");

    const BuildingSetting* pNextBuildingSetting = GameData::getNextBuildingSetting(m_pBuilding->getId());
    
    GameScene::getInstance()->sharedGameStage->getBuildingManager()->showBuildingToPlace(pNextBuildingSetting->id, m_pBuilding->getOriginTilePosition());
    
    Building* pNextBuilding = Building::create(pNextBuildingSetting->id, 0, 0, true);

    char message[128] = {0};
    
    int cost = pNextBuilding->getBaseCost();
    int playerMoney = Player::getInstance()->getMoney();

    bool buildable = true;
    
    if (pNextBuildingSetting->requireLevel > Player::getInstance()->getLevel())
    {
        sprintf(message, GameData::getText("require_level"), pNextBuildingSetting->requireLevel);
        buildable = false;
    }
    else if (cost > playerMoney)
    {
        sprintf(message, GameData::getText("not_enough_money"));
        buildable = false;
    }
    else if (Player::getInstance()->getGems()< pNextBuilding->getGemCost())
    {
        sprintf(message, GameData::getText("not_enough_gems"));
        buildable = false;
        
    }
    
    
    
    // show next level introduction
    CCSprite* pWeaponIntroductionBackground = CCSprite::create("UI/building/introduction_background.png");
    CCLabelTTF* pWeaponIntroduction = CCLabelTTF::create("", STANDARD_NUMBER_FONT_NAME, 20);
    pWeaponIntroduction->setPosition(ccp(pWeaponIntroductionBackground->getContentSize().width/2, pWeaponIntroductionBackground->getContentSize().height * 0.4));
    pWeaponIntroduction->setHorizontalAlignment(kCCTextAlignmentLeft);
    pWeaponIntroductionBackground->addChild(pWeaponIntroduction);
    
    
    char data[512] = {0};
    
    if (buildable)
    {
        sprintf(data, GameData::getText("building_info"),
                pNextBuildingSetting->introduction.c_str(),
                pNextBuilding->getDamage(),
                (int)pNextBuilding->getMaxHp()
                );
        
        pWeaponIntroduction->setString(data);
    }
    else
    {
        pWeaponIntroduction->setString(message);
        GameScene::getInstance()->sharedGameStage->getBuildingManager()->getUnplacedBuilding()->getRotatingPart()->setColor(ccc3(255,0,0));
    }
    
    pWeaponIntroduction->setColor(ccc3(255,255,255));

    if (pNextBuildingSetting->gemCost > 0)
    {
        sprintf(data, "%s[%s %d]", pNextBuildingSetting->name.c_str(), GameData::getText("requires_gems"), pNextBuilding->getGemCost());
    }
    else
    {
        sprintf(data, "%s", pNextBuildingSetting->name.c_str());
    }
    
    CCLabelTTF* weaponNameLabel = CCLabelTTF::create(data, STANDARD_NUMBER_FONT_NAME, 20);
    weaponNameLabel->setPosition(ccp(pWeaponIntroduction->getPosition().x, pWeaponIntroduction->getPosition().y + pWeaponIntroduction->getContentSize().height/2 + 8));
    weaponNameLabel->setColor(ccc3(0,255,0));
    pWeaponIntroductionBackground->addChild(weaponNameLabel);
    
    
    m_pBackground->addChild(pWeaponIntroductionBackground);
    pWeaponIntroductionBackground->setAnchorPoint(ccp(0.5, 0));
    
    // by default, on the top
    pWeaponIntroductionBackground->setPosition(ccp(m_pBackground->getContentSize().width/2, m_pBackground->getContentSize().height));
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint backgroundWorldPos = m_pBackground->getParent()->convertToWorldSpace(m_pBackground->getPosition());
    if (backgroundWorldPos.y + m_pBackground->getContentSize().height/2 + pWeaponIntroductionBackground->getContentSize().height >= winSize.height)
    {
        // over the top
        pWeaponIntroductionBackground->setPosition(ccp(pWeaponIntroductionBackground->getPosition().x, -m_pBackground->getContentSize().height));
    }
    if (backgroundWorldPos.x + m_pBackground->getContentSize().width/2 + pWeaponIntroductionBackground->getContentSize().width >= winSize.width)
    {
        // over the right side
        pWeaponIntroductionBackground->setPosition(ccp(-pWeaponIntroductionBackground->getContentSize().width/2, pWeaponIntroductionBackground->getPosition().y));
    }
    else if (backgroundWorldPos.x - m_pBackground->getContentSize().width/2 - pWeaponIntroductionBackground->getContentSize().width <= 0)
    {
        // over the left side
        pWeaponIntroductionBackground->setPosition(ccp(pWeaponIntroductionBackground->getContentSize().width/2 + m_pBackground->getContentSize().width/2, pWeaponIntroductionBackground->getPosition().y));
    }
    
    if (buildable)
    {
        CCMenuItemImage* pOkIcon = CCMenuItemImage::create("UI/ok.png", "UI/ok_pressed.png");
        pOkIcon->setPosition(m_pUpgradeToNewBuildingIcon->getPosition());
        pOkIcon->setTarget(this, menu_selector(BuildingUpgradeMenu::onUpgradeToNewBuilding));
        m_pUpgradeToNewBuildingIcon->removeFromParentAndCleanup(true);
        m_pMenu->addChild(pOkIcon);
    }
    
    
}

void BuildingUpgradeMenu::openIAPMenu(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    removeFromParentAndCleanup(true);
    GameScene::getInstance()->sharedGameStage->getBuildingManager()->removeBuildingToPlace();

    GameScene::getInstance()->sharedGameStage->pause();
    
    IAPMenu::create(-256);
}

void BuildingUpgradeMenu::onUpgradeToNewBuilding(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    
    BuildingManager* pBuildingManager = GameScene::getInstance()->sharedGameStage->getBuildingManager();

    pBuildingManager->removeBuildingToPlace();

    CCPoint tilePos = m_pBuilding->getOriginTilePosition();
    const BuildingSetting* pSetting = GameData::getNextBuildingSetting(m_pBuilding->getId());
    Building* pNextBuilding = Building::create(pSetting->id, 0, 0, true);
    
    if (!pNextBuilding)
    {
        return;
    }
    
    Player* pPlayer = Player::getInstance();
    
    int cost = pNextBuilding->getBaseCost();
    if (pPlayer->getMoney() < cost)
    {
        return;
    }
    
    if (pPlayer->getGems() < pNextBuilding->getGemCost())
    {
        return;
    }
    
    removeFromParentAndCleanup(true);
    
    
//    pBuildingManager->removeBuilding(tilePos.x, tilePos.y);
    m_pBuilding->remove();
    
    if (pBuildingManager->placeBuilding(tilePos.x, tilePos.y, pNextBuilding->getId(), true))
    {
        pBuildingManager->adjustTotalBuildingDestroyed(-1);
        pPlayer->updateMoney(-cost);
        pPlayer->updateGems(-pNextBuilding->getGemCost());
        
        pPlayer->setProgressFlag(ENUM_FIRST_TIME_UPGRADE_NEXT_GENERATION_WEAPON);
    }
    
}


void BuildingUpgradeMenu::onRepair(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");

    int cost = m_pBuilding->getRepairCost();
    
    if (Player::getInstance()->getGems() < m_pBuilding->getRepairGemCost())
    {
        return;
    }
    
    if (Player::getInstance()->getMoney() < cost)
    {
        return;
    }
    
    Player::getInstance()->updateMoney(-cost);

    
    Player::getInstance()->updateGems(-m_pBuilding->getRepairGemCost());

    
    removeFromParentAndCleanup(true);
    
    m_pBuilding->repair();
    
}


int BuildingUpgradeMenu::getSellPrice()
{
    const BuildingSetting& setting = GameData::getBuildingSetting(m_pBuilding->getId());
    
    return setting.baseCost * m_pBuilding->getLevel() * 0.5;
}

