//
//  NpcSetting.cpp
//  tdgame
//
//  Created by Like Zhang on 4/25/13.
//
//

#include "NpcSetting.h"
#include "Function.h"
#include "SystemHelper.h"


void NpcSetting::init(char **row)
{
    int i = 0;
    id = atoi(row[i]);
    
    vector<string> values = Function::splitString(row[i+1], ";");
    name = values[SystemHelper::getCurrentLanguage()];
    
    maxHp = atoi(row[i+2]);
    
}