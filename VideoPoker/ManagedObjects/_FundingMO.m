// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to FundingMO.m instead.

#import "_FundingMO.h"

@implementation FundingMOID
@end

@implementation _FundingMO

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"funding" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"funding";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"funding" inManagedObjectContext:moc_];
}

- (FundingMOID*)objectID {
	return (FundingMOID*)[super objectID];
}




@dynamic amount;



- (int)amountValue {
	NSNumber *result = [self amount];
	return [result intValue];
}

- (void)setAmountValue:(int)value_ {
	[self setAmount:[NSNumber numberWithInt:value_]];
}

- (int)primitiveAmountValue {
	NSNumber *result = [self primitiveAmount];
	return [result intValue];
}

- (void)setPrimitiveAmountValue:(int)value_ {
	[self setPrimitiveAmount:[NSNumber numberWithInt:value_]];
}





@dynamic date;






@dynamic session;

	



@end
