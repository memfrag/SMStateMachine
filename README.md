# SMStateMachine

A GCD backed Objective-C state machine. MIT license.

## Example

```
#import <Foundation/Foundation.h>
#import "SMStateMachine.h"

@interface MyInitialState : NSObject <SMState>
@end
@implementation MyInitialState
- (void)dealloc { NSLog(@"%s", __func__); }
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
    [transition.stateMachine goToState:[MyInitialState new] userInfo:nil];
}
@end

@interface MyOtherState : NSObject <SMState>
@end
@implementation MyOtherState
- (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)didEnterWithTransition:(SMTransition *)transition {
    NSLog(@"%s", __func__);
    [transition.stateMachine goToState:[MyThirdState new] userInfo:nil];
}
@end

void runStateMachine()
{
    NSArray *transitions = @[SMTransition(MyInitialState, MyOtherState),
                             SMTransition(MyOtherState, MyThirdState),
                             SMTransition(MyThirdState, MyOtherState)];
    
    SMStateMachine *stateMachine = [SMStateMachine stateMachineWithTransitions:transitions initialState:[MyInitialState new]];
    stateMachine.logTransitions = YES;
    [stateMachine goToState:[MyOtherState new] userInfo:nil];
    
    sleep(3);
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        runStateMachine();
        
    }
    return 0;
}
```
