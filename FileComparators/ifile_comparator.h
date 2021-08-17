#ifndef FILECOMPARATOR_H
#define FILECOMPARATOR_H
#include "boost/filesystem.hpp"

namespace fs = boost::filesystem;

template <typename HASH>
void getFileHash(HASH& file_hash, const size_t file_block_size, const boost::filesystem::path& file_path)
{
	std::string line;
	std::ifstream myfile (file_path.c_str());
	if (myfile.is_open())
	{
		auto buffer = new char [file_block_size + 1];
		while ( myfile.good() )
		{
			myfile.read(buffer, file_block_size);
			buffer[file_block_size] = '\0';
			file_hash.insert(buffer);
		}
		delete[] buffer;
		myfile.close();
	}
}

class IFileComparator
{
public:
    virtual bool operator()(const fs::path& first_file,
                    const fs::path& second_file) = 0;
    void setFileBlockSize(const size_t file_block_size)
    {
        m_FileBlockSize = file_block_size;
    }

protected:
    size_t m_FileBlockSize;
};
#endif // FILECOMPARATOR_H
