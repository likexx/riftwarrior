//
//  BuildingStatusBar.h
//  tdgame
//
//  Created by Like Zhang on 4/1/13.
//
//

#ifndef __tdgame__BuildingStatusBar__
#define __tdgame__BuildingHpBar__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class Building;

class BuildingStatusBar : public CCNode
{
public:
    static BuildingStatusBar* create(Building* pBuilding);
    virtual bool init(Building* pBuilding);
    virtual void update(float dt);

private:
    void resetLevelStars();

    Building* m_pBuilding;
    CCLabelBMFont* m_LevelLabel;
    int m_CurrentLevel;
    
//    CCNode* m_StatusBox;
};

class BuildingHpBar : public CCNode
{
public:
    static BuildingHpBar* create(Building* pBuilding);
    virtual bool init(Building* pBuilding);
    virtual void draw();
    
private:
    
    
    Building* m_pBuilding;
    
};

#endif /* defined(__tdgame__BuildingStatusBar__) */
