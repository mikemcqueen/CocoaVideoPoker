// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to SessionMO.m instead.

#import "_SessionMO.h"

@implementation SessionMOID
@end

@implementation _SessionMO

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"session" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"session";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"session" inManagedObjectContext:moc_];
}

- (SessionMOID*)objectID {
	return (SessionMOID*)[super objectID];
}




@dynamic balance;



- (int)balanceValue {
	NSNumber *result = [self balance];
	return [result intValue];
}

- (void)setBalanceValue:(int)value_ {
	[self setBalance:[NSNumber numberWithInt:value_]];
}

- (int)primitiveBalanceValue {
	NSNumber *result = [self primitiveBalance];
	return [result intValue];
}

- (void)setPrimitiveBalanceValue:(int)value_ {
	[self setPrimitiveBalance:[NSNumber numberWithInt:value_]];
}





@dynamic gameReturn;



- (double)gameReturnValue {
	NSNumber *result = [self gameReturn];
	return [result doubleValue];
}

- (void)setGameReturnValue:(double)value_ {
	[self setGameReturn:[NSNumber numberWithDouble:value_]];
}

- (double)primitiveGameReturnValue {
	NSNumber *result = [self primitiveGameReturn];
	return [result doubleValue];
}

- (void)setPrimitiveGameReturnValue:(double)value_ {
	[self setPrimitiveGameReturn:[NSNumber numberWithDouble:value_]];
}





@dynamic gameId;



- (int)gameIdValue {
	NSNumber *result = [self gameId];
	return [result intValue];
}

- (void)setGameIdValue:(int)value_ {
	[self setGameId:[NSNumber numberWithInt:value_]];
}

- (int)primitiveGameIdValue {
	NSNumber *result = [self primitiveGameId];
	return [result intValue];
}

- (void)setPrimitiveGameIdValue:(int)value_ {
	[self setPrimitiveGameId:[NSNumber numberWithInt:value_]];
}





@dynamic endDate;






@dynamic denomination;



- (int)denominationValue {
	NSNumber *result = [self denomination];
	return [result intValue];
}

- (void)setDenominationValue:(int)value_ {
	[self setDenomination:[NSNumber numberWithInt:value_]];
}

- (int)primitiveDenominationValue {
	NSNumber *result = [self primitiveDenomination];
	return [result intValue];
}

- (void)setPrimitiveDenominationValue:(int)value_ {
	[self setPrimitiveDenomination:[NSNumber numberWithInt:value_]];
}





@dynamic perfectPlayCount;



- (int)perfectPlayCountValue {
	NSNumber *result = [self perfectPlayCount];
	return [result intValue];
}

- (void)setPerfectPlayCountValue:(int)value_ {
	[self setPerfectPlayCount:[NSNumber numberWithInt:value_]];
}

- (int)primitivePerfectPlayCountValue {
	NSNumber *result = [self primitivePerfectPlayCount];
	return [result intValue];
}

- (void)setPrimitivePerfectPlayCountValue:(int)value_ {
	[self setPrimitivePerfectPlayCount:[NSNumber numberWithInt:value_]];
}





@dynamic actualReturn;



- (double)actualReturnValue {
	NSNumber *result = [self actualReturn];
	return [result doubleValue];
}

- (void)setActualReturnValue:(double)value_ {
	[self setActualReturn:[NSNumber numberWithDouble:value_]];
}

- (double)primitiveActualReturnValue {
	NSNumber *result = [self primitiveActualReturn];
	return [result doubleValue];
}

- (void)setPrimitiveActualReturnValue:(double)value_ {
	[self setPrimitiveActualReturn:[NSNumber numberWithDouble:value_]];
}





@dynamic startDate;






@dynamic handCount;



- (int)handCountValue {
	NSNumber *result = [self handCount];
	return [result intValue];
}

- (void)setHandCountValue:(int)value_ {
	[self setHandCount:[NSNumber numberWithInt:value_]];
}

- (int)primitiveHandCountValue {
	NSNumber *result = [self primitiveHandCount];
	return [result intValue];
}

- (void)setPrimitiveHandCountValue:(int)value_ {
	[self setPrimitiveHandCount:[NSNumber numberWithInt:value_]];
}





@dynamic fundings;

	
- (NSMutableSet*)fundingsSet {
	[self willAccessValueForKey:@"fundings"];
	NSMutableSet *result = [self mutableSetValueForKey:@"fundings"];
	[self didAccessValueForKey:@"fundings"];
	return result;
}
	

@dynamic hands;

	
- (NSMutableSet*)handsSet {
	[self willAccessValueForKey:@"hands"];
	NSMutableSet *result = [self mutableSetValueForKey:@"hands"];
	[self didAccessValueForKey:@"hands"];
	return result;
}
	



@end
