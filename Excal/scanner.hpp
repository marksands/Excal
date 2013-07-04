//
//  scanner.hpp
//  KE
//
//  Created by Mark Sands on 7/2/13.
//  Copyright (c) 2013 Mark Sands. All rights reserved.
//

#pragma once

#include <sstream>
#include <string>
#include <memory>

class Scanner {
    private:
        std::string expression;
        std::shared_ptr<std::stringstream> stream;
    
    public:
        Scanner(const std::string& exp) : expression(exp) {
            stream = std::make_shared<std::stringstream>(expression);
        }
        
        inline char next() __attribute__((always_inline)) {
            return stream->peek();
        }

        inline char advance() __attribute__((always_inline)) {
            return stream->get();
        }

        inline void previous() __attribute__((always_inline)) {
            stream->unget();
        }

        inline bool valid() __attribute__((always_inline)) {
            return stream->good() && !stream->eof();
        }

    public:
        void reverse(unsigned int amount = 1) {
            for (int i = 0; i < amount; ++i)
                previous();
        }
};

