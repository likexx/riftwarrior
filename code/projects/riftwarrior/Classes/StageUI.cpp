//
//  StageUI.cpp
//  tdgame
//
//  Created by Like Zhang on 3/2/13.
//
//

#include "StageUI.h"

#include "ActionMenu.h"
#include "PlayerStatusView.h"
#include "GameStatusView.h"
#include "GameControlMenu.h"
#include "GameOverMenu.h"
#include "GameSettingMenu.h"
#include "GameWinningMenu.h"
#include "GameScene.h"
#include "MapHelper.h"
#include "BuildingMenu.h"
#include "BuildingUpgradeMenu.h"
#include "StageMap.h"
#include "MessageBox.h"
#include "Player.h"
#include "SpecialActions.h"
#include "SmartAnimation.h"
#include "NpcMessageBox.h"
#include "PlayerMoveMenu.h"
#include "MiniGame/SlotMachine.h"
#include "AnimationManager.h"
#include "SystemHelper.h"
#include "WinningConditionMenu.h"
#include "NpcStatusMenu.h"
#include "SkillTreeMenu.h"
#include "PlayerBaseMenu.h"
#include "NpcMessageBox.h"
#include "SimpleAudioEngine.h"
#include "constant.h"

#define BUILDING_MENU_TAG 1001
#define ACTION_MENU_TAG 1002
#define PLAYER_STATUS_TAG 1003
#define GAME_STATUS_VIEW_TAG 1004

#define GAMEOVER_MENU_TAG 1010
#define MESSAGEBOX_TAG 1011
#define WINNING_BOX_TAG 1012
#define NPC_MESSAGE_BOX_TAG 1013
#define GAME_SETTING_MENU_TAG 1014
#define PLAYER_MOVE_MENU_TAG 1015
#define UNPLACEABLE_ERROR_SPRITE_TAG 1016
#define SLOT_MACHINE_LAYER_TAG 1017
#define NPC_STATUS_MENU_TAG 1018
#define WINNING_CONDITION_MENU_TAG 1019
#define HINT_TAG 1020
#define ARROW_TAG 1021
#define INFOMENU_TAG 1022
#define SKILLTREE_MENU_TAG 1023



bool StageUI::init()
{
    /*
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* mask = CCSprite::create("UI/stage_mask.png");
    mask->setPosition(ccp(winSize.width/2, winSize.height/2));
    mask->setScaleX(winSize.width/mask->getContentSize().width);
    mask->setScaleY(winSize.height/mask->getContentSize().height);
    */
//    ccBlendFunc bf = {GL_ONE, GL_ZERO};
//    mask->setBlendFunc(bf);
    
    /*
    BuildingMenu* buildingMenu = BuildingMenu::create();
    this->addChild(buildingMenu, 0, BUILDING_MENU_TAG);
    */

    reset();
    
    return true;
}

