//
//  SystemHelper.h
//  tdgame
//
//  Created by Like Zhang on 4/6/13.
//
//

#ifndef __tdgame__SystemHelper__
#define __tdgame__SystemHelper__

#include <iostream>
#include "enums.h"
#include "cocos2d.h"
#include <string>

using namespace cocos2d;
using namespace std;

class SystemHelper
{
public:
    
    ENUM_LANGUAGE static getCurrentLanguage();
    static void setGameLanguage(ENUM_LANGUAGE language);
    
    static bool isSimplifiedChinese();
    
    static void logEvent(const char* pEventName);
    static void logEvent(const char* pEventName, CCDictionary* params, bool timed);
    static void endEvent(const char* pEventName, CCDictionary* params);
    
    static void vibrate();
    
    static void openUrl(const char* url);
    
    static string encryptWithAES(const char* pszData);
    
    static string decryptWithAES(const char* pszData);
    
    static void loadServerImageAsync(const char* pImageName);
    static void loadServerMessageAsync();
    
    static void takeScreenShot();
    
    static unsigned long getCurrentTimeInSeconds();

    static void sendNotification(unsigned long time,const char *msg);
    
    static string getVersionInfo();
    
    static void showGoogleBar();
    static void removeGoogleBar();

};

#endif /* defined(__tdgame__LangSystemHelper__) */
