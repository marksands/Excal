#include <iostream>
#include "application.hpp"

int main(int argc, const char * argv[])
{
    std::cout << "Tests Addition: 2+2=4" << std::endl;
    const char *expression[] = {"system_path", "2+2"};
    Application::Run(1, expression);
    std::cout << std::endl;

    std::cout << "Tests Subtraction: 4-2=2" << std::endl;
    const char *expression2[] = {"system_path", "4-2"};
    Application::Run(1, expression2);
    std::cout << std::endl;
    
    std::cout << "Tests Multiplication: 4*2=8" << std::endl;
    const char *expression3[] = {"system_path", "4*2"};
    Application::Run(1, expression3);
    std::cout << std::endl;
    
    std::cout << "Tests Division: 6/2=3" << std::endl;
    const char *expression4[] = {"system_path", "6/2"};
    Application::Run(1, expression4);
    std::cout << std::endl;
    
    std::cout << "Tests Parens: (2+2)*4=16" << std::endl;
    const char *expression5[] = {"system_path", "(2+2)*4"};
    Application::Run(1, expression5);
    std::cout << std::endl;
    
    return 0;
}
