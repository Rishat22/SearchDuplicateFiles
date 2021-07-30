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
		for(const auto& scan_dir : m_ScanDirs)
		{
			FindDuplicate(scan_dir);
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

void DuplicateSearcher::FindDuplicate( const fs::path& dir_path )
{
	fs::directory_iterator end_itr;
	for( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
	{
	  if( is_directory(itr->status()) )
	  {
		FindDuplicate( itr->path() );
	  }
	  else
	  {
		  std::vector<fs::path> duplicate_files;
		  FindDuplicate( dir_path, itr->path(), duplicate_files );
		  if( !duplicate_files.empty() )
		  {
			  duplicate_files.push_back(itr->path());
			  m_DuplicateFiles.emplace_back(duplicate_files);
		  }
	  }
	}
}
void DuplicateSearcher::FindDuplicate( const fs::path& dir_path,
									   const fs::path& search_file_path, std::vector<fs::path>& duplicate_files)
{
  if( !exists( dir_path ) )
	  return;
  const auto search_file_size = fs::file_size( search_file_path );
  fs::directory_iterator end_itr;
  for( fs::directory_iterator itr( dir_path ); itr != end_itr; ++itr )
  {
	if( is_directory(itr->status()) )
	{
	  FindDuplicate( itr->path(), search_file_path, duplicate_files );
	}
	else if( fs::file_size( itr->path() ) == search_file_size && itr->path() != search_file_path)
	{
		duplicate_files.push_back(itr->path());
	}
  }
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
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

std::vector<std::vector<fs::path> > DuplicateSearcher::duplicateFiles() const
{
	return m_DuplicateFiles;
}
