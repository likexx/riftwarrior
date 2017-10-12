//
//  tdgameAppController.mm
//  tdgame
//
//  Created by Like Zhang on 2/4/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"
#include "Player.h"
#import "RootViewController.h"
#import <AdSupport/ASIdentifierManager.h>
//#import "GADAdSize.h"
#include "AdHelper.h"

@implementation AppController

@synthesize window;
@synthesize viewController;
@synthesize interstitialReceived;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // Override point for customization after application launch.
//    NSSetUncaughtExceptionHandler(&uncaughtExceptionHandler);
    
    
//    [[KKIAPHelper sharedInstance] loadProducts];
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGB565
                                     depthFormat: GL_DEPTH_COMPONENT24_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples:0 ];
    
    [__glView setMultipleTouchEnabled:YES];
    
    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
//    weiboDelegate = [[WeiboDelegate alloc] init];
//    sinaweibo = [[SinaWeibo alloc] initWithAppKey:kAppKey appSecret:kAppSecret appRedirectURI:@"http://" andDelegate:weiboDelegate];

    
    [[UIApplication sharedApplication] setStatusBarHidden: YES];
    
//    [[UIAccelerometer sharedAccelerometer] setUpdateInterval:1.0f/25];
    
    cocos2d::CCApplication::sharedApplication()->run();
    
    return YES;
}


- (void) createGoogleBar
{
    
//    if (s_BannerView != nil)
//    {
//        return;
//    }
//
//    CGSize size = CGSizeFromGADAdSize(kGADAdSizeSmartBannerLandscape);
//
//    CGFloat adWidth = size.width * 0.6;
//    CGFloat adHeight = size.height * 1.5;
//
//    //    CGPoint origin = CGPointMake(winWidth * 0.3,
//    //                                 300
//    //                                 );
//
//
//    //    GADAdSize adSize = GADAdSizeFromCGSize(CGSizeMake(winWidth * 0.3, adHeight));
//    GADAdSize adSize = GADAdSizeFromCGSize(CGSizeMake(adWidth, adHeight));
//
//    //    NSLog(@"%f, %f", self.viewController.view.frame.size.width, self.viewController.view.frame.size.height);
//
//    CGPoint origin = CGPointMake((self.viewController.view.frame.size.height - adWidth)/2,
//                                 self.viewController.view.frame.size.width - adHeight);
//
//    s_BannerView = [[GADBannerView alloc] initWithAdSize:adSize origin:origin];
//    s_BannerView.delegate = self;
//    //    [s_BannerView retain];
//
//    s_BannerView.adUnitID = @"xxxxxxxxx";
//    s_BannerView.rootViewController = viewController;
//    [viewController.view addSubview:s_BannerView];
    
}

- (void) removeGoogleBar {
//    if (s_BannerView != nil) {
//        [s_BannerView removeFromSuperview];
//
//        [s_BannerView release];
//
//        s_BannerView = nil;
//    }
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
//    [FBAppEvents activateApp];
//    [FBAppCall handleDidBecomeActive];

    application.applicationIconBadgeNumber = 0;  
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    CCDirector::sharedDirector()->stopAnimation();
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    CCDirector::sharedDirector()->startAnimation();
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
    
//    [FBSession.activeSession close];
}


// weibo delegate
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
//    if (SystemHelper::isSimplifiedChinese())
//    {
//        return [self.sinaweibo handleOpenURL:url];
//    }
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    
//    if (SystemHelper::isSimplifiedChinese())
//    {
//        return [self.sinaweibo handleOpenURL:url];
//    }
//    else
//    {
//        return [FBAppCall handleOpenURL:url
//                      sourceApplication:sourceApplication];
//        
//    }

}

- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    application.applicationIconBadgeNumber = 0;
}




#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
     cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}


- (void)dealloc {
    [super dealloc];
}


@end

