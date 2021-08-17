#include <iostream>
#include <boost/program_options.hpp>
#include "FileComparators/crc32_file_comparator.h"
#include "FileComparators/md5_file_comparator.h"
#include "FileComparators/md5_file_comparator.h"
#include "FileComparators/empty_file_comparator.h"
#include "duplicate_search_exceptions.h"
#include "duplicate_searcher.h"

namespace po = boost::program_options;

DuplicateSearcher::DuplicateSearcher()
	: m_FileComparator(new EmptyFileComparator)
	, m_FileBlockSize(5)
	, m_ScanLevel(0)
	, m_MinFileSize(1)
{

}

bool DuplicateSearcher::scan()
{
	try {
		if(m_ScanDirs.empty())
			throw ScanDirException();
		for(const auto& scan_dir : m_ScanDirs)
		{
			getAllScanFiles(scan_dir, m_AllScanFiles);
			for(auto& file : m_AllScanFiles)
			{
				if(file.isPassed == false)
				{
					file.isPassed = true;
					std::vector<fs::path> duplicate_files;
					findDuplicate( file.filePath, duplicate_files );
					if( !duplicate_files.empty() )
					{
						duplicate_files.push_back(file.filePath);
						m_DuplicateFiles.emplace_back(duplicate_files);
					}

				}
			}
		}
		if(m_DuplicateFiles.empty())
			throw NoDuplicatesException();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return false;
	}
	return true;
}

void DuplicateSearcher::getAllScanFiles(const fs::path & dir_path, std::vector<ScannedFile>& all_scan_files)
{
	if( !exists( dir_path ) )
		return;
	size_t curr_scan_level = 0;
	fs::directory_iterator end_itr;
	for( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
	  if( is_directory(itr->status()) )
	  {
		  if(curr_scan_level++ >= m_ScanLevel)
			  continue;
		  if(std::find(m_ExludeScanDirs.begin(), m_ExludeScanDirs.end(), itr->path()) != m_ExludeScanDirs.end())
			  continue;
		  getAllScanFiles( itr->path(), all_scan_files );
	  }
	  else
	  {
		  if( fs::file_size( itr->path() ) <= m_MinFileSize )
			  continue;
		  all_scan_files.emplace_back(itr->path().c_str());
	  }
	}
}


void DuplicateSearcher::findDuplicate(const fs::path& search_file_path, std::vector<fs::path>& duplicate_files)
{
	for(auto& file : m_AllScanFiles)
	{
		if( file.isPassed )
			continue;
		if( m_FileComparator->operator()( search_file_path, file.filePath ) )
		{
			duplicate_files.push_back(file.filePath);
			file.isPassed = true;
		}
	}
}

void DuplicateSearcher::setupFileComparator(const std::string& str_file_comparator)
{
	static std::map<std::string, std::shared_ptr<IFileComparator>> file_comparators
	{
		{ "md5",  std::make_shared<MD5FileComparator>()},
		{ "crc32", std::make_shared<CRC32FileComparator>()}
	};
	const auto file_comparator = file_comparators.find(str_file_comparator);
	if( file_comparator == file_comparators.end())
		throw NoHashAlgorithmException();
	m_FileComparator = file_comparator->second;
	m_FileComparator->setFileBlockSize(m_FileBlockSize);
}

void DuplicateSearcher::setFileBlockSize(const size_t file_block_size)
{
	m_FileBlockSize = file_block_size;
}

void DuplicateSearcher::addScanDir(const std::string& scan_dir)
{
	m_ScanDirs.push_back(scan_dir);
}

void DuplicateSearcher::addExcludeScanDir(const std::string& scan_exclude_dir)
{
	m_ExludeScanDirs.push_back(scan_exclude_dir);
}

void DuplicateSearcher::setScanLevel(const size_t scan_level)
{
	m_ScanLevel = scan_level;
}

void DuplicateSearcher::setMinFileSize(const size_t min_file_size)
{
	m_MinFileSize = min_file_size;
}

bool DuplicateSearcher::setParamsFromCmdLineArgs(int argc, const char* argv[])
{
	try {
		fillParamsFromCmdLineArgs(argc, argv);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return false;
	}
	return true;
}

bool DuplicateSearcher::fillParamsFromCmdLineArgs(int argc, const char* argv[])
{
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
				 this->addExcludeScanDir(dir);
			}), "Directories to exclude from scanning (there may be several)")

			("scan-level,l",  po::value<size_t>()->default_value(0)->notifier(
				 [this](const size_t scan_level)
			{
			 this->setScanLevel(scan_level);
			}), "Scan level (one for all directories, 0 - only the specified directory)")

			("min-file-size,s",  po::value<size_t>()->default_value(1)->notifier(
				 [this](const size_t min_file_size)
			{
			 this->setMinFileSize(min_file_size);
			}), "Min file size, by default, all files larger than 1 byte are checked")

			("file-masks,m", "Masks of file names allowed for comparison (case-independent)")

			("file-block-size,b",  po::value<size_t>()->default_value(5)->notifier(
				 [this](const size_t file_block_size)
			{
			 this->setFileBlockSize(file_block_size);
			}), "Size of the block that reads files")

			("hash-algorithm,a", po::value<std::string>()->default_value("md5")->notifier(
				 [this](const std::string& str_file_comparator)
			{
			 this->setupFileComparator(str_file_comparator);
			}), "One of the available hashing algorithms (crc32, md5)");

	po::variables_map variables_map;
	po::store(parse_command_line(argc, argv, description), variables_map);
	po::notify(variables_map);

	if (variables_map.count("help"))
	{
		std::cout << description << '\n';
		return false;
	}
	return true;
}

std::vector<std::vector<fs::path> > DuplicateSearcher::duplicateFiles() const
{
	return m_DuplicateFiles;
}
