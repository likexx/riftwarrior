//
//  KKIAPWrapper.h
//  tdgame
//
//  Created by Like Zhang on 4/22/13.
//
//

#ifndef __tdgame__KKIAPWrapper__
#define __tdgame__KKIAPWrapper__

#include <iostream>
#include "IAPItem.h"
#include <vector>
#include <map>
using namespace std;

class KKIAPWrapper
{
public:

    static void addItem(const IAPItem& item);
    static const map<string, IAPItem>& getIAPItems();
    
    static bool buyItem(const char* productIdentifier);
    
    static void notifyTransactionCompleted(const char* identifier);
    static void notifyTransactionFailed(const char* identifer);
    
};


#endif /* defined(__tdgame__KKIAPWrapper__) */
