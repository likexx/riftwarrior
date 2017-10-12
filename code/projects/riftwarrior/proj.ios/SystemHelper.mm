//
//  SystemHelper.cpp
//  tdgame
//
//  Created by Like Zhang on 4/6/13.
//
//

#include "SystemHelper.h"
#include "enums.h"
#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioServices.h>
#include <CoreFoundation/CoreFoundation.h>
#import <Security/Security.h>
#import "NSData+Encryption.h"
#import <AdSupport/ASIdentifierManager.h>
#import "AppController.h"

#include <stdio.h>
#include <stdlib.h>

#include "GameScene.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

#define SERVER_URL "http://localhost"
#define SERVER_IMAGE_TAG 50001

static bool _usingSystemDefaultLanguage = true;
static ENUM_LANGUAGE _userSelectedLanguage = ENUM_LANGUAGE_ENGLISH;

void SystemHelper::setGameLanguage(ENUM_LANGUAGE language)
{
    _usingSystemDefaultLanguage = false;
    _userSelectedLanguage = language;
}

ENUM_LANGUAGE SystemHelper::getCurrentLanguage()
{
    if (_usingSystemDefaultLanguage)
    {
        NSString* value = [[NSLocale preferredLanguages] objectAtIndex:0];
        
        if ([value hasPrefix:@"zh"])
        {
            return ENUM_LANGUAGE_CHINESE;
        }
        
        return ENUM_LANGUAGE_ENGLISH;
    }
    else
    {
        return _userSelectedLanguage;
    }
    
}

bool SystemHelper::isSimplifiedChinese()
{
    
    if (!_usingSystemDefaultLanguage)
    {
        return _userSelectedLanguage == ENUM_LANGUAGE_CHINESE;
    }
    
    NSString* value = [[NSLocale preferredLanguages] objectAtIndex:0];
    
    NSLog(@"%@", value);
    
    if ([value hasPrefix:@"zh-Hans"])
    {
        return true;
    }
    
    return false;
}

string SystemHelper::getVersionInfo()
{
    NSDictionary *infoDictionary = [[NSBundle mainBundle] infoDictionary];
    //    // app名称
    //    NSString *app_Name = [infoDictionary objectForKey:@"CFBundleDisplayName"];
    //    // app版本
    NSString *app_Version = [infoDictionary objectForKey:@"CFBundleShortVersionString"];
    //    // app build版本
    //    NSString *app_build = [infoDictionary objectForKey:@"CFBundleVersion"];
    
    NSString *info = [NSString stringWithFormat:@"Version:%@",app_Version];
    
    return [info UTF8String];
}


void SystemHelper::logEvent(const char *pEventName)
{
}

void SystemHelper::logEvent(const char *pEventName, CCDictionary* params, bool timed)
{
    
}

void SystemHelper::endEvent(const char *pEventName, cocos2d::CCDictionary *params)
{
    
}

void SystemHelper::vibrate()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}


void SystemHelper::openUrl(const char *url)
{
    NSString* webUrl = [NSString stringWithUTF8String:url];
    
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:webUrl]];
}



string SystemHelper::encryptWithAES(const char *pszData)
{
    NSString *key = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    
//	NSString *secret = [NSString stringWithUTF8String:pszData];
    
//	NSData *plain = [secret dataUsingEncoding:NSUTF8StringEncoding];
    NSData *plain = [NSData dataWithBytes:pszData length:strlen(pszData)];
	NSData *encrypted = [plain AES256EncryptWithKey:key];
    
    NSString* hexData = [encrypted DecimalString];
    
    return [hexData UTF8String];

//    NSLog(@"%s", [[encrypted description] UTF8String]);
/*
    vector<char> bytes(encrypted.length+1, 0);
    
    memcpy(&bytes[0], encrypted.bytes, encrypted.length);
    
    return &bytes[0];
*/    
}

string SystemHelper::decryptWithAES(const char *pszData)
{
    NSString *key = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    
    int length = strlen(pszData);

//    NSData* pEncrypted = [NSData dataWithBytes:pszData length:length];
    
    NSData* pEncrypted = [NSData dataWithDecimalString:pszData withLength:strlen(pszData)];

//    NSLog(@"%s", [[pEncrypted description] UTF8String]);
    
	NSData* pDecrypted = [pEncrypted AES256DecryptWithKey:key];
   
    vector<char> result(pDecrypted.length+1, 0);
    [pDecrypted getBytes:&result[0]];

    /*
    NSString* result = [[NSString alloc] initWithData:pDecrypted encoding:NSUTF8StringEncoding];
    
    NSLog(@"%@", result);
    
    string finalResult = [result UTF8String];
    
    [result release];
    
    return finalResult;
     */
    
    return &result[0];
    
}

void SystemHelper::loadServerImageAsync(const char* pImageName)
{

}


void SystemHelper::loadServerMessageAsync()
{

}

void SystemHelper::takeScreenShot()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    float width = winSize.width > 2048 ? 2048 : winSize.width;
    float height = width * winSize.height/winSize.width;
    
    CCRenderTexture* rt = CCRenderTexture::create(width, height);
    
    rt->beginWithClear(0, 0, 0, 0);

    GameScene::getInstance()->sharedScene->visit();
    
    rt->end();

    CCSprite* sprite = CCSprite::createWithTexture(rt->getSprite()->getTexture());
    sprite->setFlipY(true);
    CCLayer* tempLayer = CCLayer::create();
    tempLayer->setContentSize(CCSizeMake(640, 640 * height/width));
    
    sprite->setScale(640/width);
    sprite->setPosition(ccp(tempLayer->getContentSize().width/2, tempLayer->getContentSize().height/2));
    tempLayer->addChild(sprite);
    
    CCRenderTexture* smallRt = CCRenderTexture::create(640, 640 * height/width);
    smallRt->beginWithClear(0, 0, 0, 0);
    tempLayer->visit();
    smallRt->end();
    
    smallRt->saveToFile("screenshot.jpg", kCCImageFormatJPEG);
}

void SystemHelper::sendNotification(unsigned long time,const char *msg)
{
    UILocalNotification* notif = [[[UILocalNotification alloc] init] autorelease];
    if(nil != notif){
        NSDate* now = [[NSDate new] autorelease];
        notif.fireDate= [now dateByAddingTimeInterval:time];
        notif.repeatInterval=0;
        notif.timeZone=[NSTimeZone defaultTimeZone];
        notif.applicationIconBadgeNumber = [[UIApplication sharedApplication] applicationIconBadgeNumber] + 1;
        notif.soundName= UILocalNotificationDefaultSoundName;
        notif.alertBody= [NSString stringWithUTF8String:msg];
        notif.hasAction = NO;
        [[UIApplication sharedApplication] scheduleLocalNotification:notif];
    }
}

unsigned long SystemHelper::getCurrentTimeInSeconds()
{
    //    return time(NULL);
    NSDate *current =[NSDate date];
    
    unsigned long time = [current timeIntervalSince1970];
    
    //    CCLog("seconds: %lu", time);
    return time;
}


void SystemHelper::showGoogleBar()
{
    if (!Player::getInstance()->hasCompletedMap(103))
    {
        return;
    }
    
    AppController *app = (AppController *)[UIApplication sharedApplication].delegate;
    
    
    [app createGoogleBar];
}

void SystemHelper::removeGoogleBar()
{
    AppController *app = (AppController *)[UIApplication sharedApplication].delegate;
    [app removeGoogleBar];
}

