#include "error_printer.h"
#include <iostream>


void ErrorPrinter::PrintError(const std::string& string)
{
    std::cout << "\033[31m" << "Error: " << string << "\033[0m" << std::endl;
}