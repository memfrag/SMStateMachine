#import <Foundation/Foundation.h>
#import "SMStateMachine.h"

@interface MyInitialState : NSObject <SMState>
@end
@implementation MyInitialState
- (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)didEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
@end

@interface MyThirdState : NSObject <SMState>
@end
@implementation MyThirdState
- (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)didEnterWithTransition:(SMTransition *)transition {
    NSLog(@"%s", __func__);
    [transition.stateMachine goToState:[MyInitialState new]];
}
@end

@interface MyOtherOtherState : NSObject <SMState>
@end
@implementation MyOtherOtherState
- (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)didEnterWithTransition:(SMTransition *)transition {
    NSLog(@"%s", __func__);
    [transition.stateMachine goToState:[MyThirdState new]];
}
@end

@interface MyOtherState : NSObject <SMState>
@end
@implementation MyOtherState
- (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)didEnterWithTransition:(SMTransition *)transition {
    NSLog(@"%s", __func__);
    [transition.stateMachine goToState:[MyOtherOtherState new]];
}
@end

void runStateMachine()
{
    NSArray *transitions = @[SMTransition(MyInitialState, MyOtherState),
                             SMTransition(MyOtherOtherState, MyThirdState),
                             SMTransition(SMStateTypeAny, MyOtherOtherState)];
    
    SMStateMachine *stateMachine = [SMStateMachine stateMachineWithTransitions:transitions initialState:[MyInitialState new]];
    stateMachine.logTransitions = YES;
    [stateMachine goToState:[MyOtherState new]];
    
    sleep(3);
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        runStateMachine();
        
    }
    return 0;
}
