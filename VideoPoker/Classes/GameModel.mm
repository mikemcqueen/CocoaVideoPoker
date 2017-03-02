//
//  GameModel.mm
//  VideoPoker
//
//  Created by Mike McQueen on 7/24/10.
//

#import "Platform.h"
#import "GameModel.h"
#import "Deck.h"
#import "SessionMO.h"
#import "HandMO.h"
#import "HandDataMO.h"
#import "VideoPokerAppDelegate.h"

#include "SolverDataLookup.hpp"
#include "PaySchedule.hpp"

#include "GameRegister.hpp"

@implementation GameModel

@synthesize bet                 = _bet;
@synthesize handValueChanged    = _handValueChanged;
@synthesize deck                = _deck;
@synthesize schedule            = _schedule;
@synthesize solver              = _solver;
@synthesize hand                = _hand;
@synthesize session             = _session;

+ (NSURL*)
storeURL
{
    return [NSURL fileURLWithPath: [[GameModel applicationDocumentsDirectory] stringByAppendingPathComponent: @"VideoPoker.sqlite"]];
}

+ (NSString *)
applicationDocumentsDirectory
{
	return [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
}

- (void)
dealloc
{
    [_context release];
    [_model release];
    [_stores release];
    [_deck release];
    
    [super dealloc];
}
 
#define LOAD_SOLVER 1

- (id)
init
{
    self = [super init];
    
    using namespace PaySchedule;

    PaySchedule::Manager_t::registerAllPaySchedules();
    
    try
    {
        [self loadOrStartNewSession];
    }
    catch (std::exception& e)
    {
        // TODO: HACKY
        [self deleteStores];
        [self loadOrStartNewSession];
    }

    _handValue = 0;
    _handValueChanged = NO;
    _bet = 5;
    _maxBet = 5;
    
   
    _handMO = nil;
    _hand = NULL;
    
    return self;
}

- (BOOL)
loadOrStartNewSession
{
    using namespace PaySchedule;
    
    Game::Id_t defaultGameId = Game::JacksOrBetter;
    double defaultReturn = 99.54;
    
    if (nil != self.context)
    {

        // assign with self.session required to add ref count?
        self.session = [self loadLastSession];
        if (nil == _session)
        {
            uint32_t defaultBalance = 10000;
            uint32_t defaultDenomination = 25;
            
            // assign with self.session required to add ref count?
            self.session = [self startNewSession:defaultGameId
                                      withReturn:defaultReturn
                                         balance:defaultBalance
                                    denomination:defaultDenomination];
        }
        [self setPaySchedule: Manager_t::getPaySchedule((Game::Id_t)_session.gameIdValue, _session.gameReturnValue)
                  loadSolver: YES];
    }
    else
    {
        [self setPaySchedule: Manager_t::getPaySchedule(defaultGameId, defaultReturn)
                  loadSolver: YES];
    }

    return YES;
}

- (SessionMO*)
loadLastSession
{
    NSFetchRequest* request = [[NSFetchRequest alloc] init];
    NSEntityDescription* entity = [NSEntityDescription entityForName:[SessionMO entityName] inManagedObjectContext:_context];
    [request setEntity:entity];
    
    NSSortDescriptor* startDate = [[NSSortDescriptor alloc] initWithKey:@"startDate" ascending:NO];
    NSArray* descriptors = [[NSArray alloc] initWithObjects:startDate, nil];
    [request setSortDescriptors:descriptors];
    [descriptors release];
    [startDate release];
    
    [request setFetchLimit:1];

    SessionMO* session = nil;

    NSError* error;
    NSArray* results = [_context executeFetchRequest:request error:&error];
    [request release];
    if ((nil != results) && [results count] > 0)
    {
        NSLog(@"rows loaded = %lu", [results count]);
        session = (SessionMO*)[results objectAtIndex:0];
    }
    return session;
}

- (SessionMO*)
startNewSession:(PaySchedule::Game::Id_t) gameId
withReturn:     (double) gameReturn
balance:        (int) balanceValue
denomination:   (int) denominationValue
{
    SessionMO* session = [SessionMO insertInManagedObjectContext:self.context];
    [session setGameIdValue:       (int)gameId];
    [session setGameReturnValue:   gameReturn];
    [session setBalanceValue:      balanceValue];
    [session setDenominationValue: denominationValue];
    
    if (session.startDate == nil)
    {
        [session setStartDate: [NSDate date]];
    }

    return session;
}

- (BOOL)
switchToPaySchedule: (const PaySchedule::Lookup::Data_t&) schedule
{
    // Bail if same schedule
    if (&schedule == _schedule)
    {
        NSLog(@"Same pay schedule - ignoring");
        return NO;
    }

    // If a session is active, close it, and create a new one
    if (nil != _session)
    {
        int oldBalance = _session.balanceValue;
        int oldDenomination = _session.denominationValue;
        [self save];
        //[_session release];
        
        // assign with self.session required to add ref count?
        self.session = [self startNewSession: schedule.getGameId()
                                  withReturn: schedule.getReturn()
                                     balance: oldBalance
                                denomination: oldDenomination];
    }
    
    BOOL loadSolverFlag = NO;
    
    if (_schedule->getGameId() != schedule.getGameId())
    {
        // TODO: if base pay schedule is the same don't need to reload?
        delete _solver;
        [_deck release];
        loadSolverFlag = YES;
    }

    [self setPaySchedule:schedule loadSolver:loadSolverFlag];

    return YES;
}

- (void)
setPaySchedule: (const PaySchedule::Lookup::Data_t&) schedule
loadSolver: (BOOL) loadSolverFlag
{
    _schedule = &schedule;
    
    if (loadSolverFlag)
    {
        _solver = [self loadSolver: _schedule];

        int deckSize = (int)(PaySchedule::Manager_t::getGameData(_schedule->getGameId())._deckSize);
        _deck = [[Deck alloc] initWithSize: deckSize];
    }
}

- (Hand::Solver::Data_t*)
loadSolver: (const PaySchedule::Lookup::Data_t*) schedule
{
#if LOAD_SOLVER
    using namespace Hand::Solver::Lookup;
    using namespace PaySchedule;
    int deckSize = (int)(PaySchedule::Manager_t::getGameData(_schedule->getGameId())._deckSize);
    Data_t* data = new Data_t(*schedule, deckSize, 5);
    NSString* filePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"];
    filePath = [filePath stringByAppendingString:
                [NSString stringWithUTF8String: Manager_t::getGameFilename(schedule->getGameId()).c_str()]];
    data->load([filePath UTF8String]);
    return data;
#endif
}

- (void)
setCard: (Card_t) card
atIndex: (NSInteger) index
{
    _hand->setCard((int)index, card, *_solver);
}

- (void)
setCards: (const CardVector_t&) cards 
{
    _hand->setCards(cards, *_solver);
    _hand->clearHeldCards();
}

- (void)
deal
{
    // Shuffle deck and deal cards to CurrentHand
    [_deck shuffle];
    const int cardCount = 5;
    CardVector_t cards(cardCount);
    for (int cardIndex = 0; cardIndex < cardCount; ++cardIndex)
    {
        cards[cardIndex] = [_deck deal];        
    }
    [self setCards:cards];
    
    // Create new hand
    _handMO = [HandMO insertInManagedObjectContext:[_session managedObjectContext]];

    [_handMO setStartDate:[NSDate date]];
    [_handMO setBetValue:_bet];

    const CardVector_t& dealtCards = _hand->cards();
    Hand::Value_t dealtHandValue = _schedule->Match(Hand::getValue(dealtCards, *_schedule));
    [_handMO addDealtHand:Card_t::getUniqueValue(dealtCards) value:dealtHandValue ev:[self getEvForCardBits:0]];
    
    self.handValue = dealtHandValue;
    
    const CardVector_t& bestCards = _hand->getBestCards();
    Hand::Value_t bestHandValue = _schedule->Match(Hand::getValue(bestCards, *_schedule));
    [_handMO addBestHand:Card_t::getUniqueValue(bestCards) value:bestHandValue ev:[self getBestEv]];
    
    _handMO.session = _session;
    [_session addHandsObject:_handMO];
    [_session setBalanceValue:_session.balanceValue - _bet * _session.denominationValue];

    [self save];
}

- (uint32_t)
draw
{
    // here, we need to determine what the "held ev" is, so we can compare it
    // to "best ev" and increment "perfect hand count" accordingly

    // heldEv and bestEv are stored in _handMO
    
    const int cardCount = 5;
    for (int cardIndex = 0; cardIndex < cardCount; ++cardIndex)
    {
        if (!_hand->isHeldCard(_hand->getCard(cardIndex)))
        {
            _hand->setCard(cardIndex, [_deck deal]);
        }
    }

    const CardVector_t& finalCards = _hand->cards();
    Hand::Value_t finalHandValue = _schedule->Match(Hand::getValue(finalCards, *_schedule));
    [_handMO addFinalHand:Card_t::getUniqueValue(finalCards) value:finalHandValue ev:0.0];
    
    self.handValue = finalHandValue;
    
    uint32_t payout = 0;
    if (finalHandValue > 0)
    {
        payout = _schedule->getHandPayout(finalHandValue, _bet);
        if (payout > 0)
        {
            [_session setBalanceValue: _session.balanceValue + payout * _session.denominationValue];
        }
    }

    [_session setHandCountValue:_session.handCountValue + 1];

    if (_handMO.yourHand.evValue == _handMO.bestHand.evValue)
    {
        [_session setPerfectPlayCountValue:_session.perfectPlayCountValue + 1];
    }
        
    [self save];

    return payout;
}

- (Hand::Value_t)
handValue
{
    return _handValue;
}

- (void)
setHandValue: (Hand::Value_t) handValue
{
    _handValueChanged = (handValue == _handValue) ? NO : YES;
    _handValue = handValue;
}

- (double)
cash
{
    return _session.balanceValue / 100.0;
}

- (double)
updateEv
{
    const CardVector_t& heldCards = _hand->getHeldCards();
    double ev = ((Hand::Solver::Default::Results_t&)_solver->getResults(_hand->getCardBits(heldCards))).ev;
    [_handMO addOrUpdateYourHand:Card_t::getUniqueValue(heldCards) value:0 ev:ev];

    return ev;
}

- (void)
save
{
    [self saveContext];
}

- (void)
saveContext
{
    NSError *error = nil;
    if (_context != nil)
    {
        if ([_context hasChanges] && ![_context save: &error])
        {
            /*
             Replace this implementation with code to handle the error appropriately.
             
             abort() causes the application to generate a crash log and terminate. You should not 
             use this function in a shipping application, although it may be useful during development.
             If it is not possible to recover from the error, display an alert panel that instructs the
             user to quit the application by pressing the Home button.
             */
            NSLog(@"saveContext(): Unresolved error %@, %@", error, [error userInfo]);
            abort();
        } 
    }
}    


- (void)
betOne
{
    if (_bet < _maxBet)
    {
        _bet++;
    }
    else
    {
        _bet = 1;
    }
}

- (void)
betMax
{
    _bet = _maxBet;
}

- (double)
getBestEv
{
    return ((Hand::Solver::Default::Results_t&)_solver->getBestResults()).ev;
}

- (double)
getEvForCardBits: (uint32_t) bits
{
    return ((Hand::Solver::Default::Results_t&)_solver->getResults(bits)).ev;
}

#pragma mark -
#pragma mark Core Data stack

/**
 Returns the managed object context for the application.
 If the context doesn't already exist, it is created and bound to the persistent store coordinator for the application.
 */
- (NSManagedObjectContext*)
context
{
    if (_context != nil)
    {
        return _context;
    }
	
    NSPersistentStoreCoordinator *coordinator = self.stores;
    if (coordinator != nil)
    {
        _context = [[NSManagedObjectContext alloc] init];
        [_context setPersistentStoreCoordinator:coordinator];
    }
    return _context;
}

/**
 Returns the managed object model for the application.
 If the model doesn't already exist, it is created from the application's model.
 */
- (NSManagedObjectModel*)
model
{
    if (_model != nil)
    {
        return _model;
    }
    NSString *modelPath = [[NSBundle mainBundle] pathForResource: @"ManagedObjects" ofType: @"momd"];
    if (nil == modelPath)
    {
        modelPath = [[NSBundle mainBundle] pathForResource: @"ManagedObjects" ofType: @"mom"];
    }
    NSAssert(nil != modelPath, @"can't find model path in main bundle");
    NSURL *modelURL = [NSURL fileURLWithPath: modelPath];
    _model = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];    
    
    return _model;
}

