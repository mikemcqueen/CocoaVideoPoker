// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to FundingMO.h instead.

#import <CoreData/CoreData.h>


@class SessionMO;




@interface FundingMOID : NSManagedObjectID {}
@end

@interface _FundingMO : NSManagedObject {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (FundingMOID*)objectID;



@property (nonatomic, retain) NSNumber *amount;

@property int amountValue;
- (int)amountValue;
- (void)setAmountValue:(int)value_;

//- (BOOL)validateAmount:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSDate *date;

//- (BOOL)validateDate:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) SessionMO* session;
//- (BOOL)validateSession:(id*)value_ error:(NSError**)error_;



@end

@interface _FundingMO (CoreDataGeneratedAccessors)

@end

@interface _FundingMO (CoreDataGeneratedPrimitiveAccessors)

- (NSNumber*)primitiveAmount;
- (void)setPrimitiveAmount:(NSNumber*)value;

- (int)primitiveAmountValue;
- (void)setPrimitiveAmountValue:(int)value_;


- (NSDate*)primitiveDate;
- (void)setPrimitiveDate:(NSDate*)value;




- (SessionMO*)primitiveSession;
- (void)setPrimitiveSession:(SessionMO*)value;


@end