void StageUI::reset()
{
    unscheduleUpdate();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    setContentSize(winSize);
    
    m_PrevInstructionArrow = NULL;
    m_PrevInstructionMessageBox = NULL;
    m_pInstruction = NULL;
    
    m_ArrowIsForSettingMenu = false;
    
    ActionMenu* actionMenu = ActionMenu::create();
    addChild(actionMenu, 0, ACTION_MENU_TAG);
    Function::scaleNodeForScreen(actionMenu, ccp(1,0), ccp(winSize.width, 0));
    
    /*
    if (GameScene::getInstance()->sharedGameStage->getPlayerBase())
    {
        ActionIndication* actionIndication = ActionIndication::create();
        actionIndication->setAnchorPoint(CCPointZero);
        actionIndication->setContentSize(winSize);
        actionIndication->setActionMenu(actionMenu);
        addChild(actionIndication);
        
    }
    */
    
    
    
    addChild(m_pPlayerStatusView = PlayerStatusView::create(), 0, PLAYER_STATUS_TAG);
    Function::scaleNodeForScreen(m_pPlayerStatusView, ccp(0,1), ccp(0, winSize.height));

    
    // adjust player status view after scaling down
    /*
     CCPoint topLeft = ccp(0, winSize.height);
     m_pPlayerStatusView->setScale(0.75);
     CCPoint newPos = m_pPlayerStatusView->convertToWorldSpace(topLeft);
     
     CCPoint diff = ccpSub(topLeft, newPos);
     newPos = ccpAdd(m_pPlayerStatusView->getPosition(), diff);
     m_pPlayerStatusView->setPosition(newPos);
     */
    
    if(m_pControlMenu != NULL){
        this->removeChild(m_pControlMenu, true);
    }
    addChild(m_pControlMenu = GameControlMenu::create());
    Function::scaleNodeForScreen(m_pControlMenu, ccp(1,1), ccp(winSize.width, winSize.height));
    
    showInfoMenu();
    
    addChild(m_pPlayerMoveMenu = PlayerMoveMenu::create(), 0, PLAYER_MOVE_MENU_TAG);
    Function::scaleNodeForScreen(m_pPlayerMoveMenu, ccp(0,0), ccp(0,0));

    
    m_MessageBox = MessageBox::create();
    
    addChild(m_MessageBox, 128, MESSAGEBOX_TAG); // messagebox has the highest z-order
    
    m_NpcMessageBox = NpcMessageBox::create();
    addChild(m_NpcMessageBox, 128, NPC_MESSAGE_BOX_TAG);
    
    m_pGameStatusView = NULL;
    
    
    scheduleUpdate();
}

void StageUI::updatePlayerMoveMenu()
{
    m_pPlayerMoveMenu->updateButtonFunction();
}

void StageUI::showGameStatus()
{
    if (m_pGameStatusView)
    {
        m_pGameStatusView->removeFromParentAndCleanup(true);
    }
   
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    addChild(m_pGameStatusView = GameStatusView::create());
    Function::scaleNodeForScreen(m_pGameStatusView, ccp(0.5,1), ccp(winSize.width/2, winSize.height));
    
}

void StageUI::updateStageWave(int currentWave, int maxWave)
{
    if (m_pGameStatusView)
    {
        m_pGameStatusView->updateWave(currentWave, maxWave);
    }
}

void StageUI::updateStageWave(int currentWave)
{
    if (m_pGameStatusView)
    {
        m_pGameStatusView->updateWave(currentWave);
    }
}

void StageUI::refreshActionMenu()
{
    ActionMenu* menu = static_cast<ActionMenu*>(getChildByTag(ACTION_MENU_TAG));
    menu->reset();
}


void StageUI::showWinningCondition()
{
    CCNode* menu = getChildByTag(OPENINGWINDOW_TAG);
    if (menu)
    {
        return;
    }
    
    menu = WinningConditionMenu::create();
    addChild(menu, 0, OPENINGWINDOW_TAG);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    Function::scaleNodeForScreen(menu, ccp(0.5, 0.5), ccp(winSize.width/2, winSize.height/2));
/*
    float scale = winSize.height/640;
    
    menu->setContentSize(winSize);
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint(anchor);
    menu->setPosition(position);
    menu->setScale(scale);
*/
}

void StageUI::showGameOverMenu()
{

    CCNode* existingMenu = getChildByTag(OPENINGWINDOW_TAG);
    if (existingMenu)
    {
        return;
    }
    
    // save experiece and level
    int exp = Player::getInstance()->getExperience();
    int level = Player::getInstance()->getLevel();
    
    Player::getInstance()->setExperience(exp);
    Player::getInstance()->setLevel(level);

    GameScene::getInstance()->sharedMainUI->getStageUI()->getNpcMessageBox()->setCallback(NULL);
    GameScene::getInstance()->sharedMainUI->getStageUI()->getNpcMessageBox()->close(NULL);
    
    // flurry
    CCDictionary* params = CCDictionary::create();
    params->setObject(CCString::createWithFormat("%d", GameScene::getInstance()->sharedGameStage->getMapId()), "map_id");
    params->setObject(CCString::createWithFormat("%d", GameScene::getInstance()->sharedGameStage->getCurrentWave()), "wave");
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getLevel()), "player_level");
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getMoney()), "player_money");
    
    SystemHelper::logEvent("game_over", params, false);
    
    char eventName[128] = {0};
    sprintf(eventName, "play_map_%d", GameScene::getInstance()->sharedGameStage->getMapId());
    params = CCDictionary::create();
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getLevel()), "player_level");
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getMoney()), "player_money");
    params->setObject(CCString::create("lose"), "result");
    SystemHelper::endEvent(eventName, params);

    
    GameOverMenu* menu = GameOverMenu::create();
    this->addChild(menu, 0, OPENINGWINDOW_TAG);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    Function::scaleNodeForScreen(menu, ccp(0.5, 0.5f), ccp(winSize.width/2, winSize.height/2));
    
