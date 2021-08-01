#ifndef FILECOMPARATOR_H
#define FILECOMPARATOR_H
#include "boost/filesystem.hpp"

namespace fs = boost::filesystem;

class IFileComparator
{
public:
    virtual bool operator()(const fs::path& first_file,
                    const fs::path& second_file) = 0;
    void setFileBlockSize(const size_t file_block_size)
    {
        m_FileBlockSize = file_block_size;
    }

private:
    size_t m_FileBlockSize;
};
#endif // FILECOMPARATOR_H
