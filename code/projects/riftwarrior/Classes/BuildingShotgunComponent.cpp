//
//  BuildingShotgunComponent.cpp
//  tdgame
//
//  Created by Like Zhang on 4/24/13.
//
//

#include "BuildingShotgunComponent.h"

BuildingShotgunComponent::~BuildingShotgunComponent()
{
    
}

CCPoint BuildingShotgunComponent::getFirePosition()
{
    float xOffsets[] = {230, 210, 136, 63, 136};
    float yOffsets[] = {110,  67,  47, 157, 175};
    
    return calculateFirePosition(xOffsets, yOffsets);
}