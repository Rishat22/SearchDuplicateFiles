# Using Boost libraries, structures and algorithms have developed a utility for detecting duplicate files.
### The utility allows you to specify parameters via the command line:
* directories to scan (there may be several)
* directories to exclude from scanning (there may be several)
* scan level (one for all directories, 0 - only the specified
directory without nested ones)
* minimum file size, by default, all files
larger than 1 byte are checked.
* masks of file names allowed for comparison (
case-independent)
* the size of the block that reads files, this
size is referred to as `S` in the task
* one of the available hashing algorithms (crc32, md5 -
determine the specific options yourself), in the task
this function is referred to as `H`
