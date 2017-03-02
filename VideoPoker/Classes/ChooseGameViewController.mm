//
//  ChooseGameViewController.mm
//  VideoPoker
//
//  Created by Mike McQueen on 8/24/10.
//  Copyright 2010 Mike McQueen. All rights reserved.
//

#include "Platform.h"

#import "ChooseGameViewController.h"
#import "PayScheduleViewController.h"

#include "PaySchedule.hpp"

#define ROTATED_PICKER 0

@implementation ChooseGameViewController

@synthesize label       = _label;
@synthesize scrollView  = _scrollView;
@synthesize pageControl = _pageControl;
@synthesize gamePicker  = _gamePicker;
@synthesize delegate    = _delegate;

- (id)
initWithPaySchedule: (const PaySchedule::Lookup::Data_t*) schedule
{
    if (self = [super initWithNibName: @"ChooseGameView" bundle: nil])
    {
        _schedule = schedule;
        _gameId = PaySchedule::Game::Invalid;
    }
    return self;
}

- (void)
dealloc
{
    self.label = nil;
    self.scrollView = nil;
    self.pageControl = nil;
    self.gamePicker = nil;
    
    [super dealloc];
}

- (void)
viewDidLoad
{
    [super viewDidLoad];

#if ROTATED_PICKER
    CGAffineTransform xform = CGAffineTransformMakeRotation(M_PI / 2.0);
    xform = CGAffineTransformScale(xform, .5, 3.0);
    [self.gamePicker setTransform: xform];
    self.gamePicker.backgroundColor = [UIColor clearColor];
#endif
    self.gamePicker.showsSelectionIndicator = YES;
    
    [self initGameMap];
}

- (void)
initGameMap
{
    // for each game
    // TODO: iterate over game map
    PaySchedule::Game::Map_t::const_iterator it = PaySchedule::Manager_t::getMap().begin();
    for (; it != PaySchedule::Manager_t::getMap().end(); ++it)
    {
        // instantiate a mutable array of controllers
        NSMutableArray* controllerArray = [[NSMutableArray alloc] init];
        const unsigned controllerCount = (unsigned)it->second.getScheduleCount();
        
        // add a null value to array for each pay schedule
        for (unsigned index = 0; index < controllerCount; ++index)
        {
            [controllerArray addObject:[NSNull null]];
        }
              
        // add array to game map
        _controllerMap.insert(make_pair(it->first, controllerArray));
    }
}

- (void)
viewWillAppear: (BOOL) animated
{
    PaySchedule::Game::Id_t gameId = _schedule->getGameId(); 
    [_gamePicker selectRow: PaySchedule::Manager_t::getGameIndex(gameId) inComponent: 0 animated: NO]; 
    [self setGame: gameId];
}

- (void)
viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (NSInteger)
numberOfComponentsInPickerView: (UIPickerView *) pickerView
{
    return 1;
}

- (NSInteger)
pickerView: (UIPickerView *) pickerView
numberOfRowsInComponent: (NSInteger) component
{
    // TODO: access global pay schedule map 
    return PaySchedule::Manager_t::getMap().size();
}

#if ROTATED_PICKER
- (CGFloat)
pickerView: (UIPickerView *) pickerView
widthForComponent: (NSInteger) component
{
    return 220.0;
}

- (CGFloat)
pickerView: (UIPickerView *) pickerView
rowHeightForComponent: (NSInteger) component
{
    return 50.0;
}
#endif

- (NSString *)
pickerView: (UIPickerView *) pickerView
titleForRow: (NSInteger) row
forComponent: (NSInteger) component
{
    return [NSString stringWithUTF8String: PaySchedule::Manager_t::getGameName(PaySchedule::Manager_t::getGameId((unsigned)row)).c_str()];
}

#if ROTATED_PICKER
- (UIView*)
pickerView: (UIPickerView*) pickerView
viewForRow: (NSInteger) row
forComponent: (NSInteger) component
reusingView: (UIView*) view
{
    CGRect rect = CGRectMake(0, 0, 80, 108);
    UILabel* label = [[UILabel alloc] initWithFrame: rect];
    CGAffineTransform xform = CGAffineTransformMakeRotation(M_PI / 2.0);
    xform = CGAffineTransformScale(xform, -.50, -2.0);
    [label setTransform: xform];
    label.text = [NSString stringWithUTF8String: PaySchedule_t::getGameName(PaySchedule_t::getGameId(row)).c_str()];
    label.font = [UIFont boldSystemFontOfSize: 16.0];
    label.textAlignment = UITextAlignmentCenter;
    label.numberOfLines = 3;
    label.lineBreakMode = UILineBreakModeWordWrap;
    label.clipsToBounds = YES;
        //    label.backgroundColor = [UIColor clearColor];
    
    return label;
}
#endif

/* UIPickerViewDelegate */

- (void)
pickerView: (UIPickerView*) pickerView
didSelectRow: (NSInteger) row
inComponent: (NSInteger) component
{
    [self setGame: PaySchedule::Manager_t::getGameId((unsigned)row)];
}

