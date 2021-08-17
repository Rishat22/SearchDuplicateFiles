#include <iostream>
#include <fstream>
#include <unordered_set>
#include "crc32_file_comparator.h"

using FileHash = std::unordered_set<std::string, CRC32HashFunction>;

bool CRC32FileComparator::operator()(const boost::filesystem::path& first_file, const boost::filesystem::path& second_file)
{
	if ( fs::file_size( first_file ) == fs::file_size( second_file ) )
	{
		FileHash first_file_hash;
		getFileHash<FileHash>(first_file_hash, m_FileBlockSize, first_file);
		FileHash second_file_hash;
		getFileHash<FileHash>(second_file_hash, m_FileBlockSize, first_file);

		return first_file_hash == second_file_hash;
	}
	return false;
}


size_t CRC32HashFunction::operator()(const std::string& str) const
{
	boost::crc_32_type crc32;
	crc32 = std::for_each(str.begin(), str.end(), crc32);
	return crc32();
}
