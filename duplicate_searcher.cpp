#include <iostream>
#include <boost/program_options.hpp>
#include "duplicate_search_exceptions.h"
#include "duplicate_searcher.h"

namespace po = boost::program_options;

DuplicateSearcher::DuplicateSearcher()
{

}

bool DuplicateSearcher::scan()
{
	try {
		if(m_ScanDirs.empty())
			throw ScanDirException();
//		for(const auto& scan_dir : m_ScanDirs)
//		{

//		}
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return false;
	}
	return true;
}

void DuplicateSearcher::addScanDir(const std::string& scan_dir)
{
	m_ScanDirs.push_back(scan_dir);
}

void DuplicateSearcher::addScanExcludeDir(const std::string& scan_exclude_dir)
{
	m_ScanExludeDirs.push_back(scan_exclude_dir);
}

void DuplicateSearcher::setParamsFromCmdLineArgs(int argc, const char* argv[])
{
	try {
		po::options_description description{"Options"};
		description.add_options()
				("help,h", "Getting more information about command line parameters")
				("dirs,d", po::value<std::vector<std::string>>()->notifier(
						[this](const std::vector<std::string>& scan_dirs)
				{
					for(const auto& dir : scan_dirs)
						this->addScanDir(dir);
				}), "Directories to scan (there may be several)")

				("exclude-dirs,e", po::value<std::vector<std::string>>()->notifier(
					 [this](const std::vector<std::string>& scan_exclude_dirs)
				{
				 for(const auto& dir : scan_exclude_dirs)
					 this->addScanExcludeDir(dir);
				}), "Directories to exclude from scanning (there may be several)")

				("scan-level,l",  po::value<size_t>()->default_value(0),
				 "Scan level (one for all directories, 0 - only the specified directory)")

				("min-file-size,s",  po::value<size_t>()->default_value(1),
				 "Min file size, by default, all files larger than 1 byte are checked")

				("file-masks,m", "Masks of file names allowed for comparison (case-independent)")

				("file-block-size,b",  po::value<size_t>()->default_value(5),
				 "Size of the block that reads files")

				("hash-algorithm,a", po::value<std::string>()->default_value("md5"),
				 "One of the available hashing algorithms (crc32, md5)");

		po::variables_map variables_map;
		po::store(parse_command_line(argc, argv, description), variables_map);
		po::notify(variables_map);

		if (variables_map.count("help"))
			std::cout << description << '\n';
		else if (variables_map.count("dirs"))
		{
			for(const auto& dir : variables_map["dirs"].as<std::vector<std::string>>())
				std::cout << dir << ", ";
			std::cout << std::endl;
		}
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
