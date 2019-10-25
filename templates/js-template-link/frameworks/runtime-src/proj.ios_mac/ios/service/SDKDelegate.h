//
//  SDKWrapperDelegate.h
//  polish_project-mobile
//
//  Created by 杨欣 on 2018/10/22.
//

#import <Foundation/Foundation.h>

#if CC_PLATFORM_TARGET == CC_PLATFORM_IOS
#import <UIKit/UIKit.h>
#endif

NS_ASSUME_NONNULL_BEGIN

@protocol SDKDelegate <NSObject>

@optional
- (void) optionalFunction;
- (void)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
- (void)applicationDidBecomeActive:(UIApplication *)application;
- (void)applicationWillResignActive:(UIApplication *)application;
- (void)applicationDidEnterBackground:(UIApplication *)application;
- (void)applicationWillEnterForeground:(UIApplication *)application;
- (void)applicationWillTerminate:(UIApplication *)application;

@end

NS_ASSUME_NONNULL_END
