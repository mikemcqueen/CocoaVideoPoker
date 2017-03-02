//
//  AssistViewController.mm
//  VideoPoker
//
//  Created by Mike McQueen on 8/2/10.
//  Copyright 2010 Amazon.com. All rights reserved.
//

#include "Platform.h"

#import "SolveViewController.h"
#import "HandDetailsViewController.h"
#import "GameModel.h"
#import "Deck.h"
#import "CardTableView.h"
#import "CardView.h"


@implementation SolveViewController

@synthesize game;
@synthesize cardTable;
@synthesize returnViewController;
@synthesize cardPicker;
@synthesize holdBestButton;
@synthesize suitViewArray;
@synthesize numberViewArray;

@synthesize selectedView; // bad

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
 - (id)
 initWithNibName:(NSString *)nibNameOrNil
 bundle:(NSBundle *)nibBundleOrNil
 {
 if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
 // Custom initialization
 }
 return self;
 }
 */

- (void)
viewDidLoad
{
    [super viewDidLoad];
    
    // Add CardTableView
    [self.view addSubview: cardTable.view];
    
    // Add ReturnView
    CGFloat top = cardTable.view.frame.origin.y + cardTable.view.frame.size.height + 3.0;
    returnViewController.view.frame = CGRectOffset(returnViewController.view.bounds, 0.0, top);
    [self.view addSubview: returnViewController.view];
    
    const unsigned cardCount = 5;
    CardVector_t cards(cardCount);
    for (unsigned index = 0; index < cardCount; ++index)
    {
        cards[index] = Card_t(Card_t::makeValue(10 + index, Suit::Spades));
    }
    
    game.hand = &self->_hand;
    [game setCards: cards];
    
    selectedView = [(CardTableView*)cardTable.view getCardView: 0];
    
    // Initialize PickerView arrays
    [self buildSuitViews];
    [self buildNumberViews];
    
    NSLog (@"SolveView::viewDidLoad()");
}

- (void)
buildSuitViews
{
    CGImageRef allSuits;
    
    NSString* imageFilename = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"suits-60.png"];
    CGDataProviderRef provider = CGDataProviderCreateWithFilename([imageFilename UTF8String]);
    allSuits = CGImageCreateWithPNGDataProvider(provider, NULL, true, kCGRenderingIntentDefault);
    CGDataProviderRelease(provider);

    
    self->suitViewArray = [NSMutableArray arrayWithCapacity: Suit::Count];
    CGRect cardRect = { 0, 0, 48, 53 };
    for (int suit = 0; suit < Suit::Count; ++suit)
    {
        
        cardRect.origin.x = cardRect.size.width * suit;
        CGImageRef cgSuitImage = CGImageCreateWithImageInRect(allSuits, cardRect);
        UIImage* uiSuitImage = [UIImage imageWithCGImage: cgSuitImage];
        UIImageView* uiSuitView = [[[UIImageView alloc] initWithImage: uiSuitImage] retain];
    //    [uiSuitImage release];
    //    CGImageRelease(cgSuitImage);
        switch (suit)
        {
            case 0: self->clubView = [uiSuitView retain];
            case 1: self->diamondView = [uiSuitView retain];
            case 2: self->heartView = [uiSuitView retain];
            case 3: self->spadeView = [uiSuitView retain];
        }
        
        [suitViewArray addObject: uiSuitView];
    }
    //self->suitViewArray = suitViewArray;
        
    CGImageRelease(allSuits);
}

- (void)
buildNumberViews
{
    /*NSMutableArray* */ self->numberViewArray = [NSMutableArray arrayWithCapacity: 13];
    CGRect rect = { 0, 0, 48, 53 };
    for (size_t number = 2; number <= Ace; ++number)
    {
        UILabel* uiNumberView = [[UILabel alloc] initWithFrame: rect];
        //    [uiSuitImage release];
        //    CGImageRelease(cgSuitImage);

        uiNumberView.text = [self getCardNumberString: number];
       // uiNumberView.textAlignment = UITextAlignmentCenter;
        uiNumberView.adjustsFontSizeToFitWidth = NO;
        uiNumberView.numberOfLines = 1;
        uiNumberView.font = [UIFont boldSystemFontOfSize: 36.0];
        [numberViewArray addObject: uiNumberView];
        [uiNumberView release];
    }
//    self.numberViewArray = numberViewArray;
}

- (NSString*)
getCardNumberString: (size_t) number
{
    if (number >=2 && number <= 9)
    {
        return [NSString stringWithFormat: @"%d", (int)number];
    }
    using namespace Poker::Card;
    switch (number)
    {
        case 10:    return @"10";
        case Jack:  return @"J";
        case Queen: return @"Q";
        case King:  return @"K";
        case Ace:   return @"A";
    }
    // TODO: throw exception
    return @"ERR";
}

- (void)
viewWillAppear:(BOOL)animated
{
    game.hand = &self->_hand;    
}

