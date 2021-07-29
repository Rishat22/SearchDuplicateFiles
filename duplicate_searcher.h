#ifndef DUPLICATESEARCHER_H
#define DUPLICATESEARCHER_H
#include <vector>
#include <string>

class DuplicateSearcher
{
public:
    DuplicateSearcher();
    bool scan();
    void addScanDir(const std::string& scan_dir);
    void addScanExcludeDir(const std::string& scan_exclude_dir);
    void setParamsFromCmdLineArgs(int argc, const char *argv[]);
private:
    std::vector<std::string> m_ScanDirs;
    std::vector<std::string> m_ScanExludeDirs;

};

#endif // DUPLICATESEARCHER_H
