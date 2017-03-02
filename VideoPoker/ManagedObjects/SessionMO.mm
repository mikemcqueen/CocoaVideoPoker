
#include "Platform.h"
#import "SessionMO.h"

@implementation SessionMO

- (PaySchedule::Game::Id_t)getGameId
{
    return (PaySchedule::Game::Id_t)self.gameIdValue;
}

@end