/**
 Returns the persistent store coordinator for the application.
 If the coordinator doesn't already exist, it is created and the application's store added to it.
 */
 - (NSPersistentStoreCoordinator*)
stores
{
    if (_stores == nil)
    {
        [self initStores];
    }

    return _stores;
}
    
- (BOOL)
initStores
{
    NSURL* storeUrl = [GameModel storeURL];
	
    NSError* error = nil;
    NSPersistentStoreCoordinator* coordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:self.model]; // must use self.model here to init model

    if (![coordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeUrl options:nil error:&error])
    {
        /*
         Replace this implementation with code to handle the error appropriately.
         
         abort() causes the application to generate a crash log and terminate. You should not use this
         function in a shipping application, although it may be useful during development. If it is not
         possible to recover from the error, display an alert panel that instructs the user to quit the
         application by pressing the Home button.
         
         Typical reasons for an error here include:
         * The persistent store is not accessible;
         * The schema for the persistent store is incompatible with current managed object model.
         Check the error message to determine what the actual problem was.
         
         
         If the persistent store is not accessible, there is typically something wrong with the file path.
         Often, a file URL is pointing into the application's resources directory instead of a writeable directory.
         
         If you encounter schema incompatibility errors during development, you can reduce their frequency by:
         * Simply deleting the existing store:
         [[NSFileManager defaultManager] removeItemAtURL:storeURL error:nil]
         
         * Performing automatic lightweight migration by passing the following dictionary as the options parameter: 
         [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES],NSMigratePersistentStoresAutomaticallyOption,
         [NSNumber numberWithBool:YES], NSInferMappingModelAutomaticallyOption, nil];
         
         Lightweight migration will only work for a limited set of schema changes;
         consult "Core Data Model Versioning and Data Migration Programming Guide" for details.
         
         */
        NSLog(@"stores::addPersistentStoreWithType: %@, %@", error, [error userInfo]);
        if (error.code == NSPersistentStoreIncompatibleVersionHashError)
        {
            [self askDeleteStores:self];
        }
        else
        {
            abort();
        }

        [coordinator release];
    }
    else
    {
        _stores = coordinator;    
    }

    return YES;
}

