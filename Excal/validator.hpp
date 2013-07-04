//
//  validator.hpp
//  KE
//
//  Created by Mark Sands on 7/2/13.
//  Copyright (c) 2013 Mark Sands. All rights reserved.
//

#pragma once

#include <algorithm>

template <typename T>
struct Validator : std::unary_function<T, bool> {
    inline bool operator()(char character) const __attribute__((always_inline)) {
        return impl_validation(character);
    }
private:
    virtual bool impl_validation(char character) const = 0;
};

struct NumberValidator : Validator<NumberValidator> {
    bool impl_validation(char character) const {
        return character >= '0' && character <= '9';
    }
};

struct PeriodValidator : Validator<PeriodValidator> {
    bool impl_validation(char character) const {
        return character == '.';
    }
};

struct LeftParenthesisValidator : Validator<LeftParenthesisValidator> {
    bool impl_validation(char character) const {
        return character == '(';
    }
};

struct RightParenthesisValidator : Validator<RightParenthesisValidator> {
    bool impl_validation(char character) const {
        return character == ')';
    }
};

struct PlusSymbolValidator : Validator<PlusSymbolValidator> {
    bool impl_validation(char character) const {
        return character == '+';
    }
};

struct MinusSymbolValidator : Validator<MinusSymbolValidator> {
    bool impl_validation(char character) const {
        return character == '-';
    }
};

struct MultiplySymbolValidator : Validator<MultiplySymbolValidator> {
    bool impl_validation(char character) const {
        return character == '*';
    }
};

struct DivideSymbolValidator : Validator<DivideSymbolValidator> {
    bool impl_validation(char character) const {
        return character == '/';
    }
};

struct PowerOfSymbolValidator : Validator<PowerOfSymbolValidator> {
    bool impl_validation(char character) const {
        return character == '^';
    }
};

template <typename T>
inline __attribute__((always_inline)) bool Accept(char expression, T predicate) {
    return predicate(expression);
}

