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
 * The transition currently being performed.
 */
@interface SMTransition : NSObject

@property (nonatomic, weak) SMStateType fromStateType;
@property (nonatomic, weak) SMStateType toStateType;
@property (nonatomic, weak) SMStateMachine *stateMachine;
@property (nonatomic, strong) id userInfo;

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
+ (instancetype)stateMachineWithTransitions:(NSArray *)transitions initialState:(id<SMState>)initialState;

/**
 * Go to a new state (asynchronously).
 * Ignores transitions that have not been added to the state machine.
 *
 * @param userInfo Any object. It will be passed along in the transition.
 * @return Returns YES if transition is legal.
 */
- (BOOL)goToState:(id<SMState>)toState userInfo:(id)userInfo;

@end
