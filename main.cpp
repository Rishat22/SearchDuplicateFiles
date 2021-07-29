#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

void set_bulk(size_t bulk) {
	std::cout << "bulk size is " << bulk << std::endl;
}

int main(int argc, const char *argv[]) {

	try {
		po::options_description description{"Options"};
		description.add_options()
				("help,h", "Getting more information about command line parameters")
				("dirs,d", po::value<std::string>()->default_value("./"), "Directories to scan (there may be several)")
				("exclude-dirs,e", "Directories to exclude from scanning (there may be several)")
				("scan-level,l",  po::value<size_t>()->default_value(0), "Scan level (one for all directories, 0 - only the specified directory)")
				("min-file-size,s",  po::value<size_t>()->default_value(1), "Min file size, by default, all files larger than 1 byte are checked")
				("file-masks,m", "Masks of file names allowed for comparison (case-independent)")
				("file-block-size,b",  po::value<size_t>()->default_value(5), "Size of the block that reads files")
				("hash-algorithm,a", po::value<std::string>()->default_value("md5"), "One of the available hashing algorithms (crc32, md5)")
				("bulk", po::value<size_t>()->default_value(5)->notifier(set_bulk), "bulk size");

		po::variables_map variables_map;
		po::store(parse_command_line(argc, argv, description), variables_map);
		po::notify(variables_map);

		if (variables_map.count("help"))
			std::cout << description << '\n';
		else if (variables_map.count("dirs"))
			std::cout << "readfrom: " << variables_map["dirs"].as<std::string>() << std::endl;
		else if (variables_map.count("bulk"))
			std::cout << "bulk: " << variables_map["bulk"].as<size_t>() << std::endl;
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}
