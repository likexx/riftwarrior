//
//  SkillTreeMenu.cpp
//  tdgame
//
//  Created by Like Zhang on 3/4/13.
//
//

#include "SkillTreeMenu.h"
#include "GameData.h"
#include "Magic.h"
#include "BuildingSetting.h"
#include "Player.h"
#include "GameScene.h"
#include "KKMenu.h"
#include "IAPMenu.h"
#include "SaleStoreMenu.h"
#include "SimpleAudioEngine.h"
#include "SimplePlayerView.h"
#include "SystemHelper.h"
#include "constant.h"

#define SKILL_LEARN_GEM_COST 50

#define SKILL_POINT_TITLE_LABEL_TAG 1001
#define SKILL_SELECTED_ICON_TAG 1002

#define SKILL_LEVEL_LABEL_TAG 1003

#define RESET_SKILL_MENU_TAG 1004
#define SIDE_BAR_TAG 1005

#define BUILDING_ITEM_LABEL_TAG 10000
#define MAGIC_ITEM_LEVEL_LABEL_TAG 20000
#define SKILL_ITEM_TAG 30000
#define SKILL_LEARN_COST_GEMS 50
#define TAG_ADDSKILLPOINT_LAB 2003
#define TAG_ADDSKILLPOINT_GEMS 2001
#define TAG_ADDSKILLPOINT_MONEY 2002
#define STATE_OPENIAP 2006
#define STATE_LEARNSKILL 2007
#define TAG_NOT_ENOUGH_MONEY 20091

#define TAG_SKILLTIME_MASK 4000

#define TAG_MSGBOX_MAIN 3000
#define TAG_MSGBOX_MENU 3001
#define TAG_MSGBOX_OKBTN 3002
#define TAG_MSGBOX_CANCLEBTN 3003
#define TAG_MSGBOX_OKLABLE 3004
#define TAG_MSGBOX_CANCLELABLE 3005
#define TAG_MSGBOX_LABLE 3006

#define USE_FOR_TRAIN 3007
#define USE_FOR_COMPLETE 3008

bool SkillTreeMenu::init()
{

    m_IsInUse = false;
    m_RemoveOnClose = false;
    resetSkillTree();
    
    scheduleUpdate();
    
    return true;
}