//    CCDirector::sharedDirector()->pause();
    GameScene::getInstance()->sharedGameStage->pause();
}

void StageUI::showGameSettingMenu()
{
    CCNode* existingMenu = getChildByTag(OPENINGWINDOW_TAG);
    if (existingMenu)
    {
        existingMenu->removeFromParentAndCleanup(true);
    }
    
    if (m_ArrowIsForSettingMenu)
    {
        removeArrow();
    }

    bool resumeAfterClose = false;
    if (!GameScene::getInstance()->sharedGameStage->isPaused())
    {
        GameScene::getInstance()->sharedGameStage->pause();
        resumeAfterClose = true;
    }

    GameSettingMenu* menu = GameSettingMenu::create();
    this->addChild(menu, 0, OPENINGWINDOW_TAG);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    Function::scaleNodeForScreen(menu, ccp(0.5, 0.5f), ccp(winSize.width/2, winSize.height / 2));
}

void StageUI::showSkillTreeMenu()
{
    if (GameScene::getInstance()->sharedGameStage &&
        GameScene::getInstance()->sharedGameStage->isPaused())
    {
        return;
    }
    
    
    if(!getChildByTag(SKILLTREE_MENU_TAG)){
        
        if (GameScene::getInstance()->sharedGameStage)
        {
            GameScene::getInstance()->sharedGameStage->pause();
        }
        
        SkillTreeMenu* pMenu = SkillTreeMenu::create();
        pMenu->setRemoveOnClose(true);
        addChild(pMenu, 0, SKILLTREE_MENU_TAG);
    }
}

bool StageUI::isInSkillTreeMenu()
{
    return getChildByTag(SKILLTREE_MENU_TAG) != NULL;
}

void StageUI::UpdateSkillTreeMenu()
{
    if(this->getChildrenCount() > 0){
        CCNode* child = getChildByTag(SKILLTREE_MENU_TAG);
        if(child != NULL)
        {
            SkillTreeMenu* menu = dynamic_cast<SkillTreeMenu*>(child);
            menu->updateSkillTreeInfo();
        }
    }
}

void StageUI::showInfoMenu()
{
    if(this->getChildByTag(INFOMENU_TAG) == NULL){
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        m_pInfoMenu = NewInfoMenu::create();
        m_pInfoMenu->setAnchorPoint(ccp(0.0f,0.0f));
        m_pInfoMenu->setPosition(ccp(0,winSize.height * 0.08));
        this->addChild(m_pInfoMenu, 0, INFOMENU_TAG);
    }
}

void StageUI::addInfoButton(const int enemyId)
{
    if(m_pInfoMenu != NULL){
        m_pInfoMenu->createNewInfo(enemyId);
    }
}


void StageUI::showNpcStatusMenu(int npcId)
{
    NpcStatusMenu* pMenu = NpcStatusMenu::create(npcId);
    
    CCPoint pos = m_pPlayerStatusView->getBottomPosition();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCPoint worldPos = m_pPlayerStatusView->convertToWorldSpace(pos);
    CCPoint currentPos = this->convertToNodeSpace(worldPos);

    float scale = winSize.width/640;
    float offset = (5 + pMenu->getContentSize().height/2) * scale;
    
    pMenu->setPosition(ccp(currentPos.x * 2 + 213, currentPos.y  ));
    pMenu->setScale(scale);
    
    addChild(pMenu, 0, NPC_STATUS_MENU_TAG);
//    scaleNodeForScreen(pMenu, ccp(0.5, 1), ccp(winSize.width/2, winSize.height));
}

