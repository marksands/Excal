//
//  application.hpp
//  KE
//
//  Created by Mark Sands on 7/2/13.
//  Copyright (c) 2013 Mark Sands. All rights reserved.
//

#pragma once

#include <queue>
#include <stack>
#include <string>

#include "scanner.hpp"
#include "validator.hpp"
#include "state.hpp"

class Application {
    private:
        typedef std::stack<std::shared_ptr<State>> StateStack;
        typedef std::vector<std::shared_ptr<State>> StateVector;
        
        static void RotateOperatorPrecedence(StateStack& stack, StateVector& tokens, std::shared_ptr<SymbolState> state) {
            while (!stack.empty() && stack.top()->isKindOfClass(SymbolState *) && state->compareOperatorPrecedence(static_cast<SymbolState *>(stack.top().get())) <= 0) {
                   tokens.push_back(stack.top()); stack.pop();
            }
        }
    
        static float CalculateExpression(StateVector tokens) {
            StateStack tokenStack;
            
            for (auto state : tokens)
            {
                if (!state->isKindOfClass(SymbolState *)) {
                    tokenStack.push(state);
                }
                else {
                    auto state1 = tokenStack.top(); tokenStack.pop();
                    auto state2 = tokenStack.top(); tokenStack.pop();
                    
                    float d1 = State::GetValue<NumberState>()(state1);
                    float d2 = State::GetValue<NumberState>()(state2);
                    
                    float result = (*static_cast<SymbolState *>(state.get()))(d2, d1);
                    
                    std::shared_ptr<State> newState = std::make_shared<NumberState>(result);
                    tokenStack.push( newState );
                }
            }
            
            auto state = tokenStack.top();
            
            return State::GetValue<NumberState>()(state);
        }
    
    public:
        static void Run(int argc, const char * argv[])
        {
            // Scanner * scanner = new Scanner(argv[2]);
            Scanner * scanner = new Scanner("(2+1.14)*2");
            
            StateVector tokens;
            StateStack stack;
            
            while (scanner->valid()) {
                if (Accept(scanner->next(), NumberValidator())) {
                    auto number = std::make_shared<NumberState>(scanner->advance());
                    while (Accept(scanner->next(), NumberValidator()) || Accept(scanner->next(), PeriodValidator()) || Accept(scanner->next(), CommaValidator())) {
                        if (Accept(scanner->next(), CommaValidator())) {
                            scanner->advance();
                        } else {
                            number->append(scanner->advance());
                        }
                    }
                    tokens.push_back(number);
                }
                else if (Accept(scanner->next(), LeftParenthesisValidator())) {
                    auto leftParenthesis = std::make_shared<LeftParenthesisState>(scanner->advance());
                    stack.push(leftParenthesis);
                }
                else if (Accept(scanner->next(), RightParenthesisValidator())) {
                    auto rightParenthesis = std::make_shared<RightParenthesisState>(scanner->advance());
                    while (!stack.empty() && !stack.top()->isKindOfClass(LeftParenthesisState*)) {
                        tokens.push_back(stack.top()); stack.pop();
                    }
                    stack.pop();
                }
                else if (Accept(scanner->next(), PlusSymbolValidator())) {
                    auto plus = std::make_shared<PlusOperatorState>(scanner->advance());
                    RotateOperatorPrecedence(stack, tokens, plus);
                    stack.push(plus);
                }
                else if (Accept(scanner->next(), MinusSymbolValidator())) {
                    auto minus = std::make_shared<MinusOperatorState>(scanner->advance());
                    RotateOperatorPrecedence(stack, tokens, minus);
                    stack.push(minus);
                }
                else if (Accept(scanner->next(), DivideSymbolValidator())) {
                    auto divide = std::make_shared<DivideOperatorState>(scanner->advance());
                    RotateOperatorPrecedence(stack, tokens, divide);
                    stack.push(divide);
                }
                else if (Accept(scanner->next(), MultiplySymbolValidator())) {
                    auto multiply = std::make_shared<MultiplyOperatorState>(scanner->advance());
                    RotateOperatorPrecedence(stack, tokens, multiply);
                    stack.push(multiply);
                }
                else if (Accept(scanner->next(), PowerOfSymbolValidator())) {
                    auto powerOf = std::make_shared<PowerOfOperatorState>(scanner->advance());
                    RotateOperatorPrecedence(stack, tokens, powerOf);
                    stack.push(powerOf);
                }
                else {
                    scanner->advance();
                    // TODO: fix the weird EOF bug
                    // throw std::runtime_error("Invalid Expression!");
                }
            }
            
            while (!stack.empty()) {
                tokens.push_back(stack.top());
                stack.pop();
            }
        
            float result = CalculateExpression(tokens);

            std::cout << result << std::endl;
        }
};

