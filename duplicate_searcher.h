#ifndef DUPLICATESEARCHER_H
#define DUPLICATESEARCHER_H
#include "boost/filesystem.hpp"
#include <vector>
#include <string>

namespace fs = boost::filesystem;

class DuplicateSearcher
{
public:
    DuplicateSearcher();
    bool scan();
    void FindDuplicate(const fs::path & dir_path);
    void FindDuplicate(const fs::path & dir_path
                       , const fs::path& search_file_path
                       , std::vector<fs::path>& duplicate_files);
    void addScanDir(const std::string& scan_dir);
    void addScanExcludeDir(const std::string& scan_exclude_dir);
    void setParamsFromCmdLineArgs(int argc, const char *argv[]);
    std::vector<std::vector<fs::path> > duplicateFiles() const;

private:
    std::vector<fs::path> m_ScanDirs;
    std::vector<fs::path> m_ScanExludeDirs;
    std::vector<std::vector<fs::path>> m_DuplicateFiles;

};

#endif // DUPLICATESEARCHER_H