void StageUI::removeNpcStatusMenu()
{
    removeChildByTag(NPC_STATUS_MENU_TAG, true);
}

void StageUI::onUserTapOnMap(const cocos2d::CCPoint &touchPos)
{
    if (Player::getInstance()->isDead())
    {
        return;
    }
    
    GameScene::getInstance()->sharedGameStage->getGroundMap()->removeChildByTag(UNPLACEABLE_ERROR_SPRITE_TAG, true);
 
    if (hasMessageBox() || GameScene::getInstance()->sharedGameStage->isCompleted())
    {
        return;
    }
    
    StageMap* pMap = GameScene::getInstance()->sharedGameStage->getGroundMap();
    CCPoint groundMapPos = GameScene::getInstance()->sharedGameStage->getGroundMap()->convertToNodeSpace(touchPos);
    CCPoint tilePos = MapHelper::groundMapPosToTilePos(groundMapPos, GameScene::getInstance()->sharedGameStage->getGroundMap(), false);

    // for tutorial
    if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_MOVE_PLAYER))
    {
        if (tilePos.x != pMap->getFirstTimeMoveTarget().x ||
            tilePos.y != pMap->getFirstTimeMoveTarget().y)
        {
            showUnplaceable(tilePos);
        }
        else
        {
            Player::getInstance()->setMoveDestination(groundMapPos, true);
            removeArrow();
        }
        
        return;
    }

    if (pMap->isRoadTile(tilePos))
    {
        Player::getInstance()->setMoveDestination(groundMapPos, true);
        
        return;
    }

    bool buildable = true;
    if (!Player::getInstance()->hasCompletedTutorial())
    {
        if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON))
        {
            if (!(Player::getInstance()->getProgressFlags() & ENUM_FIRST_TIME_PLACE_SNIPE_WEAPON))
            {
                // building tutorial not triggered yet
                buildable = false;
            }
            else if (tilePos.x != pMap->getFirstTimeLightWeaponPlaceTile().x ||
                     tilePos.y != pMap->getFirstTimeLightWeaponPlaceTile().y)
            {
                buildable = false;
            }
        }
        else if (tilePos.x != pMap->getFirstTimeMissleWeaponPlaceTile().x ||
                 tilePos.y != pMap->getFirstTimeMissleWeaponPlaceTile().y)
        {
            buildable = false;
        }

    }
    
    if (buildable)
    {
        buildable = Player::getInstance()->isBuildableTile(tilePos);
        
    }

    if (!buildable)
    {
        showUnplaceable(tilePos);
        return;
    }
    
    
    BuildingManager* pManager = GameScene::getInstance()->sharedGameStage->getBuildingManager();
    if (!pManager->isPlaceable(tilePos.x, tilePos.y, 0))
    {
        showUnplaceable(tilePos);
        return;
    }
    
    CCPoint pos = MapHelper::tilePosToGroundMapPos(ccp(tilePos.x + 0.5, tilePos.y + 0.5));
    CCPoint screenPos = GameScene::getInstance()->sharedGameStage->getGroundMap()->convertToWorldSpace(pos);
    BuildingMenu* menu = BuildingMenu::create(tilePos);
    menu->setPosition(this->convertToNodeSpace(screenPos));
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    menu->setScale(winSize.height/640);
    
    addChild(menu,0, BUILDING_MENU_TAG);

}

void StageUI::showUnplaceable(const cocos2d::CCPoint &tilePos)
{
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/place_error.wav");

    GameScene::getInstance()->sharedGameStage->getGroundMap()->removeChildByTag(UNPLACEABLE_ERROR_SPRITE_TAG, true);
    
    CCSprite* pUnplaceable = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("UI/unplaceable_tile2.png"));
    pUnplaceable->setAnchorPoint(ccp(0.5, 1));
    
    CCPoint pos = MapHelper::tilePosToGroundMapPos(tilePos);
    
    pUnplaceable->setPosition(pos);
    
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(pUnplaceable, 0, UNPLACEABLE_ERROR_SPRITE_TAG);
    
    pUnplaceable->runAction(CCSequence::create(CCDelayTime::create(1),
                                               CCFadeOut::create(1),
                                               CCRemoveSelf::create(),
                                               NULL));
    return;

}

