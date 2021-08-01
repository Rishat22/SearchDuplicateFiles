#include <iostream>
#include "duplicate_searcher.h"
#include "FileComparators/crc32_file_comparator.h"


int main(int argc, const char *argv[])
{
	DuplicateSearcher duplicate_searcher;
	duplicate_searcher.setParamsFromCmdLineArgs(argc, argv);
//	if(!duplicate_searcher.scan())
//		return 1;
//	for(const auto& duplicate_files : duplicate_searcher.duplicateFiles())
//	{
//		for(const auto& duplicate_file : duplicate_files)
//			std::cout << duplicate_file << std::endl;
//		std::cout << std::endl;
//	}
	CRC32FileComparator file_comparator;
	file_comparator.operator()("/home/user-az/projects/otherProjects/OTUS/SearchDuplicateFiles/testFolder/HEAD",
							   "/home/user-az/projects/otherProjects/OTUS/SearchDuplicateFiles/testFolder/HEAD");
}
