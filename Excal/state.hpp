//
//  state.hpp
//  KE
//
//  Created by Mark Sands on 7/2/13.
//  Copyright (c) 2013 Mark Sands. All rights reserved.
//

#pragma once

#include <string>
#include <cmath>

class State {
    public:
        typedef std::nullptr_t value_type;
        
        std::string str;
        virtual ~State() { }
        
        template <typename T> bool isKindOfClass() {
            if (dynamic_cast<T>(this) != nullptr) {
                return true;
            }
            return false;
        }
        #define isKindOfClass(K) isKindOfClass<K>()
        
        template <typename T> struct GetValue {
            typename T::value_type operator()(std::shared_ptr<State> s) {
                return static_cast<T*>(s.get())->operator()();
            }
        };
};

class NullState : public State {
    public:
        std::nullptr_t operator()() const {
            return nullptr;
        }
};

// -----------------------------------------------------------------------------

class MultiCharacterState : public State {
    public:
        virtual ~MultiCharacterState() { }
};

class IntegralState : public MultiCharacterState {
    public:
        typedef int value_type;
        
        IntegralState(char character) {
            str = std::string(1, character);
        }
    
        IntegralState(float result) {
            std::ostringstream s;
            s << result;
            str = s.str();
        }
    
        int operator()() const {
            return stoi(str);
        }
};

class FloatState : public MultiCharacterState {
    public:
        typedef float value_type;
    
        FloatState(char character) {
            str = std::string(1, character);
        }
        
        FloatState(float result) {
            std::ostringstream s;
            s << result;
            str = s.str();
        }
    
        float operator()() const {
            return stol(str);
        }
};

// -----------------------------------------------------------------------------

class SingleCharacterState : public State {
    public:
        virtual ~SingleCharacterState() { }
};

class SymbolState : public SingleCharacterState {
    public:
        typedef char value_type;
        
        SymbolState(char character) {
            value = character;
        }
        
        char operator()() const {
            return value;
        }
    
        float operator()(float T, float U) {
            return impl_getOperator(T, U);
        }
        
    private:
        value_type value;
    
        virtual float impl_getOperator(float T, float U) = 0;
};

class PlusOperatorState : public SymbolState {
    public:
        PlusOperatorState(char character) : SymbolState(character) { }
    
        inline float impl_getOperator(float T, float U) __attribute__((always_inline)) {
            return T + U;
        }
};

class MinusOperatorState : public SymbolState {
    public:
        MinusOperatorState(char character) : SymbolState(character) { }

        inline float impl_getOperator(float T, float U) __attribute__((always_inline)) {
            return T - U;
        }
};

class MultiplyOperatorState : public SymbolState {
    public:
        MultiplyOperatorState(char character) : SymbolState(character) { }
    
        inline float impl_getOperator(float T, float U) __attribute__((always_inline)) {
            return T * U;
        }
};

class DivideOperatorState : public SymbolState {
    public:
        DivideOperatorState(char character) : SymbolState(character) { }
    
        inline float impl_getOperator(float T, float U) __attribute__((always_inline)) {
            return T / U;
        }
};

class PowerOfOperatorState : public SymbolState {
    public:
        PowerOfOperatorState(char character) : SymbolState(character) { }
    
        inline float impl_getOperator(float T, float U) __attribute__((always_inline)) {
            return pow(T, U);
        }
};

