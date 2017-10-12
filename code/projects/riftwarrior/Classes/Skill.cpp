//
//  Skill.cpp
//  tdgame
//
//  Created by Like Zhang on 3/15/13.
//
//

#include "Skill.h"
#include "Function.h"
#include "SystemHelper.h"

void Skill::init(char** row)
{
    int i = 0;
    
    id = atoi(row[i]);
    vector<string> values = Function::splitString(row[i+1], ";");
    name = values[SystemHelper::getCurrentLanguage()];

    values = Function::splitString(row[i+2], ";");
    introduction = values[SystemHelper::getCurrentLanguage()];
    
    // find last non-empty space
    int k = introduction.length() - 1;
    while (introduction[k]=='\n' || introduction[k]=='\r' || introduction[k] == ' ')
    {
        --k;
    }
    introduction = introduction.substr(0, k+1);
    
    cost = atoi(row[i+3]);
    requireLevel = atoi(row[i+4]);
    maxLevel = parseInt(row[i+5]);
    
    values = Function::splitString(row[i+6], ",");
    trainingBaseTime = atoi(values[0].c_str());
    trainingTimePerLevel = atoi(values[1].c_str());
}

int Skill::parseInt(char *pValue)
{
    if (!pValue || strlen(pValue) == 0)
    {
        return 0;
    }
    
    return atoi(pValue);
}
