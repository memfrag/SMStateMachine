//
// Copyright (C) 2015 by Martin Johannesson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#import <Foundation/Foundation.h>

@protocol SMState;
@protocol SMEvent;
@class SMStateMachine;
@class SMTransition;
@class SMStateTypeWildcard;

/**
 * Used to specify a valid transition in the transition specification.
 */
#define SMTransition(from, to) @[SMState(from), SMState(to)]

/**
 * @define SMState
 *
 * Represents a state for use in type comparisons.
 *
 * @param x Name of a state class.
 */
#define SMState(x) x.class

/**
 * @define SMEvent
 *
 * Represents an event for use in type comparisons.
 *
 * @param x Name of a event class.
 */
#define SMEvent(x) x.class

/**
 * @typedef SMState
 *
 * Represents a state.
 */
typedef Class<SMState> SMState;

/**
 * State classes conform to the SMState protocol.
 */
@protocol SMState <NSObject>

@optional

/**
 * Create an instance of the current state class.
 * If this method is not implemented, the default
 * init method is used.
 */
+ (instancetype)createStateContext;

/**
 * Called after exiting the previous state.
 *
 * NOT called on the main thread.
 *
 * @param transition The transition object that triggered the call.
 */
+ (void)willEnterWithTransition:(SMTransition *)transition;

/**
 * Called after entering the current state.
 *
 * NOT called on the main thread.
 *
 * @param transition The transition object that triggered the call.
 */
+ (void)didEnterWithTransition:(SMTransition *)transition;

/**
 * Called before exiting the current state.
 *
 * NOT called on the main thread.
 *
 * @param transition The transition object that triggered the call.
 */
+ (void)willExitWithTransition:(SMTransition *)transition;

/**
 * Called when an event is fired. Might trigger transition.
 * Events are received after didEnterWithTransition has been called
 * and before willExitWithTransition.
 *
 * If this method is not implemented, there is no way to
 * transition to any other state.
 *
 * Example:
 *
 * @return A state to transition to, or nil if no transition was triggered.
 */
+ (SMState)didFireEvent:(id<SMEvent>)event stateContext:(id<SMState>)stateContext;

@end


/**
 * For specifying transition where the from or to state matches any state.
 */
@interface SMStateAny : NSObject
@end


/**
 * Event that can be fired to trigger transitions.
 */
@protocol SMEvent <NSObject>
@end

/**
 * The transition currently being performed.
 */
@interface SMTransition : NSObject

/**
 * The type of the state that is being transitioned from.
 */
@property (nonatomic, weak) SMState fromState;

/**
 * The type of the state that is being transitioned to.
 */
@property (nonatomic, weak) SMState toState;

/**
 * The state machine that is performing the transition.
 */
@property (nonatomic, weak) SMStateMachine *stateMachine;

/**
 * The event that triggered this transition.
 */
@property (nonatomic, weak) id<SMEvent> event;

/**
 * Store custom data that needs to be passed
 * along in the context.
 */
@property (nonatomic, weak) NSMutableDictionary *transitionContext;

/**
 * This is an instance of the state class.
 */
@property (nonatomic, weak) id<SMState> stateContext;

@end


/**
 * A state machine that manages state asynchronously
 * on its own serial GCD dispatch queue.
 */
@interface SMStateMachine : NSObject

@property (nonatomic, assign) BOOL logTransitions;

/**
 * Create a state machine that accepts the specified transitions.
 *
 * @param transitions Array of SMTransition(FromState, ToState) objects.
 * @param initialState The initial state of the machine.
 * @return On success, a state machine. On failure, i.e. invalid transitions, nil.
 */
+ (instancetype)stateMachineWithTransitions:(NSArray *)transitions
                               initialState:(SMState)initialState;

/**
 * Fire an event that may trigger a transition.
 */
- (void)fireEvent:(id<SMEvent>)event;

@end
