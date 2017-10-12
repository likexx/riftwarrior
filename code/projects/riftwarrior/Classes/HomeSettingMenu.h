//
//  HomeSettingMenu.h
//  tdgame
//
//  Created by Wang Nero on 13-7-10.
//
//

#ifndef __tdgame__HomeSettingMenu__
#define __tdgame__HomeSettingMenu__

#include "cocos2d.h"

using namespace cocos2d;

//HomeSettingMenu
class HomeSettingMenu : public CCNode
{
public:
    virtual bool init();
    CREATE_FUNC(HomeSettingMenu);
    void setCloseCallAction(CCCallFunc *const closeCall);
private:
    //create
    bool restart();
    bool createMainMenu();
    bool createBackground();
    void resetSelectLayerColor(int layerTagToW,int layerTagToB);
    //buttonFunc
    void closeMyself();
    void creditsDown();    
    void settingsDown();
    //found
    CCCallFunc* closeCall;
    CCLayer* foundBackgroundLayer();
    //foraction
    void closeActonFunc();
};

//
namespace homeSettingmenu {
    class SettingLayer : public CCLayer
    {
    public:
        static SettingLayer* getInstance(CCLayer* const parent);
        virtual bool init();
        CREATE_FUNC(SettingLayer);
    private:
        //create
        bool restart();
        bool createMenu();
        bool createButton();
        //buttonFunc
		void sharingDown(CCObject* /* dummy parameter */);
        void switchLanDown(CCObject* /* dummy parameter */);
    };
    
    class CreditsLayer : public CCLayer
    {
    public:
        static CreditsLayer* getInstance(CCLayer* const parent);
        virtual bool init();
        CREATE_FUNC(CreditsLayer);
    private:
        bool restart();
        bool createCreditList();
    };
}


#endif /* defined(__tdgame__HomeSettingMenu__) */
