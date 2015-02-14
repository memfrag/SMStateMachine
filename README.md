# SMStateMachine

A GCD backed Objective-C state machine. MIT license.

## Step 1

Create state objects that adopt the SMState protocol.

```
@interface MyInitialState : NSObject <SMState>
@end

@implementation MyInitialState
- (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
- (void)didEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
@end
```

## Step 2

Create an array of valid transitions that may occur in the state machine. This is for runtime validation of transitions.

```
NSArray *transitions = @[SMTransition(MyInitialState, MyOtherState),
                         SMTransition(MyOtherState, MyThirdState),
                         SMTransition(MyThirdState, MyOtherState)];
```

## Step 3

Create the state machine. Feed it the array of valid transitions and the initial state that the state machine should start in.

```
SMStateMachine *stateMachine = [SMStateMachine stateMachineWithTransitions:transitions 
                                                              initialState:[MyInitialState new]];
```

## Step 4

Go to a state.

```
[stateMachine goToState:[MyOtherState new] userInfo:nil];
```

## Full Example

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
