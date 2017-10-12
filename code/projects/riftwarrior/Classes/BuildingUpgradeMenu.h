//
//  BuildingUpgradeMenu.h
//  ike-tdgame
//
//  Created by Like Zhang on 11/28/12.
//
//

#ifndef __ike_tdgame__BuildingUpgradeMenu__
#define __ike_tdgame__BuildingUpgradeMenu__

#include <iostream>
#include "enums.h"
#include "cocos2d.h"
using namespace cocos2d;

class Building;

class BuildingUpgradeMenu : public CCNode
{
public:
    
    static BuildingUpgradeMenu* create(Building* pBuilding);
    
    virtual bool init(Building* pBuilding);
    
    void setText(const char* pText);
    
private:

    void resetMenu();

    void selectUpgrade(CCObject* pSender);
    void selectUpgradeToNewBuilding(CCObject *pSender);
    
    void onSell(CCObject* pSender);
    void onUpgrade(CCObject* pSender);
    void onUpgradeToNewBuilding(CCObject* pSender);
    void onRepair(CCObject* pSender);

    void openIAPMenu(CCObject* pSender);
    
    CCLabelTTF* addGemCostLabel(CCNode* parent, int gemCost);
    int getSellPrice();
    
    Building* m_pBuilding;
    
    CCMenu* m_pMenu;
    CCMenuItem* m_pUpgradeIcon;
    CCMenuItem* m_pUpgradeToNewBuildingIcon;
    
    CCNode* m_pBackground;
};

#endif /* defined(__ike_tdgame__BuildingUpgradeMenu__) */
