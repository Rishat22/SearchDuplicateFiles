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

struct NoDuplicatesException : public std::exception
{
        const char* what () const throw ()
    {
        return "There are no duplicates in the specified directories";
    }
};

#endif // DUPLICATE_SEARCH_EXCEPTIONS_H