void StageUI::removeInstructions()
{
    if(m_pInstruction)
    {
        m_pInstruction->removeFromParentAndCleanup(true);
        m_pInstruction = NULL;
    }
    
    GameScene::getInstance()->sharedGameStage->getGroundMap()->removeChildByTag(UNPLACEABLE_ERROR_SPRITE_TAG, true);
    
}

void StageUI::showBuildingUpgradeMenu(Building *pBuilding)
{
    /*
    if (isInBuildingMenu())
    {
        return;
    }
    */
    removeBuildingMenu();
    
    CCPoint pos = pBuilding->getParent()->convertToWorldSpace(pBuilding->getPosition());
    BuildingUpgradeMenu* pMenu = BuildingUpgradeMenu::create(pBuilding);
    pMenu->setPosition(pos);

    addChild(pMenu,0, BUILDING_MENU_TAG);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    pMenu->setScale(winSize.height/640);
}

void StageUI::showPlayerBaseMenu(Building *pBuilding)
{
    removeBuildingMenu();
    
    CCPoint pos = pBuilding->getParent()->convertToWorldSpace(pBuilding->getPosition());
    PlayerBaseMenu* pMenu = PlayerBaseMenu::create(pBuilding);
    pMenu->setPosition(pos);
    
    addChild(pMenu,0, BUILDING_MENU_TAG);
}


void StageUI::removeBuildingMenu()
{
    removeChildByTag(BUILDING_MENU_TAG, true);
}

bool StageUI::isInBuildingMenu()
{
    return getChildByTag(BUILDING_MENU_TAG) != NULL;
}

void StageUI::showTutorialInstruction(ENUM_PROGRESS_FLAG progressFlag)
{
    switch (progressFlag)
    {
        case ENUM_FIRST_TIME_MOVE_PLAYER:
            showInstructionForFirstTimePlayerMove(NULL);
            break;
        case ENUM_FIRST_TIME_PLACE_SNIPE_WEAPON:
            showInstructionForFirstTimeLightWeapon();
            break;
        case ENUM_FIRST_TIME_UPGRADE_WEAPON:
            showInstructionForFirstTimeUpgrade();
            break;
        case ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON:
            showInstructionForFirstTimeMissleWeapon();
            break;
        case ENUM_FIRST_TIME_GOING_TO_DIE:
            showInstructionForFirstTimeGoingToDie();
            break;
        case ENUM_FIRST_TIME_LEVEL_UP:
            showInstructionForFirstTimeLevelUp();
            break;
        case ENUM_FIRST_TIME_UPGRADE_NEXT_GENERATION_WEAPON:
            showInstructionForFirstTimeUpgradeToNextGenerationWeapon();
            break;
    }
    
}

void StageUI::showFirstTimeOnStageDone()
{

    Player::getInstance()->showMessage(GameData::getText("tutorial_stop_walking"));
    getNpcMessageBox()->setCallback(CCCallFunc::create(this, callfunc_selector(StageUI::showMessageForFirstTimeGameStart)));
}

void StageUI::showMessageForFirstTimeGameStart()
{
    removeInstructions();
    removeArrow();
    hideNpcMessage();
    m_MessageBox->showTextMessage(GameData::getText("tutorial_monsters_are_coming"), false);
    m_MessageBox->setCallback(CCCallFunc::create(this, callfunc_selector(StageUI::finishInitialTutorial)));
}

