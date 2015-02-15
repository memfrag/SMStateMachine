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

#import "SMStateMachine.h"

@implementation SMStateAny
@end


@interface SMStateMachine ()

- (instancetype)init __attribute__((unavailable("init not available, call sharedInstance instead")));
+ (instancetype)new __attribute__((unavailable("new not available, call sharedInstance instead")));

@property (nonatomic, strong) NSArray *transitions;
@property (nonatomic, strong) SMState currentState;
@property (nonatomic, strong) NSMutableDictionary *context;

@end

@implementation SMTransition
@end

@implementation SMStateMachine {
    dispatch_queue_t _queue;
}

+ (instancetype)stateMachineWithTransitions:(NSArray *)transitions initialState:(SMState)initialState
{
    // Make sure the state types are actually states
    for (NSArray *entry in transitions) {
        id fromStateType = entry[0];
                
        if (!([fromStateType conformsToProtocol:@protocol(SMState)] || fromStateType == SMStateAny.class)) {
            return nil;
        }
        
        id toStateType = entry[1];
        if (!([toStateType conformsToProtocol:@protocol(SMState)] || toStateType == SMStateAny.class)) {
            return nil;
        }
    }
    
    SMStateMachine *stateMachine = [[SMStateMachine alloc] initWithTransitions:transitions initialState:initialState];
    return stateMachine;
}

- (instancetype)initWithTransitions:(NSArray *)transitions initialState:(SMState)initialState
{
    self = [super init];
    if (self) {
        [self setUpTransitions:transitions];
        [self setUpDispatchQueue];
        _context = [NSMutableDictionary dictionary];
        [self goToState:initialState];
    }
    return self;
}

- (void)setUpTransitions:(NSArray *)transitions
{
    self.transitions = [NSArray arrayWithArray:transitions];
}

- (void)setUpDispatchQueue
{
    _queue = dispatch_queue_create("SMStateMachine", DISPATCH_QUEUE_SERIAL);
}

- (BOOL)isLegalTransitionWithFromState:(SMState)fromState toState:(SMState)toState
{
    for (NSArray *entry in self.transitions) {
        Class fromStateType = entry[0];
        Class toStateType = entry[1];
        if ((fromStateType == SMStateAny.class || fromState == fromStateType)
            && (toStateType == SMStateAny.class || toState == toStateType)) {
            // Yes, this is a legal transition.
            return YES;
        }
    }
    
    return NO;
}

- (void)fireEvent:(id<SMEvent>)event
{
    __weak typeof(self) weakSelf = self;
    dispatch_async(_queue, ^{
        SMState requestedState = [weakSelf.currentState didFireEvent:event];
        if (requestedState) {
            [weakSelf goToState:requestedState];
        }
    });
}

- (BOOL)goToState:(SMState)toState
{
    if (toState == nil) {
        return NO;
    }
    
    if (![(Class)toState conformsToProtocol:@protocol(SMState)]) {
        return NO;
    }
    
    SMState fromState = self.currentState;
    
    if (fromState) {
        BOOL isLegal = [self isLegalTransitionWithFromState:fromState toState:toState];
        if (!isLegal) {
            NSLog(@"ERROR: Illegal transition: %@ -> %@",
                  NSStringFromClass(fromState),
                  NSStringFromClass(toState));
            return NO;
        }
    } else {
        // Initial state.
    }
    
    SMTransition *transition = [SMTransition new];
    transition.fromState = fromState ? fromState : nil /* initial state */;
    transition.toState = toState;
    transition.stateMachine = self;
    transition.context = _context;
    
    __weak typeof(self) weakSelf = self;
    
    if (fromState && [(Class)fromState respondsToSelector:@selector(willExitWithTransition:)]) {
        [fromState willExitWithTransition:transition];
    }

    if ([(Class)toState respondsToSelector:@selector(willEnterWithTransition:)]) {
        [toState willEnterWithTransition:transition];
    }

    self.currentState = toState;

    if (weakSelf.logTransitions) {
        NSLog(@"Transition: %@ -> %@",
              NSStringFromClass(fromState),
              NSStringFromClass(toState));
    }
    
    if ([(Class)toState respondsToSelector:@selector(didEnterWithTransition:)]) {
        [toState didEnterWithTransition:transition];
    }
    
    return YES;
}

@end