/*
 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
 // Return YES for supported orientations
 return (interfaceOrientation == UIInterfaceOrientationPortrait);
 }
 */

- (NSInteger)
numberOfComponentsInPickerView: (UIPickerView *) pickerView
{
    return 2;
}

- (NSInteger)
pickerView: (UIPickerView *) pickerView
numberOfRowsInComponent: (NSInteger) component
{
    switch (component)
    {
        case 0: return 13;
        case 1: return 4;
    }
    return 0;
}

- (CGFloat)
pickerView: (UIPickerView *) pickerView
widthForComponent: (NSInteger) component
{
    return 54.0;
}

- (CGFloat)
pickerView: (UIPickerView *) pickerView
rowHeightForComponent: (NSInteger) component
{
    return 53.0;
}

- (NSString *)
pickerView: (UIPickerView *) pickerView
titleForRow: (NSInteger) row
forComponent: (NSInteger) component
{
    static char cards[] = "TJQKA";
    
    switch (component)
    {
        case 0:
            if (row < 8)
            {
                return [NSString stringWithFormat: @"%ldd", row + 2];
            }
            return [NSString stringWithFormat: @"%c", cards[row - 8]];
            
        case 1:
            return nil;
        {
            NSString* suit = @"error1";
            switch (row)
            {
                case 0: suit = @"Clubs"; break;
                case 1: suit = @"Diamonds"; break;
                case 2: suit = @"Hearts"; break;
                case 3: suit = @"Spades"; break;
            }
            return suit;
        }
    }
    return @"error2";
}

- (UIView*)
pickerView: (UIPickerView*) pickerView
viewForRow: (NSInteger) row
forComponent: (NSInteger) component
reusingView: (UIView*) view
{
    switch (component)
    {
    case 0:
        return [[numberViewArray objectAtIndex: row] retain];

    case 1:
        switch (row)
        {
            case 0: return [clubView retain];
            case 1: return [diamondView retain];
            case 2: return [heartView retain];
            case 3: return [spadeView retain];
        }
        break;
                // return [[self->suitViewArray objectAtIndex: row] retain];
                //            break;
    }
    return nil;
}

- (void)
pickerView: (UIPickerView*) pickerView
didSelectRow: (NSInteger) row
inComponent: (NSInteger) component
{
    Card_t card = self.game.hand->getCard((int)[selectedView getIndex]);
    if (component == 0)
    {
        card.setNumber((unsigned)row + 2);
    }
    else
    {
        card.setSuit(Suit_t(row));
    }
    [selectedView setNeedsDisplay];
    [game setCard: card atIndex: [selectedView getIndex]];
    [self cardsDidChange];
}

- (void)
setPicker: (UIPickerView*) pickerView
toCard: (Card_t) card
{
    [pickerView selectRow: card.getNumber() - 2 inComponent: 0 animated: YES];
    [pickerView selectRow: card.getSuit()       inComponent: 1 animated: YES];
}

- (void)
showHandDetails
{
	HandDetailsViewController* handDetailsController = [[HandDetailsViewController alloc] initWithNibName:@"HandDetailsView" bundle:nil];
    handDetailsController.game = game;
    handDetailsController.hidesBottomBarWhenPushed = YES;
    [self.navigationController pushViewController:handDetailsController animated:YES];
}

////////////////////////////////////////////////////////////////////////////////

- (void)
cardViewWasPressed: (CardView*) cardView
{
    self.selectedView = cardView;
    [self setPicker: cardPicker toCard: [cardView getCard]];
}

- (void)
cardsDidChange
{
    BOOL valid = game.hand->isValid();

    holdBestButton.enabled = valid;
    
    [self.returnViewController setInvalid: !valid];
    if (valid)
    {
        [self.returnViewController setYourEv: ((Hand::Solver::Default::Results_t&)self.game.solver->getResults(0)).ev];
        [self.returnViewController setBestEv: ((Hand::Solver::Default::Results_t&)self.game.solver->getBestResults()).ev];
    }
}

- (Card_t)
getCard: (NSInteger) index
{
    return game.hand->getCard((int)index);
}

- (BOOL)
isHeldCard: (Card_t) card
{
    return false;
}

- (CGImageRef)
getCardImage: (Card_t) card
{
    return [game.deck getCardImage: card];
}

- (CGImageRef)
getBackImage: (NSInteger) index
{
    return [Deck getBackImage];
}

////////////////////////////////////////////////////////////////////////////////

- (void)
returnView: (UIView*) view
detailButtonWasPressed: (UIButton*) button
{
    [self showHandDetails];    
}

////////////////////////////////////////////////////////////////////////////////

- (void)
didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)
viewDidUnload
{
    [super viewDidUnload];
    NSLog(@"solveViewController viewDidUnload");
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)
dealloc
{
    
    NSLog(@"solveViewController dealloc");
    [super dealloc];
}


@end
