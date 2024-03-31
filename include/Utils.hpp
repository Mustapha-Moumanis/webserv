#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <map>

bool isInformational(int code);
bool isSuccessful(int code);
bool isRedirection(int code);
bool isClientError(int code);
bool isServerError(int code);
bool isError(int code);
bool isVadilCode(int code);
const char * reasonPhrase(int code);

bool isDir(std::string path);
bool isRegFile(std::string path);
std::string getNewName();

#endif