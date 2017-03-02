//
//  ChooseGameViewController.h
//  VideoPoker
//
//  Created by Mike McQueen on 8/24/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#include "PaySchedule.hpp"

@protocol ChooseGameViewControllerDelegate;
@class PayScheduleViewController;

//class PaySchedule_t;

typedef map<PaySchedule::Game::Id_t, NSMutableArray*>  PayScheduleControllerMap_t;

@interface ChooseGameViewController :
    UIViewController<UIPickerViewDataSource,
                     UIPickerViewDelegate,
                     UIScrollViewDelegate>
{
	id<ChooseGameViewControllerDelegate> _delegate;
    
    UILabel*       _label;
    UIScrollView*  _scrollView;
    UIPageControl* _pageControl;
    UIPickerView*  _gamePicker;
    
    BOOL           _pageControlUsed;
    
    const PaySchedule::Lookup::Data_t* _schedule;
    
    PaySchedule::Game::Id_t    _gameId;
    PayScheduleControllerMap_t _controllerMap;
}

@property (nonatomic, retain) IBOutlet UILabel*       label;
@property (nonatomic, retain) IBOutlet UIScrollView*  scrollView;
@property (nonatomic, retain) IBOutlet UIPickerView*  gamePicker;
@property (nonatomic, retain) IBOutlet UIPageControl* pageControl;

@property (nonatomic, assign) id<ChooseGameViewControllerDelegate> delegate;

- (IBAction) done;
- (IBAction) cancel;
- (IBAction) pageControlWasPressed: (id) sender;

- (id)
initWithPaySchedule: (const PaySchedule::Lookup::Data_t*) schedule;

- (void)
initGameMap;

- (void)
setGame: (PaySchedule::Game::Id_t) gameId;

- (void)
loadPage: (int) page;

- (NSMutableArray*)
getControllerArray: (PaySchedule::Game::Id_t) gameId;

- (void)
updateLabel;

@end

@protocol ChooseGameViewControllerDelegate

- (void) chooseGameDone: (const PaySchedule::Lookup::Data_t&) schedule;
- (void) chooseGameCancel;

@end


