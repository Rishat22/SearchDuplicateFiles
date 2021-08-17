#include <iostream>
#include "duplicate_searcher.h"

int main(int argc, const char *argv[])
{
	DuplicateSearcher duplicate_searcher;
	if(!duplicate_searcher.setParamsFromCmdLineArgs(argc, argv))
		return 0;
	if(!duplicate_searcher.scan())
		return 1;
	for(const auto& duplicate_files : duplicate_searcher.duplicateFiles())
	{
		for(const auto& duplicate_file : duplicate_files)
			std::cout << duplicate_file.c_str() << std::endl;
		std::cout << std::endl;
	}
}
