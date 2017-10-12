//
//  BuildingMenu.h
//  tdgame
//
//  Created by Like Zhang on 3/6/13.
//
//

#ifndef __tdgame__BuildingMenu__
#define __tdgame__BuildingMenu__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class BuildingMenu : public CCNode
{
public:
    
    static BuildingMenu* create(const CCPoint& tilePos);
    
    virtual bool init(const CCPoint& tilePos);
    
private:
    
    void selectSnipeWeapon(CCObject *pSender);
    void selectShotWeapon(CCObject *pSender);
    void selectAssistingWeapon(CCObject *pSender);
    void selectMissleWeapon(CCObject *pSender);
    
    void buildSnipeWeapon(CCObject *pSender);
    void buildShotWeapon(CCObject *pSender);
    void buildAssistingWeapon(CCObject *pSender);
    void buildMissleWeapon(CCObject *pSender);
    
    CCMenuItem* createWeaponMenuItem(int weaponId, const CCPoint& pos, SEL_MenuHandler callback);

    void selectWeapon(int id);
    void resetMenu();
    
    void buildWeapon(int id);
    
    void cancel(CCObject *pSender);
    
    CCPoint m_TilePos;
    
    CCNode* m_pBackground;
    CCMenu* m_pMenu;
    CCMenuItem* m_pSnipeWeaponItem;
    CCMenuItem* m_pShotWeaponItem;
    CCMenuItem* m_pAssistingWeaponItem;
    CCMenuItem* m_pMissleWeaponItem;
    
    
};
#endif /* defined(__tdgame__BuildingMenu__) */