void SkillTreeMenu::resetSkillTree()
{
    if (m_pSkillIconSelectedBackground)
    {
        m_pSkillIconSelectedBackground->removeFromParentAndCleanup(true);
        m_pSkillIconSelectedBackground = NULL;
    }
    
    this->removeAllChildrenWithCleanup(true);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    setContentSize(winSize);
    
    CCSprite* background = CCSprite::create("UI/skilltree/background.png");
    
     
    background->setAnchorPoint(ccp(0.5f,0.5f));
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    

    float scaleX = winSize.width/background->getContentSize().width;
    float scaleY = winSize.height/background->getContentSize().height;
    
    background->setScaleX(scaleX);
    background->setScaleY(scaleY);
    
    CCSize backgroundSize = background->getContentSize();
    
    addChild(background);
    m_pBackground = background;
    
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    menu->setContentSize(backgroundSize);
    m_pBackground->addChild(menu);
    
    m_pMenu = menu;
    
    
    showSkillTree(menu);
    
    
    m_SkillTitle = CCLabelTTF::create(GameData::getText("skill_tree"), "Helvetica-Bold", 24);
    m_SkillTitle->setColor(ccc3(1,1,1));
    m_SkillSummary = CCLabelTTF::create(GameData::getText("skill_tree_introduction"), "Helvetica", 20);
    m_SkillSummary->setColor(ccc3(1,1,1));
    m_SkillSummary->setHorizontalAlignment(kCCTextAlignmentLeft);
    
    m_countDown = CCLabelBMFont::create("","font/skilltree.fnt");
    m_countDown->setAnchorPoint(ccp(0.5f, 0.5f));
    m_countDown->setPosition(ccp(backgroundSize.width * 0.7f,backgroundSize.height * 0.875f));
    m_countDown->setScale(0.6f);
    
    m_pBackground->addChild(m_countDown);
    
    int x= backgroundSize.width * 0.65;
    float width = backgroundSize.width * 0.35;
    
    m_SkillTitle->setPosition(ccp(x + width * 0.22, menu->getContentSize().height*0.71 - m_SkillTitle->getContentSize().height/2));
    if (!m_SkillTitle->getParent())
    {
        m_pBackground->addChild(m_SkillTitle);
    }
    
    m_SkillSummary->setPosition(ccp(x + width * 0.25, m_SkillTitle->getPosition().y -  m_SkillTitle->getContentSize().height * 1.5));
    m_SkillSummary->setAnchorPoint(ccp(0.5, 1));
    if (!m_SkillSummary->getParent())
    {
        m_pBackground->addChild(m_SkillSummary);
    }
    
    m_AddSkillPointButton = CCMenuItemImage::create("UI/skilltree/button.png", "UI/skilltree/button_pressed.png");
    m_AddSkillPointButton->setPosition(ccp(x + width * 0.20,  m_AddSkillPointButton->getContentSize().height * 2));
    menu->addChild(m_AddSkillPointButton);

    CCLabelTTF *title = CCLabelTTF::create(GameData::getText("learn"), "Helvetica", 22);
    title->setColor(ccc3(1,1,1));
    title->setPosition(ccp(m_AddSkillPointButton->getContentSize().width/2, m_AddSkillPointButton->getContentSize().height/2));
     
    CCSprite* coin = CCSprite::create("UI/iap/coin1.png");
    coin->setAnchorPoint(ccp(0.0f, 0.5f));
    coin->setVisible(false);
    coin->setPosition(ccp(m_AddSkillPointButton->getContentSize().width * 0.17f, m_AddSkillPointButton->getContentSize().height * 0.5f));
    m_AddSkillPointButton->addChild(coin,0,TAG_ADDSKILLPOINT_MONEY);
    
    CCSprite* gem = CCSprite::create("UI/iap/gem1.png");
    gem->setAnchorPoint(ccp(0.0f, 0.5f));
    gem->setVisible(false);
    gem->setScale(0.55f);
    gem->setPosition(ccp(m_AddSkillPointButton->getContentSize().width * 0.07f, m_AddSkillPointButton->getContentSize().height * 0.5f));
    m_AddSkillPointButton->addChild(gem,0,TAG_ADDSKILLPOINT_GEMS);
    
    m_AddSkillPointButton->addChild(title,0,TAG_ADDSKILLPOINT_LAB);
    m_AddSkillPointButton->setVisible(false);
    
    m_closeSkillTreeButton = CCMenuItemImage::create("UI/skilltree/close.png", "UI/skilltree/close_pressed.png");
    m_closeSkillTreeButton->setAnchorPoint(ccp(1, 1));
    m_closeSkillTreeButton->setPosition(ccp(m_pBackground->getContentSize().width * 0.97,m_pBackground->getContentSize().height * 0.96));
    menu->addChild(m_closeSkillTreeButton);
    m_closeSkillTreeButton->setTarget(this, menu_selector(SkillTreeMenu::close));


    if (!m_RemoveOnClose)
    {
        if (isInUse())
        {
            m_closeSkillTreeButton->setTarget(this, menu_selector(SkillTreeMenu::close));
        }
    }
    
    m_CurrentSelectedSkillId = 0;
    
    CCSprite* sideBar = CCSprite::create("UI/side.png");
    sideBar->setPosition(ccp(m_pBackground->getContentSize().width/2, m_pBackground->getContentSize().height/2));
    m_pBackground->addChild(sideBar, 0, SIDE_BAR_TAG);
    sideBar->setVisible(false);

    SkillTreeMaskLayer* pMask = SkillTreeMaskLayer::create();
    pMask->setContentSize(m_pBackground->getContentSize());
    pMask->setAnchorPoint(CCPointZero);
    pMask->setPosition(CCPointZero);
    pMask->ignoreAnchorPointForPosition(false);
    m_pBackground->addChild(pMask);
    m_pMaskLayer = pMask;
    
    m_SkillTrainTime = CCLabelTTF::create("", STANDARD_NUMBER_FONT_NAME, 20);
    m_SkillTrainTime->setColor(ccc3(0, 0, 0));
    m_SkillTrainTime->setAnchorPoint(ccp(0.0f, 0.0f));
    m_pBackground->addChild(m_SkillTrainTime);
    
    CCNode* pPlayerView = SimplePlayerView::create();
    addChild(pPlayerView, 0);
    Function::scaleNodeForScreen(pPlayerView, ccp(0,1), ccp(0, winSize.height));
}

