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

class NumberState : public MultiCharacterState {
    public:
        typedef float value_type;
    
        NumberState(char character) {
            str = std::string(1, character);
        }
    
        NumberState(float expression) {
            std::ostringstream s;
            s << expression;
            str = s.str();
        }
        
        void append(char character) {
            str += character;
        }
    
        float operator()() const {
            return stof(str);
        }
};

// -----------------------------------------------------------------------------

class SingleCharacterState : public State {
    public:
        virtual ~SingleCharacterState() { }
};

class ParenthesisState : public SingleCharacterState {
    public:
        typedef char value_type;
    
        ParenthesisState(char character) {
            value = character;
        }
    
        virtual ~ParenthesisState() { }
    
    private:
        value_type value;
};

class RightParenthesisState : public ParenthesisState {
    public:
        RightParenthesisState(char character) : ParenthesisState(character) { }
};

class LeftParenthesisState : public ParenthesisState {
    public:
        LeftParenthesisState(char character) : ParenthesisState(character) { }
};

class SymbolState : public SingleCharacterState {
    public:
        typedef char value_type;
        typedef float number_type;
    
        SymbolState(char character) {
            value = character;
        }
    
        virtual ~SymbolState() { }
    
        inline char operator()() const __attribute__((always_inline)) {
            return value;
        }
    
        number_type operator()(number_type T, number_type U) {
            return impl_getOperator(T, U);
        }
    
        inline int compareOperatorPrecedence(SymbolState * rhs) __attribute__((always_inline)) {
            return this->impl_operatorPrecedence() - rhs->impl_operatorPrecedence();
        }
        
    private:
        value_type value;
    
        virtual float impl_getOperator(number_type T, number_type U) = 0;
        virtual int impl_operatorPrecedence() = 0;
};

class PlusOperatorState : public SymbolState {
    public:
        PlusOperatorState(char character) : SymbolState(character) { }
    
        inline float impl_getOperator(number_type T, number_type U) __attribute__((always_inline)) {
            return T + U;
        }

        inline int impl_operatorPrecedence() __attribute__((always_inline)) {
            return 1;
        }
};

class MinusOperatorState : public SymbolState {
    public:
        MinusOperatorState(char character) : SymbolState(character) { }

        inline float impl_getOperator(number_type T, number_type U) __attribute__((always_inline)) {
            return T - U;
        }

        inline int impl_operatorPrecedence() __attribute__((always_inline)) {
            return 1;
        }
};

class MultiplyOperatorState : public SymbolState {
    public:
        MultiplyOperatorState(char character) : SymbolState(character) { }
    
        inline float impl_getOperator(number_type T, number_type U) __attribute__((always_inline)) {
            return T * U;
        }

        inline int impl_operatorPrecedence() __attribute__((always_inline)) {
            return 2;
        }
};

class DivideOperatorState : public SymbolState {
    public:
        DivideOperatorState(char character) : SymbolState(character) { }
    
        inline float impl_getOperator(number_type T, number_type U) __attribute__((always_inline)) {
            return T / U;
        }

        inline int impl_operatorPrecedence() __attribute__((always_inline)) {
            return 2;
        }
};

class PowerOfOperatorState : public SymbolState {
    public:
        PowerOfOperatorState(char character) : SymbolState(character) { }
    
        inline float impl_getOperator(number_type T, number_type U) __attribute__((always_inline)) {
            return pow(T, U);
        }

        inline int impl_operatorPrecedence() __attribute__((always_inline)) {
            return 2;
        }
};

