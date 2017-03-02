//
//  GameModel.h
//  VideoPoker
//
//  Created by Mike McQueen on 7/24/10.
//  Copyright 2010 Amazon.com. All rights reserved.
//

#include "SolverData_t.hpp"
#include "CurrentHand.hpp"
#include "Hand.hpp"
#include "PaySchedule.hpp"

@class Deck;
@class SessionMO;
@class HandMO;

//class PaySchedule_t;

@interface GameModel :
    NSObject<UIAlertViewDelegate>
{
@public

    Deck*                           _deck;
    const PaySchedule::Lookup::Data_t* _schedule;
    Hand::Solver::Data_t*           _solver;
    CurrentHand*                    _hand;
    uint32_t                        _bet;
    uint32_t                        _maxBet;
    Hand::Value_t                   _handValue;
    BOOL                            _handValueChanged;

@private
    
    NSManagedObjectModel*           _model;
    NSManagedObjectContext*         _context;
    NSPersistentStoreCoordinator*   _stores;
    
    SessionMO*                      _session;
    HandMO*                         _handMO;
}

@property (nonatomic, readonly, retain) NSManagedObjectModel*           model;
@property (nonatomic, readonly, retain) NSManagedObjectContext*         context;
@property (nonatomic, readonly, retain) NSPersistentStoreCoordinator*   stores;

@property (nonatomic, retain)           SessionMO*                      session;
@property (nonatomic, readonly, assign) Deck*                           deck;
@property (nonatomic, readonly, assign) const PaySchedule::Lookup::Data_t*            schedule;
@property (nonatomic, readonly, assign) Hand::Solver::Data_t*           solver;

@property (nonatomic, assign)           CurrentHand*                    hand;

@property (nonatomic, /*readonly,*/ assign) Hand::Value_t               handValue;

@property (nonatomic, readonly, assign) BOOL                            handValueChanged;
@property (nonatomic, readonly, assign) uint32_t                        bet;
@property (nonatomic, readonly, assign) double                          cash;

+ (NSURL*) storeURL;
+ (NSString *) applicationDocumentsDirectory;

- (id) init;

- (BOOL) initStores;
- (void) askDeleteStores:(id<UIAlertViewDelegate>)alertViewDelegate;
- (void) deleteStores;

- (BOOL) loadOrStartNewSession;
- (SessionMO*) loadLastSession;
- (SessionMO*) startNewSession:(PaySchedule::Game::Id_t) gameId
                    withReturn:(double) gameReturn
                       balance:(int) balanceValue
                  denomination:(int) denominationValue;

- (Hand::Solver::Data_t*) loadSolver: (const PaySchedule::Lookup::Data_t*) schedule;

- (void) deal;
- (uint32_t) draw;
- (double) updateEv;
- (void) save;
- (void) saveContext;

- (void) setCard:(Card_t)card
         atIndex:(NSInteger)index;

- (void) setCards:(const CardVector_t&)cards;

- (void) betOne;
- (void) betMax;

- (double) getBestEv;
- (double) getEvForCardBits:(uint32_t)bits;

- (BOOL) switchToPaySchedule:(const PaySchedule::Lookup::Data_t&)schedule;
- (void) setPaySchedule:(const PaySchedule::Lookup::Data_t&)schedule
             loadSolver:(BOOL)loadSolverFlag;

@end

@protocol GameModelContainer

- (GameModel*) getGameModel;

@end
