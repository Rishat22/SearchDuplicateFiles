#include <boost/program_options.hpp>
#include <iostream>
#include "duplicate_searcher.h"

namespace po = boost::program_options;

void set_bulk(std::vector<size_t> bulk) {
	for(const auto bul : bulk)
		std::cout << "bulk size is " << bul << std::endl;
}

int main(int argc, const char *argv[])
{
	DuplicateSearcher duplicate_searcher;
	duplicate_searcher.setParamsFromCmdLineArgs(argc, argv);
	duplicate_searcher.scan();
}
