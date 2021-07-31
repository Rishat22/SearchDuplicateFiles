#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <boost/algorithm/hex.hpp>
#include "md5_file_comparator.h"

using FileHash = std::unordered_set<std::string, MD5HashFunction>;

void getFileHash(FileHash& file_hash, const boost::filesystem::path& file_path)
{
	std::string line;
	std::ifstream myfile (file_path.c_str());
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			file_hash.insert(line);
		}
		myfile.close();
	}
}

bool MD5FileComparator::operator()(const boost::filesystem::path& first_file, const boost::filesystem::path& second_file)
{
	if ( fs::file_size( first_file ) == fs::file_size( second_file ) )
	{
		FileHash first_file_hash;
		getFileHash(first_file_hash, first_file);
		FileHash second_file_hash;
		getFileHash(second_file_hash, first_file);

		return first_file_hash == second_file_hash;
	}
	return false;
}

size_t MD5HashFunction::operator()(const std::string& str) const
{
	md5 hash;
	md5::digest_type digest;
	hash.process_bytes(str.data(), str.size());
	hash.get_digest(digest);
	const size_t hash_result = toInt(digest);
	return hash_result;
}

size_t MD5HashFunction::toInt(const md5::digest_type &digest) const
{
	const auto char_digest = reinterpret_cast<const char *>(&digest);
	std::string hex_result;
	boost::algorithm::hex(char_digest, char_digest + sizeof(md5::digest_type), std::back_inserter(hex_result));
	std::stringstream ss;
	ss << std::hex << hex_result;
	size_t result;
	ss >> result;
	return result;
}
