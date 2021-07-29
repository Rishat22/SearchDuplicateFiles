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
    void addScanDir(const std::string& scan_dir);
    void addScanExcludeDir(const std::string& scan_exclude_dir);
    void setParamsFromCmdLineArgs(int argc, const char *argv[]);
private:
    std::vector<fs::path> m_ScanDirs;
    std::vector<fs::path> m_ScanExludeDirs;

};

#endif // DUPLICATESEARCHER_H
