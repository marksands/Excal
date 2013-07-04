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
    
        static float CalculateExpression(std::vector<std::shared_ptr<State>> tokens) {
            std::stack< std::shared_ptr<State> > tokenStack;
            
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
            // uncomment to enable command line support
            //Scanner * scanner = new Scanner(argv[2]);
            Scanner * scanner = new Scanner("6-2-2");
            
            std::vector< std::shared_ptr<State> > tokens;
            std::stack< std::shared_ptr<State> > stack;
            
            while (scanner->valid()) {
                if (Accept(scanner->next(), NumberValidator())) {
                    auto number = std::make_shared<NumberState>(scanner->advance());
                    while (Accept(scanner->next(), NumberValidator()) || Accept(scanner->next(), PeriodValidator())) {
                        number->append(scanner->advance());
                    }
                    tokens.push_back(number);
                }
                else if (Accept(scanner->next(), LeftParenthesisValidator())) {
                    stack.push( std::make_shared<LeftParenthesisState>(scanner->advance()));
                }
                else if (Accept(scanner->next(), RightParenthesisValidator())) {
                    stack.push( std::make_shared<RightParenthesisState>(scanner->advance()));
                }
                else if (Accept(scanner->next(), PlusSymbolValidator())) {
                    stack.push( std::make_shared<PlusOperatorState>(scanner->advance()) );
                }
                else if (Accept(scanner->next(), MinusSymbolValidator())) {

                    // TODO: this might not be working, not entirely sure what's going on.
                    auto minus = std::make_shared<MinusOperatorState>(scanner->advance());
                    while ( !stack.empty() && stack.top()->isKindOfClass(SymbolState *) )
                    {
                        if (minus->compareOperatorPrecedence( static_cast<SymbolState *>(stack.top().get()) ) <= 0) {
                            tokens.push_back(stack.top()); stack.pop();
                            continue;
                        }
                    
                        break;
                    }
                    
                    stack.push(minus);
                }
                else if (Accept(scanner->next(), DivideSymbolValidator())) {
                    stack.push( std::make_shared<DivideOperatorState>(scanner->advance()) );
                }
                else if (Accept(scanner->next(), MultiplySymbolValidator())) {
                    stack.push( std::make_shared<MultiplyOperatorState>(scanner->advance()) );
                }
                else if (Accept(scanner->next(), PowerOfSymbolValidator())) {
                    stack.push( std::make_shared<PowerOfOperatorState>(scanner->advance()) );
                }
                else {
                    scanner->advance();
                    // TODO: fix the weird EOF bug
                    // throw std::runtime_error("Invalid Expression!");
                }
            }
            
            while (stack.size() > 0) {
                auto t = stack.top();
                tokens.push_back(t);
                stack.pop();
            }
        
            float result = CalculateExpression(tokens);
        
            std::cout << result << std::endl;
        }
};

