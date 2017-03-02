
#include "Platform.h"

#import "HandMO.h"
#import "HandDataMO.h"

@implementation HandMO

- (void) 
addDealtHand: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev
{
    if (nil != self.dealtHand)
    {
        throw runtime_error("addDealtHand: dealtHand non-nil");
    }
    [self addHand: @selector(setPrimitiveDealtHand:) withCards: cards value: value ev: ev];
}

- (void) 
addBestHand: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev
{
    if (nil != self.bestHand)
    {
        throw runtime_error("addDealtHand: bestHand non-nil");
    }
    [self addHand: @selector(setPrimitiveBestHand:) withCards: cards value: value ev: ev];
}

- (void) 
addOrUpdateYourHand: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev
{
    if (nil == self.yourHand)
    {
        [self addHand: @selector(setPrimitiveYourHand:) withCards: cards value: value ev: ev];
    }
    else
    {
        [self updateHand: self.yourHand withCards: cards value: value ev: ev];
    }
}

- (void) 
addFinalHand: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev
{
    if (nil != self.finalHand)
    {
        throw runtime_error("addDealtHand: finalHand non-nil");
    }
    [self addHand: @selector(setPrimitiveFinalHand:) withCards: cards value: value ev: ev];
}

- (void) 
addHand: (SEL) selector
withCards: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev
{
    NSManagedObjectContext* moc = [self managedObjectContext];
    HandDataMO *data = [HandDataMO insertInManagedObjectContext: moc];
    if (nil == data)
    {
        throw runtime_error("addHand() data == nil");
    }
    [self performSelector: selector withObject: data];
    data.hand = self;
    [self updateHand: data withCards: cards value: value ev: ev];
}
    
- (void) 
updateHand: (HandDataMO*) data
withCards: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev
{
    [data setCardsValue: (int)cards];
    [data setValueValue: (int)value];
    [data setEvValue: ev];
}

@end