void SkillTreeMenu::showSkillTree(CCMenu* pMenu)
{
    
    CCSize backgroundSize = m_pBackground->getContentSize();
    const map<int, Skill*>& skills = GameData::getAllSkills();

    int maxLevel = 0;
    map<int, Skill*>::const_iterator it = skills.begin();
    while(it != skills.end())
    {
        Skill* pSkill = it->second;
        if (pSkill)
        {
            if (pSkill->requireLevel > maxLevel)
            {
                maxLevel = pSkill->requireLevel;
            }
        }
        
        it++;
    }
    
    map<int, list<Skill*> > skillsByLevel;
    for(int i=1;i<maxLevel;++i)
    {
        // max level
        skillsByLevel[i].clear();
    }

    it = skills.begin();
    while(it != skills.end())
    {
        Skill* pSkill = it->second;
        
        skillsByLevel[pSkill->requireLevel].push_back(pSkill);
        
        it++;
    }
    
    map<int, list<Skill*> >::iterator skillIt = skillsByLevel.begin();

    int j = 1;
    int count = 0; // put all skills in sequence, not by level
    
    while(skillIt != skillsByLevel.end())
    {
        list<Skill*>& skills = skillIt->second;

        if (skills.size() == 0)
        {
            skillIt++;
            continue;
        }
        
        list<Skill*>::iterator current = skills.begin();
        
        int i = 1;
        while(current != skills.end())
        {
            Skill* pSkill = *current;

            char buffer[128] = {0};
            sprintf(buffer, "UI/skilltree/skill%d_normal.png", pSkill->id);
            CCMenuItemImage* item = CCMenuItemImage::create(buffer, buffer);
            
            int x = (item->getContentSize().width * 1.2) * (0.95 + count%4);
            int y = pMenu->getContentSize().height - (item->getContentSize().height * 1.3) * (1 + count/4);
            
            item->setPosition(ccp(x, y));
            
            char title[128] = {0};
            
            if (Player::getInstance()->getLevel() >= pSkill->requireLevel)
            {
                sprintf(title, "%d/%d",Player::getInstance()->getSkillLevel(pSkill->id) , pSkill->maxLevel);
                CCLabelTTF* label = CCLabelTTF::create(title, "Helvetica", 24);
                label->setColor(ccc3(1,1,1));
                label->setAnchorPoint(ccp(0.5,1));
                label->setPosition(ccp(item->getContentSize().width/2, 0));
                item->addChild(label,0,SKILL_LEVEL_LABEL_TAG);

                item->setTarget(this, menu_selector(SkillTreeMenu::showSkillInfo));
                
            }
            else
            {
                CCLog("id: %d", pSkill->id);

                sprintf(title, GameData::getText("require_level"),pSkill->requireLevel);
                CCLabelTTF* label = CCLabelTTF::create(title, "Helvetica", 24);
                label->setAnchorPoint(ccp(0.5,1));
                label->setPosition(ccp(item->getContentSize().width/2, 0));
                label->setColor(ccc3(255,0,0));
                item->addChild(label);
                
                item->setOpacity(64);
            }

            pMenu->addChild(item, 0, SKILL_ITEM_TAG + pSkill->id);

            current++;
            i++;
            
            count++;
        }
        
        skillIt++;
        j++;
        
    }
}



