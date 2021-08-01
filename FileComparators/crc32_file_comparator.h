#ifndef CRC32HASHFUNCTION_H
#define CRC32HASHFUNCTION_H
#include <boost/crc.hpp>
#include "ifile_comparator.h"

struct CRC32HashFunction {
  size_t operator()(const std::string& str) const;
};

class CRC32FileComparator : public IFileComparator
{
public:
    bool operator()(const fs::path& first_file,
                    const fs::path& second_file);
};

#endif // CRC32HASHFUNCTION_H
