#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sys/stat.h>
#include <fstream>

bool isDir(std::string path);
bool isRegFile(std::string path);
std::string getNewName();

#endif