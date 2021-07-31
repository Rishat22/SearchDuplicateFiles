#ifndef FILECOMPARATOR_H
#define FILECOMPARATOR_H
#include "boost/filesystem.hpp"

namespace fs = boost::filesystem;

class IFileComparator
{
public:
    virtual bool operator()(const fs::path& first_file,
                    const fs::path& second_file) = 0;
};
#endif // FILECOMPARATOR_H
