//
//  StageUI.h
//  tdgame
//
//  Created by Like Zhang on 3/2/13.
//
//

#ifndef __tdgame__StageUI__
#define __tdgame__StageUI__

#include <iostream>
#include "enums.h"
#include "cocos2d.h"
#include "NewInfoMenu.h"

#define OPENINGWINDOW_TAG 9999

using namespace cocos2d;

class Building;
class MessageBox;
class PlayerStatusView;
class NpcMessageBox;
class GameControlMenu;
class PlayerMoveMenu;
class GameStatusView;
class SkillTreeMenu;

class StageUI : public CCNode
{
public:
    virtual bool init();
    virtual void update(float dt);
    
    CREATE_FUNC(StageUI);
    
    void reset();
    
    void addInfoButton(const int enemyId);
    void UpdateSkillTreeMenu();
    void updateStageWave(int currentWave, int maxWave);
    void updateStageWave(int currentWave);
    
    void showGameStatus();
    
    void showWinningCondition();
    void showGameOverMenu();
    void showGameSettingMenu();
    void showSkillTreeMenu();
    void showSlotMachine();
    void refreshActionMenu();
    
    void updatePlayerMoveMenu();
    
    void showWinningMenu();
    
    void showNpcStatusMenu(int npcId);
    void removeNpcStatusMenu();
    
    void onUserTapOnMap(const CCPoint& touchPos);
    void showBuildingUpgradeMenu(Building* pBuilding);
    void removeBuildingMenu();
    
    void showPlayerBaseMenu(Building* pBuilding);
    
    bool isInBuildingMenu();
    bool isInSkillTreeMenu();

    void showTutorialInstruction(ENUM_PROGRESS_FLAG progressFlag);
    
    void removeInstructions();
    void removeArrow();

    void showFirstTimeOnStageDone();
    void showMessageForFirstTimeGameStart();
    void finishInitialTutorial();
    void showInstructionForReachingLevel4();

    void showHint(const char* message);
    void removeHint();
    
    void showInstructionForSkillTree();
    
    void showMessage(const char* pMessage, bool pauseGame);
    void hideMessageBox();
    void hideNpcMessage();
    bool hasMessageBox();
    
    void showInfoMenu();
    
    inline PlayerMoveMenu* getPlayerMoveMenu()
    {
        return m_pPlayerMoveMenu;
    }
    
    inline MessageBox* getMessageBox()
    {
        return m_MessageBox;
    }
    
    inline GameControlMenu* getControlMenu(){
        return m_pControlMenu;
    }
    
    void showNpcMessage(const char* npcImage, const char* message);
    NpcMessageBox* getNpcMessageBox();
    
private:
    CCNode* m_PrevInstructionArrow;
    CCNode* m_pInstruction;
    
    CCNode* m_PrevInstructionMessageBox;
    PlayerStatusView* m_pPlayerStatusView;
    GameControlMenu* m_pControlMenu;
    NewInfoMenu* m_pInfoMenu;
    
    PlayerMoveMenu* m_pPlayerMoveMenu;
    
    GameStatusView* m_pGameStatusView;
    
//    CCLabelBMFont* m_WaveLabel;
    
    MessageBox* m_MessageBox;
    NpcMessageBox* m_NpcMessageBox;
    
    bool m_ArrowIsForSettingMenu;
    
    void showMessageForFirstTimeMovePlayer();
    
    void showInstructionForFirstTimePlayerMove(CCObject* pSender);

    void showInstructionForFirstTimeLightWeapon();
    void showInstructionForFirstTimeUpgrade();
    void showInstructionForFirstTimeUpgradeToNextGenerationWeapon();
    void showInstructionForFirstTimeMissleWeapon();
    
    void showInstructionForFirstTimeLevelUp();
    void showInstructionForFirstTimeGoingToDie();
    
    void showUnplaceable(const CCPoint& tilePos);
    
    void resumeGame();
    
    void removeUnnamedNode(CCNode* pNode);
    
};


#endif /* defined(__tdgame__StageUI__) */
