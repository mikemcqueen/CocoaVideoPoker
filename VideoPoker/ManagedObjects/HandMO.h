#import "_HandMO.h"

#include "Hand.hpp"

@interface HandMO : _HandMO {}

- (void) 
addDealtHand: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev;

- (void) 
addBestHand: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev;

- (void) 
addOrUpdateYourHand: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev;

- (void) 
addFinalHand: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev;

- (void) 
addHand: (SEL) selector
withCards: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev;

- (void) 
updateHand: (HandDataMO*) data
withCards: (size_t) cards
value: (Hand::Value_t) value
ev: (double) ev;

@end
