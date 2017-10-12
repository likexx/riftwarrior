//
//  GameScene.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/13/12.
//
//

#ifndef __ike_tdgame__GameScene__
#define __ike_tdgame__GameScene__

#include <iostream>
#include <cocos2d.h>
#include "GameStage.h"
#include "MainUI.h"

using namespace cocos2d;

class GameScene : public CCScene
{
public:
    static GameScene* getInstance();
    
    virtual ~GameScene();
    
    void cameraShot();
    void loadStageOpeningAnimation(int mapId);
    void loadStageEndingAnimation(int mapId);
    void setupPlayScreen(int mapId, bool isNormalStage);
    
    void loadData();
    void loadStageMap(int mapId, bool isNormalStage = true);
    void exitStageMap();
    
    CCScene* sharedScene;
    GameStage* sharedGameStage;
    
    MainUI* sharedMainUI;
    
    static const int GAME_MESSAGE_DIALOG_TAG = 111;

private:
	GameScene();

    void removeShotLayer(CCNode* pSender);
    
    void clearStage();

    void loadStageAnimation(int mapId, bool isOpening, const char* pPath);
    void playStage1Sound();
    
    bool m_DataLoaded;

};

#endif /* __ike_tdgame__GameScene__ */
