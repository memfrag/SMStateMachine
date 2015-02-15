
#import <Foundation/Foundation.h>
#import "SMStateMachine.h"

@interface MyStartEvent : NSObject <SMEvent>
@property (nonatomic, copy, readonly) NSString *message;
- (instancetype)initWithMessage:(NSString *)message;
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
- (instancetype)initWithMessage:(NSString *)message {
    self = [super init];
    if (self) {
        _message = [message copy];
    }
    return self;
}
@end

@implementation MyOtherEvent
@end

@implementation MyInitialState
+ (instancetype)createStateContext { NSLog(@"%s", __func__); return [self new]; }
+ (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)didEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (SMState)didFireEvent:(id<SMEvent>)event {
    NSLog(@"%s %@", __func__, NSStringFromClass(event.class));
    if (SMEvent(event) == SMEvent(MyStartEvent)) {
        NSLog(@"Message: %@", ((MyStartEvent *)event).message);
        return SMState(MyFirstState);
    }
    return nil;
}
@end

@implementation MyFirstState
+ (instancetype)createStateContext { NSLog(@"%s", __func__); return [self new]; }
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
+ (instancetype)createStateContext { NSLog(@"%s", __func__); return [self new]; }
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
    [stateMachine fireEvent:[[MyStartEvent alloc] initWithMessage:@"Hello, World!"]];
    
    sleep(3);
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        runStateMachine();
        
    }
    return 0;
}
