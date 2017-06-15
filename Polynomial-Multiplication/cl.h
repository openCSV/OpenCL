#include <iostream>
#include <fstream>
#include <OpenCL/cl.hpp>

using namespace cl;

template<typename... ArgTypes> void console_log(ArgTypes... args);

template<typename T, typename... ArgTypes> void console_log(T t, ArgTypes... args)
{
    std::cout << t << ' ';
    console_log(args...);
}

template<> void console_log()
{
    std::cout << std::endl;
}

std::string getKernelCode(const char *c)
{
    std::string s;
    std::ifstream file(c);
    if(file.is_open())
    {
        std::string t;
        while(file >> t)
            s += t + '\n';
    }
    else
    {
        console_log("Kernel not found");
        exit(1);
    }
    return s;
}