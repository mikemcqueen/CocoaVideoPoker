//
//  VideoPokerAppDelegate.h
//  VideoPoker
//
//  Created by Mike McQueen on 8/1/09.
//  Copyright (C) 2009 Mike McQueen. All rights reserved.
//

@class GameModel;

@interface VideoPokerAppDelegate :
    NSObject<UIApplicationDelegate>
{
    UIWindow*           window;
    UITabBarController* tabController;
    GameModel*          _game;
}

@property (nonatomic, retain)           IBOutlet UIWindow*           window;
@property (nonatomic, readonly, retain) IBOutlet UITabBarController* tabController;
@property (nonatomic, readonly, retain) IBOutlet GameModel*          game;

+ (VideoPokerAppDelegate*) app;

//- (BOOL) initMoc;
//- (NSString *) applicationDocumentsDirectory;
//- (void) saveContext;

@end