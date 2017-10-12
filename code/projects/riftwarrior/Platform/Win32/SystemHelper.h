//
//  SystemHelper.h
//  tdgame
//
//  Created by Like Zhang on 4/6/13.
//
//

#ifndef __tdgame__SystemHelper__
#define __tdgame__SystemHelper__

#include <codecvt>
#include <string>

#include <cocos2d.h>
#include <enums.h>

using namespace cocos2d;
using namespace std;

/*
 *
 * TODO: All dummy functions that do nothing to get build pass now!
 *
 */
class SystemHelper
{
public:
    static wstring_convert<codecvt_utf8<wchar_t>, wchar_t> WideStringToNarrower;
    
    ENUM_LANGUAGE static getCurrentLanguage() { return ENUM_LANGUAGE_ENGLISH; };
    static void setGameLanguage(ENUM_LANGUAGE language) {};
    
    static bool isSimplifiedChinese() { return false;};
    
    static void logEvent(const char* pEventName) {};
    static void logEvent(const char* pEventName, CCDictionary* params, bool timed) {};
    static void endEvent(const char* pEventName, CCDictionary* params) {};
    
    static void vibrate() {};
    
    static void openUrl(const char* url) {};
    
	static string encryptWithAES(const char* pszData) { return string("dummy string"); };
    
    static string decryptWithAES(const char* pszData) { return string("dummy string"); };
    
    static void loadServerImageAsync(const char* pImageName) {};
    static void loadServerMessageAsync() {};
    
    static void takeScreenShot() {};
    
    static unsigned long getCurrentTimeInSeconds() { return 0; };

    static void sendNotification(unsigned long time, const char *msg) {};
    
    static string getVersionInfo();
    
    static void showGoogleBar() {};
    static void removeGoogleBar() {};

};

#endif /* defined(__tdgame__LangSystemHelper__) */