void SkillTreeMenu::showSkillInfo(cocos2d::CCObject *pSender)
{
    if(this->createMsgWindow()->isVisible() == true){
        return;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    
    CCMenuItem* pItem = static_cast<CCMenuItem*>(pSender);
    
    int skillId = pItem->getTag() - SKILL_ITEM_TAG;
    
    if (m_pSkillIconSelectedBackground)
    {
        m_pSkillIconSelectedBackground->removeFromParentAndCleanup(true);
    }
    
    m_pBackground->removeChildByTag(SKILL_SELECTED_ICON_TAG, true);
    
    m_pSkillIconSelectedBackground = CCSprite::create("UI/skilltree/skill_select.png");
    m_pSkillIconSelectedBackground->setPosition(ccp(pItem->getContentSize().width/2, pItem->getContentSize().height/2));
    pItem->addChild(m_pSkillIconSelectedBackground, -1);
    
    const Skill* pSkill = GameData::getSkill(skillId);
    
    char buffer[128] = {0};
    sprintf(buffer, "UI/skilltree/skill%d_normal.png", pSkill->id);
    CCSprite* selectedIcon = CCSprite::create(buffer);
    float iconY = m_pMenu->getContentSize().height*0.75 - selectedIcon->getContentSize().height/2;

    selectedIcon->setPosition(ccp(m_SkillTitle->getPosition().x * 0.985, iconY));
    m_pBackground->addChild(selectedIcon, 0, SKILL_SELECTED_ICON_TAG);


    float titleX = m_SkillTitle->getPosition().x;
    float titleY = iconY - m_SkillTitle->getContentSize().height/2 - selectedIcon->getContentSize().height/2 - 10;
    
    float summaryX = m_SkillSummary->getPosition().x;
    float summaryY = titleY-  m_SkillTitle->getContentSize().height / 2 -10;
    
    
    m_SkillTitle->setPosition(ccp(titleX, titleY));
    m_SkillSummary->setPosition(ccp(summaryX, summaryY));
    
    CCString* pIntroduction;
    
    //bool hasEnoughMoney = Player::getInstance()->getMoney() >= Function::getSkillTrainingCost(pSkill->id);
    
    if (Player::getInstance()->getSkillLevel(skillId) < pSkill->maxLevel)
    {
        pIntroduction = CCString::createWithFormat(GameData::getText("learn_skill_require_skill_point"),
                                                   pSkill->introduction.c_str(),
                                                   Function::getSkillTrainingCost(pSkill->id));
    }
    else
    {
        pIntroduction = CCString::createWithFormat(GameData::getText("learn_skill_reaching_max_level"), pSkill->introduction.c_str());
    }
    
    m_SkillTitle->setString(pSkill->name.c_str());
    m_SkillTitle->setColor(ccc3(1,1,1));
    
    m_SkillSummary->setString(pIntroduction->getCString());
    
    if (Player::getInstance()->getSkillLevel(skillId) < pSkill->maxLevel){
        m_CurrentSelectedSkillId = skillId;
    }else{
        m_CurrentSelectedSkillId = 0;
        m_AddSkillPointButton->setVisible(false);
    }

    /*
    if (Player::getInstance()->getSkillLevel(skillId) < pSkill->maxLevel)
    {
        m_AddSkillPointButton->setVisible(true);
        m_CurrentSelectedSkillId = skillId;
        if(hasEnoughMoney){
            m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_GEMS)->setVisible(false);
            m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_MONEY)->setVisible(false);
            m_AddSkillPointButton->setTarget(this, menu_selector(SkillTreeMenu::trainSkill));            
        }else{
            m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_GEMS)->setVisible(false);
            m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_MONEY)->setVisible(true);
            m_AddSkillPointButton->setTarget(this, menu_selector(SkillTreeMenu::showSaleStore));
        }
    }else{
        m_AddSkillPointButton->setVisible(false);
    }*/
    
    if(m_CurrentSelectedSkillId != 0 && m_SkillSummary != NULL){
        char string[128] = {0};
        std::string timeString = Function::getTimeString(Function::getSkillTrainingTime(m_CurrentSelectedSkillId),TIME_STYLE_POINT);
        sprintf(string, GameData::getText("learn_skill_require_time"),timeString.c_str());
        m_SkillTrainTime->setString(string);
        CCPoint point = m_SkillSummary->getPosition();
        CCSize size = m_SkillSummary->getContentSize();
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        m_SkillTrainTime->setPosition(ccp((point.x - size.width / 2),(point.y - size.height - m_SkillTrainTime->getContentSize().height * 1.1)));
    }
}

void SkillTreeMenu::updateSkillInfo()
{
    if(m_CurrentSelectedSkillId == 0){
        return;
    }
    const Skill *skill = GameData::getSkill(m_CurrentSelectedSkillId);
    bool hasEnoughMoney = Player::getInstance()->getMoney() >= Function::getSkillTrainingCost(skill->id);
    bool isTraining = Player::getInstance()->isSkillAlreadyTraining(m_CurrentSelectedSkillId);
    if(m_SkillTrainTime != NULL){
        CCLabelTTF* waring = static_cast<CCLabelTTF*>(m_pBackground->getChildByTag(TAG_NOT_ENOUGH_MONEY));
        if(isTraining){
            if(waring != NULL){
                waring->setVisible(false);
            }
        }else if(!hasEnoughMoney){
            if(waring == NULL){
                waring = CCLabelTTF::create(GameData::getText("not_enough_money"), STANDARD_NUMBER_FONT_NAME, 20);
                waring->setColor(ccc3(255, 200, 50));
                m_pBackground->addChild(waring,0,TAG_NOT_ENOUGH_MONEY);
            }
            waring->setVisible(true);
            waring->setAnchorPoint(ccp(0.0f,0.0f));
            CCPoint point = m_SkillTrainTime->getPosition();
            CCSize size = waring->getContentSize();
            waring->setPosition(ccp(point.x,point.y - size.height * 1.1));
        }else if(Player::getInstance()->getMoney() >= skill->cost && waring != NULL){
            waring->setVisible(false);
        }
    }
    m_AddSkillPointButton->setVisible(true);
    if(isTraining){
        m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_GEMS)->setVisible(true);
        m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_MONEY)->setVisible(false);
        m_AddSkillPointButton->setTarget(this, menu_selector(SkillTreeMenu::showCompleteSkillMsg));
        static_cast<CCLabelTTF*>(m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_LAB))->setString(GameData::getText("skill_complete"));
        return;
    }
    static_cast<CCLabelTTF*>(m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_LAB))->setString(GameData::getText("learn"));
    if(hasEnoughMoney){
        m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_GEMS)->setVisible(false);
        m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_MONEY)->setVisible(false);
        m_AddSkillPointButton->setTarget(this, menu_selector(SkillTreeMenu::trainSkill));
    }else{
        m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_GEMS)->setVisible(false);
        m_AddSkillPointButton->getChildByTag(TAG_ADDSKILLPOINT_MONEY)->setVisible(true);
        m_AddSkillPointButton->setTarget(this, menu_selector(SkillTreeMenu::showSaleStore));
    }
}

