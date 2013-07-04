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
    
        static float CalculateExpression(std::queue<std::shared_ptr<State>> queue) {
            std::stack< std::shared_ptr<State> > tokenStack;
            
            while ( queue.size() > 0 )
            {
                auto state = queue.front();
                queue.pop();
                
                if (!state->isKindOfClass(SymbolState *)) {
                    tokenStack.push(state);
                }
                else {
                    auto state1 = tokenStack.top(); tokenStack.pop();
                    auto state2 = tokenStack.top(); tokenStack.pop();
                    
                    float d1 = State::GetValue<NumberState>()(state1);
                    float d2 = State::GetValue<NumberState>()(state2);
                    
                    float result = (*static_cast<SymbolState *>(state.get()))(d1, d2);
                    
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
            Scanner * scanner = new Scanner("6*7");
            
            std::vector< std::shared_ptr<State> > states;
        
            std::queue< std::shared_ptr<State> > queue;
            std::stack< std::shared_ptr<State> > stack;
            
            while (scanner->valid()) {
                if (Accept(scanner->next(), IntegralValidator())) {
                    char c = scanner->advance();
                    
                    states.push_back( std::make_shared<NumberState>(c) );
                    queue.push( std::make_shared<NumberState>(c) );
                }
                else if (Accept(scanner->next(), FloatValidator())) {
                    char c = scanner->advance();
                    
                    states.push_back( std::make_shared<NumberState>(c) );
                    queue.push( std::make_shared<NumberState>(c) );
                }
                else if (Accept(scanner->next(), PeriodValidator())) {
                    states.push_back( std::make_shared<NumberState>(scanner->advance()) );
                }
                else if (Accept(scanner->next(), LeftParenthesisValidator())) {
                    // states.push_back( std::make_shared<SymbolState>(scanner->advance()) );
                }
                else if (Accept(scanner->next(), RightParenthesisValidator())) {
                    // states.push_back( std::make_shared<SymbolState>(scanner->advance()) );
                }
                else if (Accept(scanner->next(), PlusSymbolValidator())) {
                    char c = scanner->advance();
                    
                    states.push_back( std::make_shared<PlusOperatorState>(c) );
                    stack.push( std::make_shared<PlusOperatorState>(c) );
                }
                else if (Accept(scanner->next(), MinusSymbolValidator())) {
                    char c = scanner->advance();
                    
                    states.push_back( std::make_shared<MinusOperatorState>(c) );
                    stack.push( std::make_shared<MinusOperatorState>(c) );
                }
                else if (Accept(scanner->next(), DivideSymbolValidator())) {
                    char c = scanner->advance();
                    
                    states.push_back( std::make_shared<DivideOperatorState>(c) );
                    stack.push( std::make_shared<DivideOperatorState>(c) );
                }
                else if (Accept(scanner->next(), MultiplySymbolValidator())) {
                    char c = scanner->advance();
                    
                    states.push_back( std::make_shared<MultiplyOperatorState>(c) );
                    stack.push( std::make_shared<MultiplyOperatorState>(c) );
                }
                else if (Accept(scanner->next(), PowerOfSymbolValidator())) {
                    char c = scanner->advance();
                    
                    states.push_back( std::make_shared<PowerOfOperatorState>(c) );
                    stack.push( std::make_shared<PowerOfOperatorState>(c) );
                }
                else {
                    scanner->advance();
                    // TODO: fix the weird EOF bug
                    // throw std::runtime_error("Invalid Expression!");
                }
            }
            
            while (stack.size() > 0) {
                auto t = stack.top();
                queue.push(t);
                stack.pop();
            }
        
            float result = CalculateExpression(queue);
        
            std::cout << result << std::endl;
        }
};

