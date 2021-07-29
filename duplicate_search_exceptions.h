#ifndef DUPLICATE_SEARCH_EXCEPTIONS_H
#define DUPLICATE_SEARCH_EXCEPTIONS_H
#include <iostream>
#include <exception>

struct ScanDirException : public std::exception
{
        const char* what () const throw ()
    {
        return "Directories to scan are not specified";
    }
};

#endif // DUPLICATE_SEARCH_EXCEPTIONS_H
