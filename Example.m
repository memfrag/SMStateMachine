
#import <Foundation/Foundation.h>
#import "SMStateMachine.h"

@interface MyStartEvent : NSObject <SMEvent>
@end

@interface MyOtherEvent : NSObject <SMEvent>
@end


@interface MyInitialState : NSObject <SMState>
@end

@interface MyFirstState : NSObject <SMState>
@end

@interface MySecondState : NSObject <SMState>
@end

@interface MyThirdState : NSObject <SMState>
@end



@implementation MyStartEvent
@end

@implementation MyOtherEvent
@end

@implementation MyInitialState
+ (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)didEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (SMState)didFireEvent:(id<SMEvent>)event {
    NSLog(@"%s %@", __func__, NSStringFromClass(event.class));
    if (SMEvent(event) == SMEvent(MyStartEvent)) {
        return SMState(MyFirstState);
    }
    return nil;
}
@end

@implementation MyFirstState
+ (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)didEnterWithTransition:(SMTransition *)transition {
    NSLog(@"%s", __func__);
    [transition.stateMachine fireEvent:[MyOtherEvent new]];
}
+ (SMState)didFireEvent:(id<SMEvent>)event {
    NSLog(@"%s %@", __func__, NSStringFromClass(event.class));
    if (SMEvent(event) == SMEvent(MyOtherEvent)) {
        return SMState(MySecondState);
    }

    return nil;
}
@end

@implementation MySecondState
+ (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)didEnterWithTransition:(SMTransition *)transition {
    NSLog(@"%s", __func__);
    [transition.stateMachine fireEvent:[MyOtherEvent new]];
}
+ (SMState)didFireEvent:(id<SMEvent>)event {
    NSLog(@"%s %@", __func__, NSStringFromClass(event.class));
    if (SMEvent(event) == SMEvent(MyOtherEvent)) {
        return SMState(MyThirdState);
    }
    return nil;
}
@end

@implementation MyThirdState
+ (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)didEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (SMState)didFireEvent:(id<SMEvent>)event {
    NSLog(@"%s %@", __func__, NSStringFromClass(event.class));
    return nil;
}
@end



void runStateMachine()
{
    NSArray *transitions = @[SMTransition(MyInitialState, MyFirstState),
                             SMTransition(MyFirstState, MySecondState),
                             SMTransition(SMStateAny, MyThirdState)];
    
    SMStateMachine *stateMachine = [SMStateMachine stateMachineWithTransitions:transitions initialState:SMState(MyInitialState)];
    stateMachine.logTransitions = YES;
    [stateMachine fireEvent:[MyStartEvent new]];
    
    sleep(3);
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        runStateMachine();
        
    }
    return 0;
}
