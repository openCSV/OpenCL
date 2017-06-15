#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <OpenCL/cl.hpp>

using namespace cl;

template<typename... ArgTypes> void console_log(ArgTypes... args);

template<typename T, typename... ArgTypes> void console_log(T t, ArgTypes... args)
{
    std::cerr << t << ' ';
    console_log(args...);
}

template<> void console_log()
{
    std::cerr << std::endl;
}

std::string readFile(const char *c)
{
    std::string s;
    std::ifstream file(c);
    if(file.is_open())
    {
        std::string t;
        while(file >> t)
            s += t + ' ';
    }
    else
    {
        console_log("Kernel not found");
        exit(1);
    }
    return s;
}

std::string getKernelCode(const char *c)
{
    return readFile(c);
}

bool checkOutput(int a[], const int n)
{
    std::string s = readFile("testing/ans");
    std::stringstream ss(s);
    for(int i=0; i<n; i++)
    {
        int t;
        ss >> t;
        if(t != a[i])
            return false;
    }
    return true;
}