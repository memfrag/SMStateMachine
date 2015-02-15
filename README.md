# SMStateMachine

A GCD backed Objective-C state machine. MIT license.

## Step 1

A state is a class that conforms to the SMState protocol. There are four class
methods that handle the state transitions.

```
@interface MyInitialState : NSObject <SMState>
@end

@implementation MyInitialState
+ (instancetype)createStateContext { return [self new]; }
+ (void)willExitWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)willEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (void)didEnterWithTransition:(SMTransition *)transition { NSLog(@"%s", __func__); }
+ (SMState)didFireEvent:(id<SMEvent>)event {
	if (SMEvent(event) == SMEvent(MyStartEvent)) {
		return SMState(MyNextEvent);
	}
	return nil;
}
@end
```

## Step 2

Create an event to trigger a transition.

```
@interface MyStartEvent : NSObject <SMEvent>
@property (nonatomic, copy, readonly) NSString *message;
- (instancetype)initWithMessage:(NSString *)message;
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
```

## Step 3

Create an array of valid transitions that may occur in the state machine. This is for runtime validation of transitions.

```
NSArray *transitions = @[SMTransition(MyInitialState, MyNextState),
                         SMTransition(MyNextState, MyThirdState),
                         SMTransition(MyThirdState, MyNextState)];
```

## Step 4

Create the state machine. Feed it the array of valid transitions and the initial state that the state machine should start in.

```
SMStateMachine *stateMachine = [SMStateMachine stateMachineWithTransitions:transitions 
                                                              initialState:SMState(MyInitialState)];
```

## Step 5

Fire an event.

```
MyStartEvent *event = [[MyStartEvent alloc] initWithMessage:@"Hello, World!"];
[stateMachine fireEvent:event];
```

It's possible to fire events from within the `willEnterWithTransition:` and `didEnterWithTransition:` methods to trigger transitions.

## Full Example

See Example.m for a full example.
