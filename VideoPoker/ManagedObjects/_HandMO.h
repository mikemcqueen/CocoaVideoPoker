// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to HandMO.h instead.

#import <CoreData/CoreData.h>


@class HandDataMO;
@class HandDataMO;
@class HandDataMO;
@class SessionMO;
@class HandDataMO;





@interface HandMOID : NSManagedObjectID {}
@end

@interface _HandMO : NSManagedObject {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (HandMOID*)objectID;



@property (nonatomic, retain) NSNumber *bet;

@property int betValue;
- (int)betValue;
- (void)setBetValue:(int)value_;

//- (BOOL)validateBet:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *payout;

@property int payoutValue;
- (int)payoutValue;
- (void)setPayoutValue:(int)value_;

//- (BOOL)validatePayout:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSDate *startDate;

//- (BOOL)validateStartDate:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) HandDataMO* bestHand;
//- (BOOL)validateBestHand:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) HandDataMO* yourHand;
//- (BOOL)validateYourHand:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) HandDataMO* finalHand;
//- (BOOL)validateFinalHand:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) SessionMO* session;
//- (BOOL)validateSession:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) HandDataMO* dealtHand;
//- (BOOL)validateDealtHand:(id*)value_ error:(NSError**)error_;



@end

@interface _HandMO (CoreDataGeneratedAccessors)

@end

@interface _HandMO (CoreDataGeneratedPrimitiveAccessors)

- (NSNumber*)primitiveBet;
- (void)setPrimitiveBet:(NSNumber*)value;

- (int)primitiveBetValue;
- (void)setPrimitiveBetValue:(int)value_;


- (NSNumber*)primitivePayout;
- (void)setPrimitivePayout:(NSNumber*)value;

- (int)primitivePayoutValue;
- (void)setPrimitivePayoutValue:(int)value_;


- (NSDate*)primitiveStartDate;
- (void)setPrimitiveStartDate:(NSDate*)value;




- (HandDataMO*)primitiveBestHand;
- (void)setPrimitiveBestHand:(HandDataMO*)value;



- (HandDataMO*)primitiveYourHand;
- (void)setPrimitiveYourHand:(HandDataMO*)value;



- (HandDataMO*)primitiveFinalHand;
- (void)setPrimitiveFinalHand:(HandDataMO*)value;



- (SessionMO*)primitiveSession;
- (void)setPrimitiveSession:(SessionMO*)value;



- (HandDataMO*)primitiveDealtHand;
- (void)setPrimitiveDealtHand:(HandDataMO*)value;


@end
