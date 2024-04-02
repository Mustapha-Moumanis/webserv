#include "Request.hpp"

bool Request::hasIndexFile(std::string url) { // remove
	struct dirent* directoryEntries;
	
	DIR* dir = opendir(url.c_str());
	if (!dir)
		throw StatusCodeExcept(403);
	while ((directoryEntries = readdir(dir))){
		std::string name = directoryEntries->d_name;
		if (name != "." && name != "..") {
			if (name.substr(0, name.find('.')) == "index") {
				url += name;
				std::cout << ">> " << url << std::endl;
				closedir(dir);
				return 1;
			}
		}
	}
	closedir(dir);
	return 0;
}

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
	// std::ifstream ifs;
	std::vector<std::string> index = location->getIndex();
	std::string token;
	DIR* dir = opendir(url.c_str());
	if (!dir)
		throw StatusCodeExcept(403);

	for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
		token = url + *it;

		if (!isRegFile(token)) {
			continue ;
		}
		if (access(token.c_str(), R_OK) != 0)
			throw StatusCodeExcept(403); // Forbidden
		// ifs.open(token.c_str());
		// if (!ifs.is_open())

		// ifs.close();
		std::cout << "GET : Has Index File" << std::endl;
		////
		// std::string type = "html";
		// std::string mimeType;
		
		// std::cout << "is file\n";
		
		// size_t pos = reqURL.find_last_of(".");
		// if (pos != std::string::npos)
		// 	type = reqURL.substr(pos + 1);
		// if (!MimeTypes::getType(type).empty())
		// 	mimeType = MimeTypes::getType(type);
		// else
		// 	mimeType = "text/html";
		// std::string header = "HTTP/1.1 200 OK\r\n";
		// header += "Content-Type: " + mimeType + "\r\n\r\n";

		throw responseGetExcept(genGetFileHeader(200, reqURL), reqURL, 1);
	}
}

