//
//  BuildingShotgunComponent.h
//  tdgame
//
//  Created by Like Zhang on 4/24/13.
//
//

#ifndef __tdgame__BuildingShotgunComponent__
#define __tdgame__BuildingShotgunComponent__

#include <iostream>
#include "BuildingMultiArrowAttackComponent.h"

class BuildingShotgunComponent : public BuildingMultiArrowAttackComponent
{
public:
    virtual ~BuildingShotgunComponent();

    CREATE_FUNC(BuildingShotgunComponent);
    
private:
    virtual CCPoint getFirePosition();

};

#endif /* defined(__tdgame__BuildingShotgunComponent__) */