void StageUI::finishInitialTutorial()
{
    Player::getInstance()->setProgressFlag(ENUM_FIRST_TIME_ON_STAGE);
//    GameScene::getInstance()->sharedGameStage->showWinningCondition();
}

 void StageUI::showInstructionForFirstTimeLightWeapon()
{
    removeInstructions();
    removeArrow();
    
    m_MessageBox->showTextMessage(GameData::getText("tutorial_build_light_weapon"), false);
    
//    getNpcMessageBox()->setCallback(CCCallFunc::create(this, callfunc_selector(StageUI::showImagesForFirstTimePlaceBuilding)));
    
    CCPoint target = GameScene::getInstance()->sharedGameStage->getGroundMap()->getFirstTimeLightWeaponPlaceTile();
    CCPoint targetPos = MapHelper::tilePosToGroundMapPos(target);

    m_pInstruction = AnimationManager::getInstance()->createDestinationGoal();
    m_pInstruction->setAnchorPoint(ccp(0.5,1));
    m_pInstruction->setPosition(targetPos);
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(m_pInstruction);
    
    
    CCSprite* arrowDown = CCSprite::create("UI/arrow_down.png");
    arrowDown->setAnchorPoint(ccp(0.5,0.5));
    arrowDown->setPosition(targetPos);
    
    CCPoint moveToPos = MapHelper::tilePosToGroundMapPos(ccp(target.x, target.y - 1));
    moveToPos.x = targetPos.x;
    arrowDown->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCMoveTo::create(0.5, moveToPos),
                                                                                       CCMoveTo::create(0.5, targetPos),
                                                                                       NULL)));
    
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(arrowDown);
    
    
    m_PrevInstructionArrow = arrowDown;
    
    Player::getInstance()->setProgressFlag(ENUM_FIRST_TIME_PLACE_SNIPE_WEAPON);
//    m_MessageBox->setCallback(CCCallFunc::create(GameScene::getInstance()->sharedGameStage, callfunc_selector(GameStage::loadWave)));

}

void StageUI::showInstructionForFirstTimeUpgrade()
{
    removeInstructions();
    removeArrow();
    
    m_MessageBox->showTextMessage(GameData::getText("tutorial_first_time_upgrade_weapon"), false);
    
    //    getNpcMessageBox()->setCallback(CCCallFunc::create(this, callfunc_selector(StageUI::showImagesForFirstTimePlaceBuilding)));
    
    CCPoint target = GameScene::getInstance()->sharedGameStage->getGroundMap()->getFirstTimeLightWeaponPlaceTile();
    CCPoint targetPos = MapHelper::tilePosToGroundMapPos(target);
    
    m_pInstruction = AnimationManager::getInstance()->createDestinationGoal();
    m_pInstruction->setAnchorPoint(ccp(0.5,1));
    m_pInstruction->setPosition(targetPos);
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(m_pInstruction);
    
    
    CCSprite* arrowDown = CCSprite::create("UI/arrow_down.png");
    arrowDown->setAnchorPoint(ccp(0.5,0.5));
    arrowDown->setPosition(targetPos);
    
    CCPoint moveToPos = MapHelper::tilePosToGroundMapPos(ccp(target.x, target.y - 1));
    moveToPos.x = targetPos.x;
    arrowDown->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCMoveTo::create(0.5, moveToPos),
                                                                                       CCMoveTo::create(0.5, targetPos),
                                                                                       NULL)));
    
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(arrowDown);
    
    
    m_PrevInstructionArrow = arrowDown;
    
}

void StageUI::showInstructionForFirstTimeUpgradeToNextGenerationWeapon()
{
    removeInstructions();
    removeArrow();
    
    m_MessageBox->showMessage("UI/building/building6_normal.png", GameData::getText("tutorial_first_time_upgrade_to_firegun"), true);
}


void StageUI::showInstructionForFirstTimeMissleWeapon()
{
    removeInstructions();
    removeArrow();
    
    m_MessageBox->showMessage("UI/building/building1_normal.png", GameData::getText("tutorial_build_missle_weapon"), false);
    
    CCPoint target = GameScene::getInstance()->sharedGameStage->getGroundMap()->getFirstTimeMissleWeaponPlaceTile();
    CCPoint targetPos = MapHelper::tilePosToGroundMapPos(target);

    
    m_pInstruction = AnimationManager::getInstance()->createDestinationGoal();
    m_pInstruction->setAnchorPoint(ccp(0.5,1));
    m_pInstruction->setPosition(targetPos);
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(m_pInstruction);
    
    
    CCSprite* arrowDown = CCSprite::create("UI/arrow_down.png");
    arrowDown->setAnchorPoint(ccp(0.5,0.5));
    arrowDown->setPosition(targetPos);
    
    CCPoint moveToPos = MapHelper::tilePosToGroundMapPos(ccp(target.x, target.y - 1));
    moveToPos.x = targetPos.x;
    arrowDown->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCMoveTo::create(0.5, moveToPos),
                                                                                       CCMoveTo::create(0.5, targetPos),
                                                                                       NULL)));
    
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(arrowDown);
    
    
    m_PrevInstructionArrow = arrowDown;
    
    Player::getInstance()->setProgressFlag(ENUM_FIRST_TIME_PLACE_MISSLE_WEAPON);
}


