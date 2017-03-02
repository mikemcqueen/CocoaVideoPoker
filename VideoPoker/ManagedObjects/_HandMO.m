// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to HandMO.m instead.

#import "_HandMO.h"

@implementation HandMOID
@end

@implementation _HandMO

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"hand" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"hand";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"hand" inManagedObjectContext:moc_];
}

- (HandMOID*)objectID {
	return (HandMOID*)[super objectID];
}




@dynamic bet;



- (int)betValue {
	NSNumber *result = [self bet];
	return [result intValue];
}

- (void)setBetValue:(int)value_ {
	[self setBet:[NSNumber numberWithInt:value_]];
}

- (int)primitiveBetValue {
	NSNumber *result = [self primitiveBet];
	return [result intValue];
}

- (void)setPrimitiveBetValue:(int)value_ {
	[self setPrimitiveBet:[NSNumber numberWithInt:value_]];
}





@dynamic payout;



- (int)payoutValue {
	NSNumber *result = [self payout];
	return [result intValue];
}

- (void)setPayoutValue:(int)value_ {
	[self setPayout:[NSNumber numberWithInt:value_]];
}

- (int)primitivePayoutValue {
	NSNumber *result = [self primitivePayout];
	return [result intValue];
}

- (void)setPrimitivePayoutValue:(int)value_ {
	[self setPrimitivePayout:[NSNumber numberWithInt:value_]];
}





@dynamic startDate;






@dynamic bestHand;

	

@dynamic yourHand;

	

@dynamic finalHand;

	

@dynamic session;

	

@dynamic dealtHand;

	



@end
