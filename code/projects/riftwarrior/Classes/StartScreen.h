//
//  StartMenu.h
//  ike-tdgame
//
//  Created by Like Zhang on 12/1/12.
//
//

#ifndef __ike_tdgame__StartMenu__
#define __ike_tdgame__StartMenu__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

class StartScreen : public CCLayer,
                    public CCBSelectorResolver,
                    public CCBMemberVariableAssigner,
                    public CCNodeLoaderListener
{
public:
    static StartScreen* createNewScreen();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(StartScreen, create);
    
    virtual void update(float dt);
    
    virtual void onEnter();
    
	void startNewGame(CCObject * /* dummy parameter to pass macro cccontrol_selector */, 
		CCControlEvent /* dummy parameter to pass macro cccontrol_selector */);

	void continueGame(CCObject * /* dummy parameter to pass macro cccontrol_selector */,
		CCControlEvent /* dummy parameter to pass macro cccontrol_selector */);
    
    //void switchLanguage();
    
	void openMenu(CCObject * /* dummy parameter to pass macro cccontrol_selector */,
		CCControlEvent /* dummy parameter to pass macro cccontrol_selector */);
    
    //void openForumUrl();
    //void showCredits();
    
    void createNewGame();
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
    
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

    void resetButtonLabels();
    
    CCSprite* background;
    CCSprite* leftImage;
    CCSprite* rightImage;
    CCSprite* logoImage;
    
    CCControlButton* firstButton;
    CCControlButton* settingButton;
    
    CCLabelTTF* m_pCopyrightLabel;


    
private:
    void setupBackgroundEffect();
    
    void initButtonAction();
    
    //void closeCredits(CCObject* pSender);
    void screenLock();    
    void screenUnlock();
    int timeUniformLocation;
    float totalTime;
    bool isShowSettingMenu;
};

#endif /* defined(__ike_tdgame__StartMenu__) */
