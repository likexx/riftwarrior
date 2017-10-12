//
//  BuildingBase.h
//  tdgame
//
//  Created by Like Zhang on 5/8/13.
//
//

#ifndef __tdgame__BuildingBase__
#define __tdgame__BuildingBase__

#include <iostream>
#include "enums.h"
#include "BuildingAttackComponent.h"
using namespace std;

class Enemy;

class BuildingBase : public BuildingAttackComponent
{
public:
    virtual ~BuildingBase();
    
    virtual bool init();
    virtual void update(float dt);
    
    CREATE_FUNC(BuildingBase);
    
    void revivePlayer();
    void reviveComputerPlayer();
    
    inline void setDestinationTile(const CCPoint& tilePos)
    {
        m_DestinationTile = tilePos;
    }
    
    inline CCPoint getDestinationTile()
    {
        return m_DestinationTile;
    }
    
    
protected:
    
    virtual CCActionInterval* createBulletAction(CCPoint currentMapPos, CCPoint targetMapPos, CCPoint targetPos, float mapSpeed);
    
    bool m_Initialized;
    
    bool m_WaitingForPlayerRevival;
    bool m_WaitingForComputerRevival;

    
    CCPoint m_DestinationTile;
};
#endif /* defined(__tdgame__BuildingBase__) */
