//
//  BuildingMachineGunComponent.h
//  tdgame
//
//  Created by Like Zhang on 4/27/13.
//
//

#ifndef __tdgame__BuildingMachineGunComponent__
#define __tdgame__BuildingMachineGunComponent__

#include <iostream>
#include "BuildingShotgunComponent.h"

class BuildingMachineGunComponent : public BuildingShotgunComponent
{
public:
    virtual ~BuildingMachineGunComponent();
    virtual void startAttack();
    
    CREATE_FUNC(BuildingMachineGunComponent);
    
private:
    
    virtual void playAttackAnimation();
    
    virtual CCPoint getFirePosition();
    
    void fireNext();
    
    float m_CurrentDegree;
    float m_FireCount;
    
};
#endif /* defined(__tdgame__BuildingMachineGunComponent__) */