void Request::generateDirAutoIndex() {
    std::string body;
	std::string name;
    std::string token;
    struct dirent* directoryEntries;
	DIR* dir = opendir(url.c_str());

    body += "<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='UTF-8'>\n<title>index of ";
    body += url;
    body += "</title>\n<style>body{font-family:Arial,sans-serif;background:#f5f5f5}a{text-decoration:none;color:#333;display:flex;align-items:center}";
    body += "h3{color:#599ac2;font-size:32px;line-height: 1.5;margin:0;padding:0 2.5rem;background:#d2edf7bd;font-weight:600;letter-spacing:1px}";
    body += ".collection{padding:0}.collection-item{list-style-type:none;line-height:1.5rem;padding:10px 20px;margin:0;border-bottom:1px solid #e0e0e0}";
    body += ".directory-icon{width:30px;min-width: 30px;margin-right:10px}.name{margin:0;font-size:16px}</style>";
    body += "</head>\n<body>\n<h3>index of ";
	body += reqURL;
	body += "</h3>";
    while ((directoryEntries = readdir(dir))){
        name = directoryEntries->d_name;
		if (name == "." || (name == ".." && reqURL == "/"))
			continue ;
        token = url + name;
        body += "<li class=\"collection-item\">\n<a href=\"";
        body += reqURL + name;
        body += "\">\n";
        if (isDir(token)) {
            body += "<svg class=\"directory-icon\" viewBox=\"0 0 1024 1024\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\"><path d=\"M182.9 694.4h-64.1c-4.4 0-6.6 4.4-6.6 6.6 0 4.4 4.4 6.6 8.8 6.6h64.1c2.2 0 4.4 0 4.4-2.2 2.2-2.2 2.2-4.4 2.2-6.6-2.2 0-4.4-4.4-8.8-4.4zM426 694.4H231.5c-4.4 0-6.6 4.4-6.6 6.6 0 4.4 4.4 6.6 6.6 6.6H426c2.2 0 4.4 0 4.4-2.2 2.2-2.2 2.2-4.4 2.2-6.6 0.1 0-2.1-4.4-6.6-4.4z\" fill=\"#FFFFFF\" /><path d=\"M837.1 188.2H463.6l-46.4-61.9c-4.4-4.4-8.8-8.8-15.5-8.8H189.5C123.2 117.5 68 170.5 68 239v459.7c0 66.3 53 121.6 121.6 121.6h649.8c66.3 0 121.6-53 121.6-121.6v-389c-2.3-66.2-57.5-121.5-123.9-121.5z\" fill=\"#429BCF\" /><path d=\"M114.4 208.1h775.8v68.5H114.4z\" fill=\"#FFFFFF\" /><path d=\"M861.5 254.5H461.4l-48.6-66.3c-4.4-4.4-11.1-8.8-17.7-8.8H165.2c-70.7 0-130.4 57.5-130.4 130.4v495.1c0 70.7 57.5 130.4 130.4 130.4h696.2c70.7 0 130.4-57.5 130.4-130.4v-420c0.1-72.9-59.6-130.4-130.3-130.4z\" fill=\"#83C6EF\" /><path d=\"M990.1 364.2c-10.1-62.8-64.7-109.7-128.6-109.7H461.4l-48.6-66.3c-4.4-4.4-11.1-8.8-17.7-8.8H165.2c-70.7 0-130.4 57.5-130.4 130.4v54.4h955.3z\" fill=\"#D2EDF7\" /><path d=\"M990.1 364.2c-10.1-62.8-64.7-109.7-128.6-109.7H461.4l-48.6-66.3c-4.4-4.4-11.1-8.8-17.7-8.8H165.2c-70.7 0-130.4 57.5-130.4 130.4v54.4h955.3z\" fill=\"#D2EDF7\" /></svg>\n";
        }
        else {
            body += "<svg class=\"directory-icon\" viewBox=\"0 0 1024 1024\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\"><path d=\"M656.2 61.7H174.9v948h694V261.9H656.2z\" fill=\"#83C6EF\" /><path d=\"M444 72.5c-3.8 0-7.4-2.2-9-5.9-2.6-6.1-6.2-11.6-10.6-16.4-3.7-4-3.5-10.1 0.5-13.8 3.9-3.7 10.1-3.5 13.8 0.5 6 6.5 10.8 13.9 14.3 22 2.1 5-0.2 10.7-5.2 12.8-1.2 0.5-2.5 0.8-3.8 0.8z\" fill=\"#FDC223\" /><path d=\"M382.9 361.5c-42 0-76.1-34.2-76.1-76.1V88.5c0-42 34.2-76.1 76.1-76.1 21.1 0 41.4 8.9 55.9 24.5 3.7 4 3.4 10.1-0.6 13.8-3.9 3.7-10.1 3.4-13.8-0.5-10.7-11.6-25.8-18.3-41.5-18.3-31.2 0-56.6 25.4-56.6 56.6v196.9c0 31.2 25.4 56.6 56.6 56.6 10.7 0 21.2-3 30.2-8.8 4.5-2.9 10.6-1.6 13.5 3 2.9 4.6 1.5 10.6-3 13.5-12.2 7.7-26.3 11.8-40.7 11.8z\" fill=\"#FDC223\" /><path d=\"M868.9 257.8H656.2V61.7z\" fill=\"#2D416C\" /><path d=\"M302 442.9h219.9v41.7H302zM302 538.9h285.3v41.7H302zM302 625.3h219.9V667H302zM302 711.6h361.6v41.7H302zM302 787.3h361.6V829H302z\" fill=\"#FFFFFF\" /><path d=\"M423.6 349.7l-10.5-16.5c16.5-10.5 26.4-28.4 26.4-47.8v-73.3H459v73.3c0 26.1-13.2 50.1-35.4 64.3z\" fill=\"#FDC223\" /><path d=\"M386.4 267.7h-19.5V119c0-25.4 20.7-46.1 46.1-46.1 25.4 0 46.1 20.7 46.1 46.1v121.4h-19.5V119c0-14.6-11.9-26.5-26.5-26.5s-26.5 11.9-26.5 26.5v148.7z\" fill=\"#FDC223\" /><path d=\"M306.8 285.4c0 42 34.2 76.2 76.1 76.2 14.4 0 28.5-4.1 40.7-11.9 4.6-2.9 5.9-9 3-13.5-2.9-4.6-9-5.9-13.5-3-9 5.8-19.5 8.8-30.2 8.8-31.2 0-56.6-25.4-56.6-56.6V88.5c0-9.7 2.7-18.8 7.1-26.8h-21.5c-3.2 8.4-5.1 17.3-5.1 26.8v196.9z\" fill=\"#FDC223\" /><path d=\"M174.9 974.5h694v35.2h-694z\" fill=\"#429BCF\" /></svg>";
        }
        body += "<h6 class = \"name\">";
		
		if (name == "..")
			name = "[parent directory]";
        body += name;
        body += "</h6>\n</a>\n</li>\n";
    }
    body += "</body>\n</html>";
    closedir(dir);
	throw responseGetExcept(genGetDirHeader(200, "text/html"), body, 0);
}

void Request::Get(){
	if (isDir(url)) {
		// rediraction
		if (location->getRediractionStatusCode() != 0)
			throw rediractionExcept(location->getRediractionStatusCode(), location->getRediractionURL());
		if (!reqURL.empty() && reqURL[reqURL.size() - 1] != '/')
			throw rediractionExcept(301, reqURL + "/" + queryString);
		
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
		if (found != std::string::npos) {
			std::string tmp = url.substr(found + 1);
			if (MimeTypes::getType(tmp).empty())
				throw StatusCodeExcept(415);
		}
		throw responseGetExcept(genGetFileHeader(200, url), url, 1);

		// throw responseGetExcept(200, url, 1);
	}
	else
		throw StatusCodeExcept(404);
}