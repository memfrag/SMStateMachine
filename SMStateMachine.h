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

@class SMStateMachine;
@class SMTransition;
@class SMStateTypeWildcard;

#define SMTransition(from, to) @[SMStateType(from), SMStateType(to)]
#define SMStateType(x) x.class

typedef Class SMStateType;

/**
 * State objects conform to the SMState protocol.
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
- (void)willEnterWithTransition:(SMTransition *)transition;

/**
 * Called after entering the current state.
 *
 * NOT called on the main thread.
 *
 * @param transition The transition object that triggered the call.
 */
- (void)didEnterWithTransition:(SMTransition *)transition;

/**
 * Called before exiting the current state.
 *
 * NOT called on the main thread.
 *
 * @param transition The transition object that triggered the call.
 */
- (void)willExitWithTransition:(SMTransition *)transition;

@end


/**
 * For specifying transition where the from or to state matches any state.
 */
@interface SMStateTypeAny : NSObject
@end


/**
 * The transition currently being performed.
 */
@interface SMTransition : NSObject

/**
 * The type of the state that is being transitioned from.
 */
@property (nonatomic, weak) SMStateType fromStateType;

/**
 * The type of the state that is being transitioned to.
 */
@property (nonatomic, weak) SMStateType toStateType;

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
 * The state the machine is currently in.
 */
@property (nonatomic, strong, readonly) id<SMState> currentState;

/**
 * Create a state machine that accepts the specified transitions.
 *
 * @param transitions Array of SMTransition(FromState, ToState) objects.
 * @param initialState The initial state of the machine.
 * @return On success, a state machine. On failure, i.e. invalid transitions, nil.
 */
+ (instancetype)stateMachineWithTransitions:(NSArray *)transitions
                               initialState:(id<SMState>)initialState;

/**
 * Go to a new state (asynchronously).
 * Ignores transitions that have not been added to the state machine.
 *
 * @return Returns YES if transition is legal.
 */
- (BOOL)goToState:(id<SMState>)toState;

@end
