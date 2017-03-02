// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to SessionMO.h instead.

#import <CoreData/CoreData.h>


@class FundingMO;
@class HandMO;











@interface SessionMOID : NSManagedObjectID {}
@end

@interface _SessionMO : NSManagedObject {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (SessionMOID*)objectID;



@property (nonatomic, retain) NSNumber *balance;

@property int balanceValue;
- (int)balanceValue;
- (void)setBalanceValue:(int)value_;

//- (BOOL)validateBalance:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *gameReturn;

@property double gameReturnValue;
- (double)gameReturnValue;
- (void)setGameReturnValue:(double)value_;

//- (BOOL)validateGameReturn:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *gameId;

@property int gameIdValue;
- (int)gameIdValue;
- (void)setGameIdValue:(int)value_;

//- (BOOL)validateGameId:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSDate *endDate;

//- (BOOL)validateEndDate:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *denomination;

@property int denominationValue;
- (int)denominationValue;
- (void)setDenominationValue:(int)value_;

//- (BOOL)validateDenomination:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *perfectPlayCount;

@property int perfectPlayCountValue;
- (int)perfectPlayCountValue;
- (void)setPerfectPlayCountValue:(int)value_;

//- (BOOL)validatePerfectPlayCount:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *actualReturn;

@property double actualReturnValue;
- (double)actualReturnValue;
- (void)setActualReturnValue:(double)value_;

//- (BOOL)validateActualReturn:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSDate *startDate;

//- (BOOL)validateStartDate:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *handCount;

@property int handCountValue;
- (int)handCountValue;
- (void)setHandCountValue:(int)value_;

//- (BOOL)validateHandCount:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) NSSet* fundings;
- (NSMutableSet*)fundingsSet;



@property (nonatomic, retain) NSSet* hands;
- (NSMutableSet*)handsSet;



@end

@interface _SessionMO (CoreDataGeneratedAccessors)

- (void)addFundings:(NSSet*)value_;
- (void)removeFundings:(NSSet*)value_;
- (void)addFundingsObject:(FundingMO*)value_;
- (void)removeFundingsObject:(FundingMO*)value_;

- (void)addHands:(NSSet*)value_;
- (void)removeHands:(NSSet*)value_;
- (void)addHandsObject:(HandMO*)value_;
- (void)removeHandsObject:(HandMO*)value_;

@end

@interface _SessionMO (CoreDataGeneratedPrimitiveAccessors)

- (NSNumber*)primitiveBalance;
- (void)setPrimitiveBalance:(NSNumber*)value;

- (int)primitiveBalanceValue;
- (void)setPrimitiveBalanceValue:(int)value_;


- (NSNumber*)primitiveGameReturn;
- (void)setPrimitiveGameReturn:(NSNumber*)value;

- (double)primitiveGameReturnValue;
- (void)setPrimitiveGameReturnValue:(double)value_;


- (NSNumber*)primitiveGameId;
- (void)setPrimitiveGameId:(NSNumber*)value;

- (int)primitiveGameIdValue;
- (void)setPrimitiveGameIdValue:(int)value_;


- (NSDate*)primitiveEndDate;
- (void)setPrimitiveEndDate:(NSDate*)value;


- (NSNumber*)primitiveDenomination;
- (void)setPrimitiveDenomination:(NSNumber*)value;

- (int)primitiveDenominationValue;
- (void)setPrimitiveDenominationValue:(int)value_;


- (NSNumber*)primitivePerfectPlayCount;
- (void)setPrimitivePerfectPlayCount:(NSNumber*)value;

- (int)primitivePerfectPlayCountValue;
- (void)setPrimitivePerfectPlayCountValue:(int)value_;


- (NSNumber*)primitiveActualReturn;
- (void)setPrimitiveActualReturn:(NSNumber*)value;

- (double)primitiveActualReturnValue;
- (void)setPrimitiveActualReturnValue:(double)value_;


- (NSDate*)primitiveStartDate;
- (void)setPrimitiveStartDate:(NSDate*)value;


- (NSNumber*)primitiveHandCount;
- (void)setPrimitiveHandCount:(NSNumber*)value;

- (int)primitiveHandCountValue;
- (void)setPrimitiveHandCountValue:(int)value_;




- (NSMutableSet*)primitiveFundings;
- (void)setPrimitiveFundings:(NSMutableSet*)value;



- (NSMutableSet*)primitiveHands;
- (void)setPrimitiveHands:(NSMutableSet*)value;


@end
