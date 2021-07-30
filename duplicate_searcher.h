#ifndef DUPLICATESEARCHER_H
#define DUPLICATESEARCHER_H
#include "boost/filesystem.hpp"
#include <vector>
#include <string>
#include <unordered_map>

namespace fs = boost::filesystem;

class DuplicateSearcher
{
public:
    DuplicateSearcher();
    bool scan();
    void addScanDir(const std::string& scan_dir);
    void addExcludeScanDir(const std::string& scan_exclude_dir);
    void setScanLevel(const size_t scan_level);
    void setParamsFromCmdLineArgs(int argc, const char *argv[]);
    std::vector<std::vector<fs::path> > duplicateFiles() const;
private:
    void FindDuplicate(const fs::path & dir_path
                       , const fs::path& search_file_path
                       , std::vector<fs::path>& duplicate_files);
    void GetAllScanFiles(const fs::path & dir_path, std::unordered_map<std::string, bool>& all_scan_files);
private:
    std::vector<fs::path> m_ScanDirs;
    std::vector<fs::path> m_ExludeScanDirs;
    std::vector<std::vector<fs::path>> m_DuplicateFiles;
    std::unordered_map<std::string, bool> m_AllScanFiles;
    size_t m_ScanLevel;

};

#endif // DUPLICATESEARCHER_H
