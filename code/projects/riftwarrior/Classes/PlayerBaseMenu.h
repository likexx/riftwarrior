//
//  PlayerBaseMenu.h
//  tdgame
//
//  Created by Like Zhang on 5/9/13.
//
//

#ifndef __tdgame__PlayerBaseMenu__
#define __tdgame__PlayerBaseMenu__

#include <iostream>
#include "enums.h"
#include "cocos2d.h"
using namespace cocos2d;

class Building;

class PlayerBaseMenu : public CCNode
{
public:
    
    static PlayerBaseMenu* create(Building* pBuilding);
    
    virtual bool init(Building* pBuilding);
    
    void setText(const char* pText);
    
private:
    
    void resetMenu();
    
    void createBaseUpgradeIcon(const CCPoint& pos);
    
    void createUnitUpgradeIcon(int unitId, const CCPoint& pos, SEL_MenuHandler callback);
    
    void createIcon(const char * pImagePath, const char* pPressedImagePath, int coinCost, int gemCost, const CCPoint& pos, SEL_MenuHandler callback);
    
    void upgrade(CCObject* pSender);
    
    void upgradeInfantry(CCObject* pSender);
    void upgradeMobileRocket(CCObject* pSender);
    void upgradeRobot(CCObject* pSender);
    void upgradeTank(CCObject* pSender);
    
    
    CCLabelTTF* addGemCostLabel(CCNode* parent, int gemCost);
    int getSellPrice();
    
    Building* m_pBuilding;
    
    CCMenu* m_pMenu;
    CCMenuItem* m_pInfantryIcon;
    CCMenuItem* m_pTankIcon;
    
    CCNode* m_pBackground;
    
};

#endif /* defined(__tdgame__PlayerBaseMenu__) */