void SkillTreeMenu::showCompleteSkillMsg()
{
    this->showMsgWindow(USE_FOR_COMPLETE);
}

void SkillTreeMenu::completeSkill()
{
    Player* player = Player::getInstance();
    player->completeSkillTrainingWithGems(m_CurrentSelectedSkillId, Function::getSkillInstantCompletionGemCost(m_CurrentSelectedSkillId));
}

void SkillTreeMenu::trainSkill(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    
    if (m_CurrentSelectedSkillId == 0)
    {
        return;
    }
    
    if(!Player::getInstance()->canTrainSkill())
    {
        this->showMsgWindow(USE_FOR_TRAIN);
        return;
    }
    
    bool startTraining = Player::getInstance()->tryStartTrainSkill(m_CurrentSelectedSkillId);
    
    if (startTraining)
    {
        
        const Skill* skill = GameData::getSkill(m_CurrentSelectedSkillId);
        std::string skillMsg;
        skillMsg += string(GameData::getText("your_skill")) + skill->name + string(GameData::getText("train_over"));
        SystemHelper::sendNotification(Function::getSkillTrainingTime(m_CurrentSelectedSkillId), skillMsg.c_str());
        CCLog("start training");
    }
    else
    {
        CCLog("failed");
    }
}

void SkillTreeMenu::showSaleStore()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    SaleStore::createAndShow();
}


