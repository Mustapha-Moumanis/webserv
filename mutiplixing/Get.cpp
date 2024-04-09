#include "Request.hpp"

// bool Request::hasIndexFile(std::string url) { // remove
// 	struct dirent* directoryEntries;
	
// 	DIR* dir = opendir(url.c_str());
// 	if (!dir)
// 		throw StatusCodeExcept(403);
// 	while ((directoryEntries = readdir(dir))){
// 		std::string name = directoryEntries->d_name;
// 		if (name != "." && name != "..") {
// 			if (name.substr(0, name.find('.')) == "index") {
// 				url += name;
// 				std::cout << ">> " << url << std::endl;
// 				closedir(dir);
// 				return 1;
// 			}
// 		}
// 	}
// 	closedir(dir);
// 	return 0;
// }

std::string Request::genGetFileHeader(int code, std::string url) {
	std::stringstream ss;
    std::string header;
	std::string sCode;

	ss << code;
	ss >> sCode;

	std::string type = "html";
	std::string mimeType;
	
	size_t pos = url.find_last_of(".");
	if (pos != std::string::npos)
		type = url.substr(pos + 1);
	if (!MimeTypes::getType(type).empty())
		mimeType = MimeTypes::getType(type);
	else
		mimeType = "text/html";

    header = "HTTP/1.1 " + sCode + " " + reasonPhrase(code) + "\r\n";
    header += "Content-Type: " + mimeType + "\r\n\r\n";
    return header;
}

std::string Request::genGetDirHeader(int code, std::string mimeType) {
	std::stringstream ss;
    std::string header;
	std::string sCode;

	ss << code;
	ss >> sCode;
    header = "HTTP/1.1 " + sCode + " " + reasonPhrase(code) + "\r\n";
    header += "Content-Type: " + mimeType + "\r\n\r\n";
    return header;
}

void Request::isDirHasIndexFile() {
	std::vector<std::string> index = location->getIndex();
	std::string token;
	DIR* dir = opendir(url.c_str());
	if (!dir)
		throw StatusCodeExcept(403);

	for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
		token = url + *it;
		if (isDir(token))
			throw rediractionExcept(302, reqURL + *it + "/");

		if (access(token.c_str(), R_OK) != 0)
			continue ;
		size_t found = token.find_last_of(".");
		if (found != std::string::npos){
			std::string tmp = token.substr(found + 1);
			std::string extention = "." + tmp;
			std::map<std::string, std::string> cgiPath = location->getCgiPaths();
			std::map<std::string, std::string>::iterator it = cgiPath.find(extention);
			if (it != cgiPath.end())
				cgiGet(it->second, token);
			if (MimeTypes::getType(tmp).empty())
				throw StatusCodeExcept(415);
		}

		throw responseGetExcept(genGetFileHeader(200, token), token, _FILE, 0);
	}
}

std::string Request::genDirItem(std::string name) {
	std::string token;
	std::string item;

	if (name == "." || (name == ".." && parentDir == 0))
		return "";
	
	token = url + name;

	item = "<li class=\"collection-item\">\n<a href=\"";
	item += reqURL + name;
	item += "\">\n";
	if (isDir(token))
		item += FOLDERICON;
	else
		item += FILEICON;
	item += "<h6 class = \"name\">";

	if (name == "..") {
		name = "[parent directory]";
		parentDir = 0;
	}

	item += name;

	item += "</h6>\n</a>\n</li>\n";
	return item;
}

void Request::generateDirAutoIndex() {
    std::string body;

	body = genGetFileHeader(200, url);
    body += "<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='UTF-8'>\n<title>index of ";
    body += reqURL;
    body += "</title>\n<style>body{font-family:Arial,sans-serif;background:#f5f5f5}a{text-decoration:none;color:#333;display:flex;align-items:center}";
    body += "h3{color:#599ac2;font-size:32px;line-height: 1.5;margin:0;padding:0 2.5rem;background:#d2edf7bd;font-weight:600;letter-spacing:1px}";
    body += ".collection{padding:0}.collection-item{list-style-type:none;line-height:1.5rem;padding:10px 20px;margin:0;border-bottom:1px solid #e0e0e0}";
    body += ".directory-icon{width:30px;min-width: 30px;margin-right:10px}.name{margin:0;font-size:16px}</style>";
    body += "</head>\n<body>\n<h3>index of ";
	body += reqURL;
	body += "</h3>";
	if (url != server->getRoot() + "/") {
		parentDir = 1;
		body += genDirItem("..");
	}
	throw responseGetExcept(body, url, _FOLDER, 0);
}

void Request::Get(){
	if (isDir(url)) {
		// rediraction
		if (location->getRediractionStatusCode() != 0)
			throw rediractionExcept(location->getRediractionStatusCode(), location->getRediractionURL());
		if (!reqURL.empty() && reqURL[reqURL.size() - 1] != '/')
			throw rediractionExcept(302, reqURL + "/" + queryString);
		
		// get index from configfile
		isDirHasIndexFile();

		if (location->getAutoIndex() == "on") {
			std::cout << "GET : is Dir" << std::endl;
			// if (hasIndexFile(url)) // get index insind folder 
			// 	throw StatusCodeExcept(200);
			// else // list folders
			generateDirAutoIndex();
		}
		throw StatusCodeExcept(403); // Forbidden

	}
	else if (isRegFile(url)) {
		std::cout << "GET : is reg file" << std::endl;
		if (access(url.c_str(), R_OK) != 0)
			throw StatusCodeExcept(403);

		size_t found = url.find_last_of(".");
		if (found != std::string::npos){
			std::string tmp = url.substr(found + 1);
			std::string extention = "." + tmp;
			std::map<std::string, std::string> cgiPath = location->getCgiPaths();
			std::map<std::string, std::string>::iterator it = cgiPath.find(extention);
			if (it != cgiPath.end())
				cgiGet(it->second, url);
			if (MimeTypes::getType(tmp).empty())
				throw StatusCodeExcept(415);
		}
		throw responseGetExcept(genGetFileHeader(200, url), url, _FILE, 0);
	}
	else
		throw StatusCodeExcept(404);
}