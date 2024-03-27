
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:36:28 by shilal            #+#    #+#             */
/*   Updated: 2024/03/23 23:25:43 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cstring>

Request::Request() : body(""), queryString(""), url(""), Method(""), length(0){
	nextchunk = "";
	type = "";
	path = "";
	ContentLength = 0;
	HeaderIsDone = 0;
	IsChunked = 0;
}

Request::~Request() {
	ftype.close();
}

void Request::setServ(Server &serv) {
	this->server = &serv;
}

void Request::setDoublicateServer(std::vector<Server *> &vec) {
    doublicateServer = vec;
}

void Request::CheckFirstLine(std::string Fline){

	std::stringstream ss(Fline);
	std::string version;
	std::string a;
	std::string b;
	ss >> a >> b >> version;

    if (b.length() > 2048)
		throw StatusCodeExcept(414);
	if ((a != "GET" && a != "DELETE" && a != "POST") )
		throw StatusCodeExcept(501);
	Method = a;
	url = b;
	HeadReq.insert(std::pair<std::string,std::string>("Method", a)); // its will be removed
	HeadReq.insert(std::pair<std::string,std::string>("Location",b)); // its will be removed

	if (version != "HTTP/1.1")
		throw StatusCodeExcept(505);
}

void Request::CheckRequest(){

	std::map<std::string, std::string>::iterator it;
	if (Method == "POST"){
		path = this->location->getUploadPath();
		// check if path end with "/" or not |!|
		if (this->location->getUpload() == "off" || path.empty())
			throw StatusCodeExcept(403);
		
		if ((it = HeadReq.find("Content-Length")) != HeadReq.end()){
			if (it->second.find_first_not_of("0123456789") != std::string::npos)
				throw StatusCodeExcept(400);
			ContentLength = atol(it->second.c_str());
			if (ContentLength > server->getClientMaxBodySize())
				throw StatusCodeExcept(413);
		}
		else if ((it = HeadReq.find("Transfer-Encoding")) != HeadReq.end()){
			if (it->second != "chunked")
				throw StatusCodeExcept(501);
		}
		else
			throw StatusCodeExcept(400);

	}

	it = HeadReq.find("Content-Type");
	if (it != HeadReq.end()){
		type = ".";
		if (MimeTypes::getExtension(it->second.c_str()) == NULL)
			throw StatusCodeExcept(415);
		type += MimeTypes::getExtension(it->second.c_str());
	}
}

bool Request::CompareURL(std::string s1, std::string s2) {
	size_t len1 = s1.length();
	size_t len2 = s2.length();

	if (len2 > len1)
		len2 = len1;
	for (size_t i = len2; i > 0; --i)
		if (s2.substr(0, i) == s1)
			return i;
    return 0;
}

void Request::specificServ() {
	std::string Host = HeadReq["Host"];
	size_t pos = Host.find(":");
	if (pos == std::string::npos)
		throw StatusCodeExcept(400);
	std::string port = Host.substr(pos + 1);
	if (port.length() > 5 || port.find_first_not_of("0123456789") != std::string::npos 
		|| atoi(port.c_str()) != server->getPort())
		throw StatusCodeExcept(400);
	Host = Host.substr(0, pos);

	if (doublicateServer.size() > 1) {
		std::vector<Server *>::iterator it = doublicateServer.begin();
		for (; it != doublicateServer.end(); it++) {
			std::vector <std::string> serv_names = (*it)->getServNames();
		
			if (find(serv_names.begin(), serv_names.end(), Host) != serv_names.end()) {
				server = *it;
				break;
			}
		}
	}
	// server->printArg();
}

void Request::matchingURL(std::string url) {
	std::string res;
	std::string root = server->getRoot();

	size_t pos = url.find("?");
	if (pos != std::string::npos) {
		queryString = url.substr(pos);
		url = url.substr(0, pos);
	}
	reqURL = url;
	for (std::vector<Location>::iterator it1 = server->getLocation().begin(); it1 != server->getLocation().end(); it1++) {
		if (CompareURL(it1->getPath(), url)) {
			if (it1->getPath().length() > res.length()) {
				res = it1->getPath();
				location = &(*it1);
			}
		}
	}
	if (!res.empty()) {
		if (!root.empty() && root[root.length() - 1] == '/')
			root = root.substr(0, root.length() - 1);

		if (res == "/") {
			this->url = url.replace(0, 1, root.append("/"));
			return ;
		}
		if (!res.empty() && res[res.length() - 1] == '/')
			res = res.substr(0, res.length() - 1);

		pos = url.find("/", res.length());
		if (pos != std::string::npos)
			res = url.substr(0, pos);
		else
			res = url;

		this->url = url.replace(0, res.length(), root);
	}
	else
		throw StatusCodeExcept(404);
}

void Request::setRequest(std::string req) {

    if (HeaderIsDone == 0){
		CheckFirstLine(req.substr(0, req.find("\r\n")));
		req.erase(0, req.find("\r\n") + 2);
		while (404) {
			std::string key = req.substr(0, req.find(": "));
			if (req.substr(0, req.find("\r\n")) == ""){
				req.erase(0, req.find("\r\n") + 2);
				// if no "\r\n\r\n" should be timeout ==> |!|
				break;
			}
			req.erase(0, req.find(": ") + 2);
			std::string val =  req.substr(0, req.find("\r\n"));
			HeadReq.insert(std::pair<std::string,std::string>(key, val));
			req.erase(0, req.find("\r\n") + 2);
		}
        body = req;
        HeaderIsDone = 1;
		specificServ();
		matchingURL(url);
		if (this->location->getRediractionStatusCode() != 0) {
			throw rediractionExcept(this->location->getRediractionStatusCode(), this->location->getRediractionURL());
		}
	
		if (this->location->getMethods().find(Method) == std::string::npos)
			throw StatusCodeExcept(405);
        CheckRequest();
	}
	if (Method == "POST"){
		Post(req);
	}
	else if (Method == "GET")
		Get();
	else
		Delete();
}

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

void Request::isDirHasIndexFile() {
	std::ifstream ifs;
	std::vector<std::string> index = location->getIndex();
	std::string token;
	for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
		token = url + *it;
		std::cout << token << std::endl;

		if (!isRegFile(token)) {
			continue ;
		}
		ifs.open(token.c_str());
		if (!ifs.is_open()) {
			continue ;
		}
		ifs.close();
		throw StatusCodeExcept(200);
	}
}

void Request::Get(){
	if (isDir(url)){
		std::cout << "is Dir" << std::endl;
		// rediraction
		if (location->getRediractionStatusCode() != 0)
			throw rediractionExcept(location->getRediractionStatusCode(), location->getRediractionURL());
		if (!reqURL.empty() && reqURL[reqURL.size() - 1] != '/')
			throw rediractionExcept(301, reqURL + "/" + queryString);
		
		// get index from configfile
		isDirHasIndexFile();

		// std::string index = getAvailableIndex(url);
		// if (!index.empty()) {
		// 	url += index;
		// 	std::cout << url << std::endl;
		// 	throw StatusCodeExcept(200);
		// }
		if (location->getAutoIndex() == "on") {
			// if (hasIndexFile(url)) // get index insind folder 
			// 	throw StatusCodeExcept(200);
			// else // list folders
			// generateDirAutoIndex();
			throw responseGetExcept(200, url, 0);
		}
		throw StatusCodeExcept(403); // Forbidden

	}
	else if (isRegFile(url)) {
		std::cout << "is reg file" << std::endl;
		throw StatusCodeExcept(200);
	}
	else
		throw StatusCodeExcept(404);
}

// void Request::Get(){

// 	struct stat buffer;
// 	int st;

// 	st = stat(url.c_str(), &buffer);
// 	if (st == -1)
// 		throw StatusCodeExcept(404);

// 	if (S_ISDIR(buffer.st_mode)){
// 		std::cout << "is Dir" << std::endl;
// 		if (!url.empty() && url[url.size() - 1] != '/')
// 			throw rediractionExcept(301, reqURL + "/" + queryString);
// 		if ()
// 		std::string index = url + "index.html";
// 		if (stat(index.c_str(), &buffer) != -1){
// 			throw StatusCodeExcept(200);
// 		}
// 		throw StatusCodeExcept(403);

// 	}
// 	else {
// 		std::cout << "is file" << std::endl;
// 		throw StatusCodeExcept(200);
// 	}
// }

void	Request::RemoveContentDir(std::string str){
	
	struct dirent* dr;
	
	if (str[str.length() - 1] != '/')
		str += "/";
	DIR* dir = opendir(str.c_str());
	if (!dir)
		throw StatusCodeExcept(403);
	while ((dr = readdir(dir))){
		std::string name = dr->d_name;
		if (name != "." && name != "..")
		{
			if (isDir(str + name) == true)
				RemoveContentDir(str + name);
			else {
				if (std::remove((str + name).c_str()) == -1){
					closedir(dir);
					throw StatusCodeExcept(500);
				}
			}
		}
	}
	if (std::remove((str).c_str()) == -1){
		closedir(dir);
		throw StatusCodeExcept(500);
	}
	closedir(dir);
}

void Request::Delete(){
	struct stat buffer;
	int st;

	st = stat(url.c_str(), &buffer);
	if (st == -1)
		throw StatusCodeExcept(404);
	if (S_ISDIR(buffer.st_mode)){
		if (access(url.c_str(), W_OK) == -1)
			throw StatusCodeExcept(403);
		RemoveContentDir(url);
	}
	else {
		if (std::remove(url.c_str()) == -1)
			throw StatusCodeExcept(403);
	}
	throw StatusCodeExcept(204);
}

void Request::setfirstBody(){
	
	std::string fileName = path + "/" + getNewName() + type;
	ftype.open(fileName.c_str(), std::ios::binary);
	if (ftype.is_open() == 0)
		throw StatusCodeExcept(404);

	std::string num = body.substr(0, body.find("\r\n"));
	std::stringstream stream;
	stream << num;
	stream >> std::hex >> buffer;
	if (buffer == 0)
		throw StatusCodeExcept(204);

	body = body.substr(body.find("\r\n") + 2);
	this->length = body.length();
	if (this->length > buffer){
		std::stringstream stream;
		stream << body.substr(body.find("\r\n") + 2);
		stream >> std::hex >> buffer;
		if (buffer == 0){
			ftype << body.substr(0, body.find("\r\n"));
			throw StatusCodeExcept(201);
		}
	}
	ftype << body;
	body.clear();

}

void Request::getBuffer(std::string req){
		
	std::string num = req.substr(0, req.find("\r\n"));
	std::stringstream stream;
	stream << num;
	stream >> std::hex >> buffer;
	if (buffer == 0)
		throw StatusCodeExcept(201);

	req = req.substr(req.find("\r\n") + 2);
	ftype << req;
	this->length = req.length();
}

void Request::PostChunked(std::string req){

	if (!body.empty())
		setfirstBody();
	else if (!nextchunk.empty()){
		req = nextchunk + req;
		if (req[0] == '\r')
			req = req.substr(req.find("\r\n") + 2);
		getBuffer(req);
		nextchunk.clear();
	}
	else {
		size_t chunkBuffer;
		this->length += req.length();
		if (this->length > buffer) {
			chunkBuffer = req.length() - (this->length - buffer); // calcul how much i should add to the chunke.
			ftype << req.substr(0, chunkBuffer); // Push the missing part of chunke.
			req = req.substr(chunkBuffer); // remove the missing part of chunke from the request.
	
			size_t pos;
			pos = req.find("\r\n");
			if (pos != std::string::npos && pos + 2 < req.length()){
				req = req.substr(pos + 2);
				pos = req.find("\r\n");
				if (pos != std::string::npos && pos + 2 < req.length()){
					getBuffer(req);
					return ;
				}
			}
			nextchunk = req; // This when the format (\r\nBUFFER\r\n) not correct.
			req.clear();
		}
		else if (this->length == buffer){
			
			ftype << req;
			nextchunk = "";
		}
		else
			ftype << req;
	}
}

void Request::Post(std::string req) {
	if (HeadReq.find("Transfer-Encoding") != HeadReq.end())
		PostChunked(req);
	else {
		// if no body sent|!|
		if (!body.empty()){
			std::string fileName = path + "/" + getNewName() + type;
			std::cout << "upload >> " << fileName << std::endl;
			ftype.open(fileName.c_str(), std::ios::binary);
			if (!ftype.is_open())
				throw StatusCodeExcept(404);
				
			ftype << body;
			this->length = body.length();
			if (this->length >= ContentLength)
				throw StatusCodeExcept(201);
			body.clear();
		}
		else {
			ftype << req;
			this->length += req.length();
			if (this->length >= ContentLength)
				throw StatusCodeExcept(201);
		}
	}

}
