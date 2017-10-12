//
//  ActiveSkillSetting.cpp
//  tdgame
//
//  Created by Like Zhang on 4/23/13.
//
//

#include "ActiveSkillSetting.h"
#include "Function.h"
#include "SystemHelper.h"

void ActiveSkillSetting::init(char** row)
{
    int i = 0;
    
    id = atoi(row[i]);
    vector<string> values = Function::splitString(row[i+1], ";");
    name = values[SystemHelper::getCurrentLanguage()];
    
    mpCost = atoi(row[i+2]);
    
    damageValue = atof(row[i+3]);
    
    requireLevel = atoi(row[i+4]);
    
    
}

int ActiveSkillSetting::parseInt(char *pValue)
{
    if (!pValue || strlen(pValue) == 0)
    {
        return 0;
    }
    
    return atoi(pValue);
}