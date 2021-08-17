#ifndef MD5FILECOMPARATOR_H
#define MD5FILECOMPARATOR_H
#include <boost/uuid/detail/md5.hpp>
#include "ifile_comparator.h"

using boost::uuids::detail::md5;

class MD5FileComparator : public IFileComparator
{
public:
    bool operator()(const fs::path& first_file,
					const fs::path& second_file) override;
};

struct MD5HashFunction {
  size_t operator()(const std::string& str) const;
private:
  size_t toInt(const md5::digest_type &digest) const;
};

#endif // MD5FILECOMPARATOR_H
