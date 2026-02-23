#pragma once

#include <string>

class ErrorPrinter 
{
    public:

        // static ErrorPrinter* GetInstance() 
        // {
        //     static ErrorPrinter* instance = new ErrorPrinter();
        //     return instance;
        // }

        // ErrorPrinter(const ErrorPrinter&) = delete;
        // ErrorPrinter& operator=(const ErrorPrinter&) = delete;

        static void PrintError(const std::string& string);

    private:
    
        // ErrorPrinter() = default;
        // ~ErrorPrinter() = default;
};