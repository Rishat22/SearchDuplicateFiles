#include <iostream>
#include <boost/program_options.hpp>
#include "FileComparators/crc32_file_comparator.h"
#include "FileComparators/md5_file_comparator.h"
#include "duplicate_search_exceptions.h"
#include "duplicate_searcher.h"

namespace po = boost::program_options;

DuplicateSearcher::DuplicateSearcher()
	: m_FileComparator(nullptr)
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
				if(file.second == false)
				{
					file.second = true;
					std::vector<fs::path> duplicate_files;
					findDuplicate( scan_dir, file.first, duplicate_files );
					if( !duplicate_files.empty() )
					{
						duplicate_files.push_back(file.first);
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

void DuplicateSearcher::getAllScanFiles(const fs::path & dir_path, std::unordered_map<std::string, bool>& all_scan_files)
{
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
		  all_scan_files[itr->path().c_str()] = false;
	  }
	}
}


void DuplicateSearcher::findDuplicate( const fs::path& dir_path,
									   const fs::path& search_file_path, std::vector<fs::path>& duplicate_files)
{
	if( !exists( dir_path ) )
		return;
	size_t curr_scan_level = 0;
	fs::directory_iterator end_itr;
	for( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
		const auto& cur_path = itr->path();
		if( is_directory(itr->status()) )
		{
			if(curr_scan_level++ >= m_ScanLevel)
				continue;
			if(std::find(m_ExludeScanDirs.begin(), m_ExludeScanDirs.end(), itr->path()) != m_ExludeScanDirs.end())
				continue;
			findDuplicate( cur_path, search_file_path, duplicate_files );
		}
		else
		{
			const auto cur_scan_file = m_AllScanFiles.find(itr->path().c_str());
			if( cur_scan_file == m_AllScanFiles.end() || cur_scan_file->second == true)
				continue;
			if( fs::file_size( itr->path() ) <= m_MinFileSize )
				continue;
			if( m_FileComparator->operator()( search_file_path, itr->path() ) )
			{
				duplicate_files.push_back(itr->path());
				cur_scan_file->second = true;
			}
		}
	}
}

void DuplicateSearcher::setFileComparator(const std::string& str_file_comparator)
{
	static std::map<std::string, IFileComparator*> file_comparators
	{
		{ "md5", new MD5FileComparator},
		{ "crc32", new CRC32FileComparator}
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
				 this->setFileComparator(str_file_comparator);
				}), "One of the available hashing algorithms (crc32, md5)");

		po::variables_map variables_map;
		po::store(parse_command_line(argc, argv, description), variables_map);
		po::notify(variables_map);

		if (variables_map.count("help"))
			std::cout << description << '\n';
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

std::vector<std::vector<fs::path> > DuplicateSearcher::duplicateFiles() const
{
	return m_DuplicateFiles;
}