void SkillTreeMenu::openIAPMenu()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    m_pBackground->removeChildByTag(RESET_SKILL_MENU_TAG, true);
    if(this->createMsgWindow()->isVisible()){
        this->closeMsgWindow();
    }
    IAPMenu::create(-256);
}

void SkillTreeMenu::setRemoveOnClose(bool value)
{
    m_RemoveOnClose = value;
}

void SkillTreeMenu::show(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    CCPoint currentPos = getPosition();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    runAction(CCMoveTo::create(0.2f, ccp(currentPos.x, 0.0f)));
    m_IsInUse = true;
    CCSprite* pBar = (CCSprite*)m_pBackground->getChildByTag(SIDE_BAR_TAG);
    if (pBar)
    {
        pBar->setVisible(true);
    }
}

void SkillTreeMenu::close(cocos2d::CCObject *pSender)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    if (CCDirector::sharedDirector()->isPaused())
    {
        CCDirector::sharedDirector()->resume();
    }
    
    if (GameScene::getInstance()->sharedMainUI->getStageUI())
    {
        Player::getInstance()->setProgressFlag(ENUM_FIRST_TIME_LEVEL_UP);
        GameScene::getInstance()->sharedMainUI->getStageUI()->removeArrow();
    }
    
    if (GameScene::getInstance()->sharedGameStage &&
        GameScene::getInstance()->sharedGameStage->isPaused())
    {
        GameScene::getInstance()->sharedGameStage->resume();
    }
    
    CCSprite* pBar = (CCSprite*)m_pBackground->getChildByTag(SIDE_BAR_TAG);
    if (pBar)
    {
        pBar->setVisible(false);
    }
    
    if (m_RemoveOnClose)
    {
        removeFromParentAndCleanup(true);
        return;
    }
    
    CCPoint currentPos = getPosition();
    runAction(CCMoveTo::create(0.2f, ccp(currentPos.x, size.height)));

    m_IsInUse = false;
}

void SkillTreeMenu::updateSkillTreeInfo()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int select = m_CurrentSelectedSkillId;
    this->resetSkillTree();
    if(select > 0){
        CCMenuItem* pItem = static_cast<CCMenuItem*>(m_pMenu->getChildByTag(SKILL_ITEM_TAG + select));
        this->showSkillInfo(pItem);
    }

}

void SkillTreeMenu::update(float dt)
{
    Player::getInstance()->updateTrainingSkills();

    map<int, Skill*>::const_iterator it= GameData::getAllSkills().begin();
    
    while (it != GameData::getAllSkills().end())
    {
        int id = it->first;
        
        unsigned long startTime = Player::getInstance()->getTrainingSkillStartTime(id);
        if(startTime == 0)
        {
            it++;
            m_pMaskLayer->setDrawProgress(id, CCPointZero, 0, 1.0f);

            continue;
        }
        
        CCNode* selectedItem = m_pMenu->getChildByTag(SKILL_ITEM_TAG + id);
        CCPoint screenPos = selectedItem->getParent()->convertToWorldSpace(selectedItem->getPosition());
        CCPoint currentPos = convertToNodeSpace(screenPos);
        
        float percentage = (float)(SystemHelper::getCurrentTimeInSeconds() - startTime)/(float)Function::getSkillTrainingTime(id);
        
        float size = selectedItem->getContentSize().width/2;
        m_pMaskLayer->setDrawProgress(id, screenPos, size, 1.0f - percentage);
        
        it++;
        
    }
    
    if(m_CurrentSelectedSkillId != 0 && Player::getInstance()->isSkillStartTraining(m_CurrentSelectedSkillId)){
        char buffer[256] = {0};
        sprintf(buffer,"%s %s", GameData::getText("learn_skill_time_remains"), Function::getTimeString(Player::getInstance()->getTrainingSkillLackTime(m_CurrentSelectedSkillId),TIME_STYLE_POINT).c_str());
        m_countDown->setString(buffer);
        
    }else{
        m_countDown->setString("");
    }
    updateSkillInfo();
}