- (void)
setGame: (PaySchedule::Game::Id_t) gameId
{
    // this should never happen?
    if (gameId == _gameId)
    {
        NSLog(@"setGame: gameId == _gameId");
        return;
    }
    
    // Remove all existing views from scrollview
    for (UIView* view in _scrollView.subviews)
    {
        [view removeFromSuperview];
    }
    
    // Set _gameId before loading pages
    _gameId = gameId;
    
    const PaySchedule::Lookup::Data_t& schedule = PaySchedule::Manager_t::getPaySchedule(_gameId, 0);
    CGRect frame = self.scrollView.frame;
    frame.size.height = [PayScheduleView getHeightForSchedule: schedule];
    self.scrollView.frame = frame;

    // Load and add views for pages 0 & 1
    [self loadPage: 0];
    [self loadPage: 1];
    
    // recalculate scrollview content size based on number of pay schedules (pages)
    int pageCount = (int)[[self getControllerArray: gameId] count];
    self.scrollView.contentSize = CGSizeMake(CGRectGetWidth(_scrollView.frame) * pageCount, CGRectGetHeight(_scrollView.frame));
    
    self.pageControl.numberOfPages = pageCount;
    self.pageControl.currentPage = 0;

    [self.scrollView setNeedsDisplay];
    [self.pageControl setNeedsDisplay];
    
    [self updateLabel];
}

- (void)
updateLabel
{
    // TODO: Could probably factor text part out and move it to PayScheduleViewController
    PayScheduleViewController *controller = [[self getControllerArray: _gameId] objectAtIndex: _pageControl.currentPage];
    NSString* text = [[NSString stringWithUTF8String: [controller getPaySchedule].getGameName().c_str()]
                      stringByAppendingFormat: @" - %4.2f%%", [controller getPaySchedule].getReturn()];
    _label.text = text;
    [_label setNeedsDisplay];
}

/* UIScrollViewDelegate */

- (void)
scrollViewDidScroll: (UIScrollView *) sender
{
    // We don't want a "feedback loop" between the UIPageControl and the scroll delegate in
    // which a scroll event generated from the user hitting the page control triggers updates from
    // the delegate method. We use a boolean to disable the delegate logic when the page control is used.
    if (_pageControlUsed)
    {
        // do nothing - the scroll was initiated from the page control, not the user dragging
        return;
    }
	
    // Switch the indicator when more than 50% of the previous/next page is visible
    CGFloat pageWidth = _scrollView.frame.size.width;
    int page = floor((_scrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
    _pageControl.currentPage = page;
    
    // load the visible page and the page on either side of it (to avoid flashes when the user starts scrolling)
    [self loadPage:page - 1];
    [self loadPage:page];
    [self loadPage:page + 1];
    
    [self updateLabel];
    
    // A possible optimization would be to unload the views+controllers which are no longer visible
}

// At the begin of scroll dragging, reset the boolean used when scrolls originate from the UIPageControl
- (void)
scrollViewWillBeginDragging: (UIScrollView *) scrollView
{
    _pageControlUsed = NO;
}

// At the end of scroll animation, reset the boolean used when scrolls originate from the UIPageControl
- (void)
scrollViewDidEndDecelerating: (UIScrollView *) scrollView
{
    _pageControlUsed = NO;
}

- (void)
loadPage: (int) page
{
    if (page < 0) return;
    
    NSMutableArray* controllerArray = [self getControllerArray: _gameId];
    int pageCount = (int)[controllerArray count];
    if (page >= pageCount) return;
    
    // replace the placeholder if necessary
    PayScheduleViewController *controller = [controllerArray objectAtIndex: page];
    if ((NSNull *)controller == [NSNull null])
    {
        controller = [[PayScheduleViewController alloc] initWithPaySchedule: &PaySchedule::Manager_t::getPaySchedule(_gameId, page)];
        [controllerArray replaceObjectAtIndex: page withObject: controller];
        [controller release];
    }
    
    // add the controller's view to the scroll view
    if (nil == controller.view.superview)
    {
#if 1
        CGRect frame = _scrollView.frame;
        frame.origin.x = frame.size.width * page;
        frame.origin.y = 0;
        controller.view.frame = frame;
#else
        CGPoint origin = _scrollView.frame.origin;
        origin.x = CGRectGetHeight(_scrollView.frame) * page;
        origin.y = 0;
        controller.frame.origin = origin;
#endif
        [self.scrollView addSubview: controller.view];
    }
}

- (NSMutableArray*)
getControllerArray: (PaySchedule::Game::Id_t) gameId
{
    PayScheduleControllerMap_t::const_iterator it = _controllerMap.find(gameId);
    if (_controllerMap.end() == it)
    {
        throw runtime_error("getControllerArray() invalid gameId");
    }
    return it->second;
}
    
- (IBAction)
pageControlWasPressed: (id) sender
{
    int page = (int)_pageControl.currentPage;
	
    // load the visible page and the page on either side of it (to avoid flashes when the user starts scrolling)
    [self loadPage:page - 1];
    [self loadPage:page];
    [self loadPage:page + 1];
    
    // update the scroll view to the appropriate page
    CGRect frame = _scrollView.frame;
    frame.origin.x = frame.size.width * page;
    frame.origin.y = 0;
    [_scrollView scrollRectToVisible:frame animated:YES];
    
    // Set the boolean used when scrolls originate from the UIPageControl. See scrollViewDidScroll: above.
    _pageControlUsed = YES;

    [self updateLabel];
}

- (IBAction)
done
{
    PayScheduleViewController *controller = [[self getControllerArray:_gameId] objectAtIndex:_pageControl.currentPage];
    [self.delegate chooseGameDone:[controller getPaySchedule]];
}

- (IBAction)
cancel
{
    [self.delegate chooseGameCancel];
}

@end
