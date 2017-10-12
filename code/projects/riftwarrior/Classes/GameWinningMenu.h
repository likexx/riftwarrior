//
//  GameRoundWinMenu.h
//  tdgame
//
//  Created by Like Zhang on 3/18/13.
//
//

#ifndef __tdgame__GameWinningMenu__
#define __tdgame__GameWinningMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class GameWinningMenu : public CCLayerColor
{
public:
    virtual bool init();
    
    CREATE_FUNC(GameWinningMenu);
    
    virtual void update(float dt);
    
private:
    void share();
    void gotoNextMap(CCObject* pSender);    
    void gotoWorldMap(CCObject* pSender);


	void showEnemyKillerScore(float /* dummy parameter */) { return showEnemyKillerScore(); }
	void showEnemyKillerScore();
    
	void showTotalBuildingCreated(float /* dummy paramter */) { return showTotalBuildingCreated(); }
	void showTotalBuildingCreated();

	void showTotalBuildingDestroyed(float /* dummy paramter */) { return showTotalBuildingDestroyed(); }
	void showTotalBuildingDestroyed();

	void showLoseRate(float /* dummy parameter */) { return showLoseRate(); }
	void showLoseRate();
    
	void showAwardedExp(float /* dummy parameter*/) { return showAwardedExp(); }
	void showAwardedExp();

	void showAwardedMoney(float /* dummy parameter */) { return showAwardedMoney(); }
	void showAwardedMoney();
	
	void showButtons(float /* dummy parameter */) { return showButtons(); }
	void showButtons();

	void showStarLevel(float /* dummy parameter */) { return showStarLevel();  }
	void showStarLevel();

	void addStarCallFunc();
    //void updateAnimationValues();
    float getLoseRate();
    
    CCNode* m_pBackground;
    
    CCMenuItem* m_pCloseBtn;
//    CCMenuItem* m_pShareBtn;
    
    // for number animation;
    float m_CurrentValue;
    long m_MaxValue;
    int starCount;
    CCLabelBMFont* m_pTargetLabel;
    //float m_ValueIncrement; //per second
    SEL_SCHEDULE m_pNextCallback;
};
#endif /* defined(__tdgame__GameRoundWinMenu__) */