CCSprite* SkillTreeMenu::createMsgWindow()
{
    CCSprite* result = NULL;
    
    m_pBackground->removeChildByTag(TAG_MSGBOX_MAIN);
    
    do {
        CCSize size = m_pBackground->getContentSize();
        result = CCSprite::create("UI/msg_background.png");
        CC_BREAK_IF(!result);
        result->setVisible(false);
        result->setPosition(ccp(size.width * 0.5f, size.height * 0.5f));
        m_pBackground->addChild(result, 256, TAG_MSGBOX_MAIN);
        
        CCMenu* menu = CCMenu::create();
        CC_BREAK_IF(!menu);
        menu->setPosition(CCPointZero);
        result->addChild(menu,0,TAG_MSGBOX_MENU);
        
        size = result->getContentSize();
        CCMenuItemImage* okButton = CCMenuItemImage::create("UI/greenbutton_nomal.png", "UI/greenbutton_pressed.png");
        okButton->setPosition(ccp(size.width * 0.25f,size.height * 0.2f));
        menu->addChild(okButton, 0, TAG_MSGBOX_OKBTN);
        
        CCLabelTTF* lable = CCLabelTTF::create(GameData::getText("learn"),STANDARD_NUMBER_FONT_NAME, 20);
        lable->setPosition(ccp(okButton->getContentSize().width * 0.5f,okButton->getContentSize().height * 0.5f));
        okButton->addChild(lable,0,TAG_MSGBOX_OKLABLE);
        
        CCMenuItemImage* cancleButton = CCMenuItemImage::create("UI/redbutton_nomal.png", "UI/redbutton_pressed.png");
        cancleButton->setPosition(ccp(size.width * 0.75f,size.height * 0.2f));
        menu->addChild(cancleButton, 0, TAG_MSGBOX_OKBTN);
        
        lable = CCLabelTTF::create(GameData::getText("close"),STANDARD_NUMBER_FONT_NAME, 20);
        lable->setPosition(ccp(cancleButton->getContentSize().width * 0.5f,cancleButton->getContentSize().height * 0.5f));
        cancleButton->setTarget(this, menu_selector(SkillTreeMenu::closeMsgWindow));
        cancleButton->addChild(lable,0,TAG_MSGBOX_CANCLELABLE);
        
        lable = CCLabelTTF::create(GameData::getText("close"),STANDARD_NUMBER_FONT_NAME, 22);
        lable->setPosition(ccp(size.width * 0.5f,size.height * 0.7f));
        lable->setColor(ccc3(0, 0, 0));
        result->addChild(lable, 0, TAG_MSGBOX_LABLE);
    } while (false);
    return result;
}

void SkillTreeMenu::showMsgWindow(int useFor)
{
    CCSprite* window = this->createMsgWindow();
    if(!window){
        return;
    }
    const Skill* skill = GameData::getSkill(m_CurrentSelectedSkillId);
    if(useFor == USE_FOR_TRAIN){
        char mainLable[256] = {0};
        int trainCost = Player::getInstance()->getGems() - SKILL_LEARN_GEM_COST;
        CCMenuItemImage* button = static_cast<CCMenuItemImage*>(window->getChildByTag(TAG_MSGBOX_MENU)->getChildByTag(TAG_MSGBOX_OKBTN));    
        if(trainCost >= 0){
            sprintf(mainLable, GameData::getText("learn_skill_by_using_gems"),SKILL_LEARN_GEM_COST);
            button->setTarget(this, menu_selector(SkillTreeMenu::learnSkillByGems));
        }else{
            sprintf(mainLable, GameData::getText("learn_skill_not_enough_gems"),trainCost * -1);
            button->setTarget(this, menu_selector(SkillTreeMenu::openIAPMenu));
            static_cast<CCLabelTTF*>(button->getChildByTag(TAG_MSGBOX_OKLABLE))->setString(GameData::getText("buy_iap_item"));
        }
        static_cast<CCLabelTTF*>(window->getChildByTag(TAG_MSGBOX_LABLE))->setString(mainLable);
    }else{
        char mainLabel[256] = {0};
        int gemsCost = Function::getSkillInstantCompletionGemCost(m_CurrentSelectedSkillId);
        int completeCost = Player::getInstance()->getGems() - gemsCost;
        CCMenuItemImage* button = static_cast<CCMenuItemImage*>(window->getChildByTag(TAG_MSGBOX_MENU)->getChildByTag(TAG_MSGBOX_OKBTN));
        if(completeCost >= 0){
            sprintf(mainLabel, GameData::getText("learn_skill_complete_now"),gemsCost,skill->name.c_str());
            button->setTarget(this, menu_selector(SkillTreeMenu::completeSkill));
        }else{
            sprintf(mainLabel, GameData::getText("skill_complete_not_enough"),skill->name.c_str(),completeCost * -1);
            button->setTarget(this, menu_selector(SkillTreeMenu::openIAPMenu));
            static_cast<CCLabelTTF*>(button->getChildByTag(TAG_MSGBOX_OKLABLE))->setString(GameData::getText("buy_iap_item"));
        }
        static_cast<CCLabelTTF*>(window->getChildByTag(TAG_MSGBOX_LABLE))->setString(mainLabel);
    }
    window->setVisible(true);
}

