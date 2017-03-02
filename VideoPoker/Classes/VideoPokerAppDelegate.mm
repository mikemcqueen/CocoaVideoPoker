//
//  VideoPokerAppDelegate.m
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//  Copyright (C) 2009 Mike McQueen. All rights reserved.
//

#include "Platform.h"

#import "VideoPokerAppDelegate.h"
#import "GameModel.h"

@implementation VideoPokerAppDelegate

@synthesize window;
@synthesize tabController;
@synthesize game = _game;

+ (VideoPokerAppDelegate*) app
{
    return (VideoPokerAppDelegate*)[[UIApplication sharedApplication] delegate];
}

- (void)
applicationDidFinishLaunching: (UIApplication *) application
{
    [window addSubview: [tabController view]];
    
	srandom((unsigned int)time(0L));
    
	[window makeKeyAndVisible];
}

- (void)
dealloc
{
    [tabController release];
    [window release];
    [super dealloc];
}

#pragma mark -
#pragma mark Application lifecycle

- (void)
awakeFromNib
{
}

/*
 - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.  
    
    // Add the main view controller's view to the window and display.
    [window addSubview:mainViewController.view];
    [window makeKeyAndVisible];
    
    return YES;
}
*/

- (void)
applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}


- (void)
applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    [_game save];
}


- (void)
applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)
applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}


/**
 applicationWillTerminate: saves changes in the application's managed object context before the application terminates.
 */
- (void)
applicationWillTerminate:(UIApplication *)application
{
    [_game save];
}

@end
