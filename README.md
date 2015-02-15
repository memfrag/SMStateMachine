# SMStateMachine

A GCD backed Objective-C state machine. MIT license.

## Step 1

A state is a class that conforms to the SMState protocol. There are four class
methods that handle the state transitions.

```
@interface MyInitialState : NSObject <SMState>
@end

@implementation MyInitialState
+ (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)didEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (SMState)didFireEvent:(id<SMEvent>)
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
[stateMachine goToState:[MyOtherState new]];
```

It's possible to go to a different state from within the `willEnterWithTransition:` and `didEnterWithTransition:` methods.

## Full Example

See Example.m for a full example.
