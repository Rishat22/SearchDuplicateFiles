#ifndef CRC32HASHFUNCTION_H
#define CRC32HASHFUNCTION_H
#include <boost/crc.hpp>
#include "ifile_comparator.h"

class CRC32FileComparator : IFileComparator
{
public:
    bool operator()(const fs::path& first_file,
                    const fs::path& second_file);
};

struct CRC32HashFunction {
  size_t operator()(const std::string& str) const;
};

#endif // CRC32HASHFUNCTION_H