- (void)
askDeleteStores:(id<UIAlertViewDelegate>)alertViewDelegate
{
    UIAlertView* alert = [[UIAlertView alloc] init];
    [alert setTitle:@"Confirm"];
    [alert setMessage:@"Delete all history?"];
    [alert setDelegate:alertViewDelegate];
    [alert addButtonWithTitle:@"No"];
    [alert addButtonWithTitle:@"Yes"];

    [alert show];
    [alert release];
}

- (void)
alertView:(UIAlertView*)alertView
clickedButtonAtIndex:(NSInteger)index
{
    if (index > 0)
    {
        [self deleteStores];
        [self loadOrStartNewSession];
//        [self initStores];
    }
    else
    {
        abort();
    }
}
       
- (void)
deleteStores
{
    NSArray* storeArray = [_stores persistentStores];
    for (NSPersistentStore* store in storeArray)
    {
        BOOL success = YES;
        NSError* storeError;
        if (![_stores removePersistentStore:store error:&storeError])
        {
            NSLog(@"deleteStores::removePersistentStore: error %@, %@", storeError, [storeError userInfo]);
            success = NO;
        }
        NSError* fileError;
        if (![[NSFileManager defaultManager] removeItemAtPath:store.URL.path error:&fileError])
        {
            NSLog(@"deleteStores::removeItemAtPath: error %@, %@", fileError, [fileError userInfo]);
            success = NO;
        }
        if (success)
        {
            NSLog(@"deleteStores: store deleted");
        }
    }

    NSError* fileError;
    if (![[NSFileManager defaultManager] removeItemAtPath:[GameModel storeURL].path error:&fileError])
    {
        NSLog(@"deleteStores::removeItemAtPath(storeURL): %@, %@", fileError, [fileError userInfo]);
    }
    else
    {
        NSLog(@"deleteStores: storeURL deleted");
    }
    
    [_stores release];
    _stores = nil;
}

@end