void StageUI::showInstructionForFirstTimePlayerMove(cocos2d::CCObject *pSender)
{
    if (m_pInstruction)
    {
        // if instruction is alreay initialized, since it's the 1st instruction, we should return
        return;
    }
    
    
    CCPoint target = GameScene::getInstance()->sharedGameStage->getGroundMap()->getFirstTimeMoveTarget();
    CCPoint targetPos = MapHelper::tilePosToGroundMapPos(target);
    
    m_pInstruction = AnimationManager::getInstance()->createDestinationGoal();
    m_pInstruction->setAnchorPoint(ccp(0.5,1));
    m_pInstruction->setPosition(targetPos);
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(m_pInstruction);

    
    CCSprite* arrowDown = CCSprite::create("UI/arrow_down.png");
    arrowDown->setAnchorPoint(ccp(0.5,0.5));
    arrowDown->setPosition(targetPos);
    
    
    CCPoint moveToPos = MapHelper::tilePosToGroundMapPos(ccp(target.x, target.y - 1));
    moveToPos.x = targetPos.x;
    arrowDown->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCMoveTo::create(0.5, moveToPos),
                                                                                       CCMoveTo::create(0.5, targetPos),
                                                                                       NULL)));


    CCLabelTTF* pLabel = CCLabelTTF::create(GameData::getText("tutorial_first_move_instruction"), STANDARD_NUMBER_FONT_NAME, 16);
    pLabel->setColor(ccc3(255,255,255));
    pLabel->setAnchorPoint(ccp(0,0.5));
    pLabel->setPosition(ccp(arrowDown->getContentSize().width, arrowDown->getContentSize().height/2));
    arrowDown->addChild(pLabel);
    
    GameScene::getInstance()->sharedGameStage->getGroundMap()->addChild(arrowDown);
    
    
    
    m_PrevInstructionArrow = arrowDown;
    
}


void StageUI::removeUnnamedNode(cocos2d::CCNode *pNode)
{
    pNode->removeFromParentAndCleanup(true);
}

void StageUI::showInstructionForFirstTimeGoingToDie()
{
    removeInstructions();
    removeArrow();
    
    m_MessageBox->showTextMessage(GameData::getText("tutorial_less_hp"), true);
    
    Player::getInstance()->setProgressFlag(ENUM_FIRST_TIME_GOING_TO_DIE);
}

void StageUI::showInstructionForFirstTimeLevelUp()
{
    removeInstructions();
    removeArrow();
    
    m_MessageBox->showTextMessage(GameData::getText("tutorial_first_time_level_up"), true);
    m_MessageBox->setCallback(CCCallFunc::create(this, callfunc_selector(StageUI::showInstructionForSkillTree)));
    
}

void StageUI::showInstructionForReachingLevel4()
{
    removeInstructions();
    removeArrow();
    
    m_MessageBox->showTextMessage(GameData::getText("reaching_level_4"), true);
}

