#ifndef EMPTY_FILE_COMPARATOR_H
#define EMPTY_FILE_COMPARATOR_H
#include "../duplicate_search_exceptions.h"
#include "ifile_comparator.h"


class EmptyFileComparator : public IFileComparator
{
public:
    bool operator()(const fs::path& /*first_file*/,
                    const fs::path& /*second_file*/) final
    {
        throw NoHashAlgorithmException();
    }

};
#endif // EMPTY_FILE_COMPARATOR_H
