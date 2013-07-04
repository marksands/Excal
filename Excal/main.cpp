//
//  main.cpp
//  KE
//
//  Created by Mark Sands on 7/2/13.
//  Copyright (c) 2013 Mark Sands. All rights reserved.
//

#include <iostream>
#include "application.hpp"

int main(int argc, const char * argv[])
{
    try {
        Application::Run(argc, argv);
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception caught. Bailing out." << std::endl;
    }
}

