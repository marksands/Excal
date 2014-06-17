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

