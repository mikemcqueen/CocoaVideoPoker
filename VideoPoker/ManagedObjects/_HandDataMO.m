// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to HandDataMO.m instead.

#import "_HandDataMO.h"

@implementation HandDataMOID
@end

@implementation _HandDataMO

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"handData" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"handData";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"handData" inManagedObjectContext:moc_];
}

- (HandDataMOID*)objectID {
	return (HandDataMOID*)[super objectID];
}




@dynamic ev;



- (double)evValue {
	NSNumber *result = [self ev];
	return [result doubleValue];
}

- (void)setEvValue:(double)value_ {
	[self setEv:[NSNumber numberWithDouble:value_]];
}

- (double)primitiveEvValue {
	NSNumber *result = [self primitiveEv];
	return [result doubleValue];
}

- (void)setPrimitiveEvValue:(double)value_ {
	[self setPrimitiveEv:[NSNumber numberWithDouble:value_]];
}





@dynamic value;



- (int)valueValue {
	NSNumber *result = [self value];
	return [result intValue];
}

- (void)setValueValue:(int)value_ {
	[self setValue:[NSNumber numberWithInt:value_]];
}

- (int)primitiveValueValue {
	NSNumber *result = [self primitiveValue];
	return [result intValue];
}

- (void)setPrimitiveValueValue:(int)value_ {
	[self setPrimitiveValue:[NSNumber numberWithInt:value_]];
}





@dynamic cards;



- (int)cardsValue {
	NSNumber *result = [self cards];
	return [result intValue];
}

- (void)setCardsValue:(int)value_ {
	[self setCards:[NSNumber numberWithInt:value_]];
}

- (int)primitiveCardsValue {
	NSNumber *result = [self primitiveCards];
	return [result intValue];
}

- (void)setPrimitiveCardsValue:(int)value_ {
	[self setPrimitiveCards:[NSNumber numberWithInt:value_]];
}





@dynamic hand;

	



@end