void StageUI::showInstructionForSkillTree()
{
    removeArrow();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* arrow = CCSprite::create("UI/arrow_down.png");
    arrow->setRotation(90);
    arrow->setAnchorPoint(ccp(0.5,0));
    
    
    CCNode* pSkillTreeIcon = m_pInfoMenu->getSkillTreeIconNode();
    CCPoint worldPos = pSkillTreeIcon->getParent()->convertToWorldSpace(pSkillTreeIcon->getPosition());
    CCPoint localPos = this->convertToNodeSpace(worldPos);
    CCPoint arrowPos = ccp(localPos.x + pSkillTreeIcon->getContentSize().width, localPos.y * 1.1);
    
    arrow->setPosition(arrowPos);
    
    CCPoint moveToPos = ccp(arrowPos.x + 30, arrowPos.y);
    arrow->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCMoveTo::create(0.5, moveToPos),
                                                                                       CCMoveTo::create(0.5, arrowPos),
                                                                                       NULL)));
    
    addChild(arrow);
    m_PrevInstructionArrow = arrow;
    
    m_ArrowIsForSettingMenu = true;
}

void StageUI::resumeGame()
{
    CCDirector::sharedDirector()->resume();
}

bool StageUI::hasMessageBox()
{
    return m_MessageBox->isVisible();
}

void StageUI::showMessage(const char *pMessage, bool pauseGame)
{
    
    m_MessageBox->showTextMessage(pMessage, pauseGame);
    
}

void StageUI::hideMessageBox()
{
    m_MessageBox->setVisible(false);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(m_MessageBox);
}

void StageUI::hideNpcMessage()
{
    m_NpcMessageBox->setVisible(false);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(m_NpcMessageBox);
}

void StageUI::removeArrow()
{
    if (m_PrevInstructionArrow)
    {
        m_PrevInstructionArrow->removeAllChildrenWithCleanup(true);
        m_PrevInstructionArrow->removeFromParentAndCleanup(true);
        m_PrevInstructionArrow = NULL;
    }
    
    if(GameScene::getInstance()->sharedGameStage->getGroundMap()->getChildByTag(ARROW_TAG))
    {
        GameScene::getInstance()->sharedGameStage->getGroundMap()->removeChildByTag(ARROW_TAG, true);
    }
    
}

void StageUI::showHint(const char *message)
{
    removeHint();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF* label = CCLabelTTF::create(message, "Helvetica", 22);
    label->setPosition(ccp(winSize.width/2, winSize.height * 0.3));
    addChild(label, 0, HINT_TAG);
}

void StageUI::removeHint()
{
    removeChildByTag(HINT_TAG, true);
}

void StageUI::showNpcMessage(const char *npcImage, const char *message)
{

    m_NpcMessageBox->showMessage(npcImage, message);
}

NpcMessageBox* StageUI::getNpcMessageBox()
{
    return m_NpcMessageBox;
}

void StageUI::showSlotMachine()
{
    CCNode* existingLayer = getChildByTag(OPENINGWINDOW_TAG);
    if (existingLayer)
    {
        return;
    }
    
    SlotMachine* machine = SlotMachine::create();
    this->addChild(machine, 0, OPENINGWINDOW_TAG);
}

void StageUI::showWinningMenu()
{
    CCNode* existingMenu = getChildByTag(OPENINGWINDOW_TAG);
    if (existingMenu)
    {
        return;
    }
    
    CCDictionary* params = CCDictionary::create();
    params->setObject(CCString::createWithFormat("%d", GameScene::getInstance()->sharedGameStage->getMapId()), "map_id");
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getLevel()), "player_level");
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getMoney()), "player_money");
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getKillCount()), "player_kill_enemies");
    SystemHelper::logEvent("game_win", params, false);
    
    char eventName[128] = {0};
    sprintf(eventName, "play_map_%d", GameScene::getInstance()->sharedGameStage->getMapId());
    
    params = CCDictionary::create();
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getLevel()), "player_level");
    params->setObject(CCString::createWithFormat("%d", Player::getInstance()->getMoney()), "player_money");
    params->setObject(CCString::create("win"), "result");
    SystemHelper::endEvent(eventName, params);

    
    GameWinningMenu* menu = GameWinningMenu::create();
    this->addChild(menu, 0, OPENINGWINDOW_TAG);
    GameScene::getInstance()->sharedGameStage->unscheduleUpdate();
    GameScene::getInstance()->sharedGameStage->pause();
//    CCDirector::sharedDirector()->pause();
}

void StageUI::update(float dt)
{
    
}


