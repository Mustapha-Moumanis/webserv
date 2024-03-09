#include <iostream>
#include <sys/stat.h>

bool isDir(std::string path) {
	struct stat s;

	if (stat(path.c_str(), &s) != 0)
		return 0;
	return S_ISDIR(s.st_mode);
}