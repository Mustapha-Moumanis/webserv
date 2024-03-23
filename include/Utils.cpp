#include "Utils.hpp"

bool isDir(std::string path) {
	struct stat s;

	if (stat(path.c_str(), &s) != 0)
		return 0;
	return S_ISDIR(s.st_mode);
}

bool isRegFile(std::string path) {
	struct stat s;

	if (stat(path.c_str(), &s) != 0)
		return 0;
	return S_ISREG(s.st_mode);
}

std::string getNewName()
{
    std::string name;
    char c;

    std::ifstream randomFile("/dev/random", std::ios::binary);
    if (!randomFile.is_open())
        return "";
    
    while (name.length() < 8)
    {
        randomFile.read(&c, 1);
        if (randomFile.fail())
            return "";
        if (std::isalpha(c))
            name += c;
    }
    randomFile.close();
    return name;
}