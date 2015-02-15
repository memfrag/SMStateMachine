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

#define SMTransition(from, to) @[SMState(from), SMState(to)]
#define SMState(x) x.class
#define SMEvent(x) x.class

typedef Class<SMState> SMState;

/**
 * State classes conform to the SMState protocol.
 */
@protocol SMState <NSObject>

@optional

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
 * @return A state to transition to, or nil if no transition was triggered.
 */
+ (SMState)didFireEvent:(id<SMEvent>)event;

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
 * Store custom data that needs to be passed
 * along in the context.
 */
@property (nonatomic, weak) NSMutableDictionary *context;

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
