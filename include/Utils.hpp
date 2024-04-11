#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <map>

#define _FILE 1
#define _FOLDER 2
#define FOLDERICON "<svg class=\"directory-icon\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\"><path fill=\"#74C0FC\" d=\"M64 480H448c35.3 0 64-28.7 64-64V160c0-35.3-28.7-64-64-64H288c-10.1 0-19.6-4.7-25.6-12.8L243.2 57.6C231.1 41.5 212.1 32 192 32H64C28.7 32 0 60.7 0 96V416c0 35.3 28.7 64 64 64z\"/></svg>"
#define FILEICON "<svg class=\"directory-icon\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 384 512\"><path fill=\"#74C0FC\" d=\"M0 64C0 28.7 28.7 0 64 0H224V128c0 17.7 14.3 32 32 32H384V448c0 35.3-28.7 64-64 64H64c-35.3 0-64-28.7-64-64V64zm384 64H256V0L384 128z\"/></svg>"
#define REPLYICON "<svg class=\"directory-icon\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\"><path fill=\"#74C0FC\" d=\"M205 34.8c11.5 5.1 19 16.6 19 29.2v64H336c97.2 0 176 78.8 176 176c0 113.3-81.5 163.9-100.2 174.1c-2.5 1.4-5.3 1.9-8.1 1.9c-10.9 0-19.7-8.9-19.7-19.7c0-7.5 4.3-14.4 9.8-19.5c9.4-8.8 22.2-26.4 22.2-56.7c0-53-43-96-96-96H224v64c0 12.6-7.4 24.1-19 29.2s-25 3-34.4-5.4l-160-144C3.9 225.7 0 217.1 0 208s3.9-17.7 10.6-23.8l160-144c9.4-8.5 22.9-10.6 34.4-5.4z\"/></svg>"

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