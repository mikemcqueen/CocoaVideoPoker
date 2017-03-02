// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to HandDataMO.h instead.

#import <CoreData/CoreData.h>


@class HandMO;





@interface HandDataMOID : NSManagedObjectID {}
@end

@interface _HandDataMO : NSManagedObject {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (HandDataMOID*)objectID;



@property (nonatomic, retain) NSNumber *ev;

@property double evValue;
- (double)evValue;
- (void)setEvValue:(double)value_;

//- (BOOL)validateEv:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *value;

@property int valueValue;
- (int)valueValue;
- (void)setValueValue:(int)value_;

//- (BOOL)validateValue:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *cards;

@property int cardsValue;
- (int)cardsValue;
- (void)setCardsValue:(int)value_;

//- (BOOL)validateCards:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) HandMO* hand;
//- (BOOL)validateHand:(id*)value_ error:(NSError**)error_;



@end

@interface _HandDataMO (CoreDataGeneratedAccessors)

@end

@interface _HandDataMO (CoreDataGeneratedPrimitiveAccessors)

- (NSNumber*)primitiveEv;
- (void)setPrimitiveEv:(NSNumber*)value;

- (double)primitiveEvValue;
- (void)setPrimitiveEvValue:(double)value_;


- (NSNumber*)primitiveValue;
- (void)setPrimitiveValue:(NSNumber*)value;

- (int)primitiveValueValue;
- (void)setPrimitiveValueValue:(int)value_;


- (NSNumber*)primitiveCards;
- (void)setPrimitiveCards:(NSNumber*)value;

- (int)primitiveCardsValue;
- (void)setPrimitiveCardsValue:(int)value_;




- (HandMO*)primitiveHand;
- (void)setPrimitiveHand:(HandMO*)value;


@end