void SkillTreeMenu::closeMsgWindow()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    CCSprite* window = this->createMsgWindow();
    if(!window){
        return;
    }
    window->setVisible(false);
}

void SkillTreeMenu::learnSkillByGems()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/button.wav");
    if(Player::getInstance()->getGems() > SKILL_LEARN_GEM_COST){
        Player::getInstance()->updateGems(-SKILL_LEARN_GEM_COST);
        bool startTraining = Player::getInstance()->tryStartTrainSkill(m_CurrentSelectedSkillId);
        if (startTraining){
            const Skill* skill = GameData::getSkill(m_CurrentSelectedSkillId);
            std::string skillMsg;
            skillMsg += string(GameData::getText("your_skill")) + skill->name + string(GameData::getText("train_over"));
            SystemHelper::sendNotification(Function::getSkillTrainingTime(m_CurrentSelectedSkillId), skillMsg.c_str());
            CCLog("start training");
        }
        else{
            CCLog("failed");
        }
    }
    this->closeMsgWindow();
}

bool SkillTreeMaskLayer::init()
{
    m_DrawInProgressSkill = false;
    scheduleUpdate();
    
    return true;
}


//
// set in training items for drawing purpose
//
void SkillTreeMaskLayer::setDrawProgress(int id, const cocos2d::CCPoint pos, float size, float completionPercentage)
{
    if (completionPercentage >= 1)
    {
        map<int, SkillProgressMask>::iterator it = m_SkillMasks.find(id);
        if (it!=m_SkillMasks.end())
        {
            removeChildByTag(TAG_SKILLTIME_MASK + it->first, true);
            
            m_SkillMasks.erase(it);
            
        }
        
        return;
    }
    
    SkillProgressMask mask;
    mask.Position = convertToNodeSpace(pos);
    mask.Size = size;
    mask.Percentage = completionPercentage;
    
    m_SkillMasks[id] = mask;
}

//
// update the timer string on each training item
//
void SkillTreeMaskLayer::update(float dt)
{
    map<int, SkillProgressMask>::iterator it = m_SkillMasks.begin();
//    this->removeAllChildrenWithCleanup(true);

    while (it != m_SkillMasks.end())
    {
        CCLabelBMFont *time = static_cast<CCLabelBMFont *>(this->getChildByTag(TAG_SKILLTIME_MASK + it->first));
        if(time == NULL){
            time =  CCLabelBMFont::create("","font/arial.fnt",16);
            time->setPosition(it->second.Position);
            this->addChild(time, 100,TAG_SKILLTIME_MASK + it->first);
        }
        string timeDate = Function::getTimeString(Player::getInstance()->getTrainingSkillLackTime(it->first),TIME_STYLE_MAX);
        time->setString(timeDate.c_str());
        
        it++;
    }

}

void SkillTreeMaskLayer::draw()
{

    map<int, SkillProgressMask>::iterator it = m_SkillMasks.begin();
    while (it != m_SkillMasks.end())
    {

		Function::drawSolidFan(it->second.Position, it->second.Size, (float)M_PI_2, 2 * M_PI * it->second.Percentage, 20.0f, ccc4f(0,0,0,0.8f));
        it++;
    }
}
