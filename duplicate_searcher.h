#ifndef DUPLICATESEARCHER_H
#define DUPLICATESEARCHER_H
#include <vector>
#include <string>
#include <map>
#include "boost/filesystem.hpp"
#include "FileComparators/ifile_comparator.h"

namespace fs = boost::filesystem;

struct ScannedFile
{
    ScannedFile(const fs::path file_path) : filePath(file_path), isPassed(false) {}
    fs::path filePath;
    bool isPassed;
};

class DuplicateSearcher
{
public:
    DuplicateSearcher();
    bool scan();
    std::vector<std::vector<fs::path> > duplicateFiles() const;

    void addScanDir(const std::string& scan_dir);
    void addExcludeScanDir(const std::string& scan_exclude_dir);
    void setScanLevel(const size_t scan_level);
    void setMinFileSize(const size_t min_file_size);
    void setParamsFromCmdLineArgs(int argc, const char *argv[]);
    void setFileComparator(const std::string& file_comparator);
    void setFileBlockSize(const size_t file_block_size);

private:
    void findDuplicate(const fs::path& search_file_path
                       , std::vector<fs::path>& duplicate_files);
    void getAllScanFiles(const fs::path & dir_path, std::vector<ScannedFile>& all_scan_files);
private:
    std::vector<fs::path> m_ScanDirs;
    std::vector<fs::path> m_ExludeScanDirs;
    std::vector<std::vector<fs::path>> m_DuplicateFiles;
    std::vector<ScannedFile> m_AllScanFiles;
    IFileComparator* m_FileComparator;
    size_t m_FileBlockSize;
    size_t m_ScanLevel;
    size_t m_MinFileSize;

};

#endif // DUPLICATESEARCHER_H
