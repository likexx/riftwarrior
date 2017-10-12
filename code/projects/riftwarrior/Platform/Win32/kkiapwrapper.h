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

    static void addItem(const IAPItem& item)
    { /* TODO: Dummy method that does nothing for now. */ } 

    static const map<string, IAPItem>& getIAPItems()
    {
		/* TODO: Dummy method that does nothing for now. */ 
		static map<string, IAPItem> dummyResult;
		return dummyResult;
	} 
    
    static bool buyItem(const char* productIdentifier)
    { 
		/* TODO: Dummy method that does nothing for now. */ 
		return false;
	}
    
    static void notifyTransactionCompleted(const char* identifier)
    { /* TODO: Dummy method that does nothing for now. */ } 

    static void notifyTransactionFailed(const char* identifer)
    { /* TODO: Dummy method that does nothing for now. */ } 
};


#endif /* defined(__tdgame__KKIAPWrapper__) */
