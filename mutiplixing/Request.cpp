
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
		throw StatusCodeExcept(HttpStatus::URITooLong);
	if ((a != "GET" && a != "DELETE" && a != "POST") )
		throw StatusCodeExcept(HttpStatus::NotImplemented);
	Method = a;
	url = b;
	HeadReq.insert(std::pair<std::string,std::string>("Method", a)); // its will be removed
	HeadReq.insert(std::pair<std::string,std::string>("Location",b)); // its will be removed

	if (version != "HTTP/1.1")
		throw StatusCodeExcept(HttpStatus::HTTPVersionNotSupported);
}

void Request::CheckRequest(){

	std::map<std::string, std::string>::iterator it;
	if (Method == "POST"){
		if (this->location->getUpload() == "off" || this->location->getUploadPath().empty())
				throw StatusCodeExcept(HttpStatus::Forbidden);

		if ((it = HeadReq.find("Content-Length")) != HeadReq.end()){
			if (it->second.find_first_not_of("0123456789") != std::string::npos)
				throw StatusCodeExcept(HttpStatus::BadRequest);
			ContentLength = atol(it->second.c_str());
			if (ContentLength > server->getClientMaxBodySize())
				throw StatusCodeExcept(HttpStatus::PayloadTooLarge);
		}
		else if ((it = HeadReq.find("Transfer-Encoding")) != HeadReq.end()){
			if (it->second != "chunked")
				throw StatusCodeExcept(HttpStatus::NotImplemented);
		}
		else
			throw StatusCodeExcept(HttpStatus::BadRequest);

	}

	it = HeadReq.find("Content-Type");
	if (it != HeadReq.end()){
		type = ".";
		if (MimeTypes::getExtension(it->second.c_str()) == NULL)
			throw StatusCodeExcept(HttpStatus::UnsupportedMediaType);
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
		throw StatusCodeExcept(HttpStatus::BadRequest);
	std::string port = Host.substr(pos + 1);
	if (port.length() > 5 || port.find_first_not_of("0123456789") != std::string::npos 
		|| atoi(port.c_str()) != server->getPort())
		throw StatusCodeExcept(HttpStatus::BadRequest);
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
		queryString = url.substr(pos + 1);
		url = url.substr(0, pos);
	}
	for (std::vector<Location>::iterator it1 = server->getLocation().begin(); it1 != server->getLocation().end(); it1++) {
		if (CompareURL(it1->getPath(), url)) {
			if (it1->getPath().length() > res.length()) {
				res = it1->getPath();
				location = &(*it1);
			}
		}
	}
	if (!res.empty()) {
		if (!root.empty() && root[root.length() - 1] != '/')
			root += '/';
		if (url[res.length()] == '/')
			res += '/';
		else {
			pos = url.find("/", res.length());
			if (pos != std::string::npos)
				res = url.substr(0, pos + 1);
			else
				res = url;
		}
		this->url = url.replace(0, res.length(), root);
	}
	else
		throw StatusCodeExcept(HttpStatus::NotFound);
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
			HeadReq.insert(std::pair<std::string,std::string>(key,val));
			req.erase(0, req.find("\r\n") + 2);
		}
        body = req;
        HeaderIsDone = 1;
		specificServ();
		matchingURL(url);
		if (this->location->getMethods().find(Method) == std::string::npos)
			throw StatusCodeExcept(HttpStatus::MethodNotAllowed);
        CheckRequest();
	}
	if (Method == "POST"){
		// check if upload is on |!|
		Post(req);
	}
	else if (Method == "GET")
		Get();
	else
		Delete();
}

void Request::Get(){

	struct stat buffer;
	int st;

	st = stat(url.c_str(), &buffer);
	if (st == -1)
		throw StatusCodeExcept(HttpStatus::NotFound);

	if (S_ISDIR(buffer.st_mode)){
		std::cout << "is Dir" << std::endl;
		std::string index = url + "index.html";
		if (stat(index.c_str(), &buffer) != -1){
			throw StatusCodeExcept(HttpStatus::OK);
		}
		throw StatusCodeExcept(HttpStatus::Forbidden);

	}
	else {
		std::cout << "is file" << std::endl;
		throw StatusCodeExcept(HttpStatus::OK);
	}
}

void	Request::RemoveContentDir(std::string str){
	
	struct dirent* dr;
	
	if (str[str.length() - 1] != '/')
		str += "/";
	DIR* dir = opendir(str.c_str());
	if (!dir)
		throw StatusCodeExcept(HttpStatus::Forbidden);
	while ((dr = readdir(dir))){
		std::string name = dr->d_name;
		if (name != "." && name != "..")
		{
			if (isDir(str + name) == true)
				RemoveContentDir(str + name);
			else {
				if (std::remove((str + name).c_str()) == -1){
					closedir(dir);
					throw StatusCodeExcept(HttpStatus::InternalServerError);
				}
			}
		}
	}
	if (std::remove((str).c_str()) == -1){
		closedir(dir);
		throw StatusCodeExcept(HttpStatus::InternalServerError);
	}
	closedir(dir);
}

void Request::Delete(){
	struct stat buffer;
	int st;

	st = stat(url.c_str(), &buffer);
	if (st == -1)
		throw StatusCodeExcept(HttpStatus::NotFound);
	if (S_ISDIR(buffer.st_mode)){
		if (access(url.c_str(), W_OK) == -1)
			throw StatusCodeExcept(HttpStatus::Forbidden);
		RemoveContentDir(url);
	}
	else {
		if (std::remove(url.c_str()) == -1)
			throw StatusCodeExcept(HttpStatus::Forbidden);
	}
	throw StatusCodeExcept(HttpStatus::NoContent);
}

void Request::setfirstBody(){

	ftype.open((url + "image" + type).c_str(), std::ios::binary);
	if (ftype.is_open() == 0)
		throw StatusCodeExcept(HttpStatus::NotFound);

	std::string num = body.substr(0, body.find("\r\n"));
	std::stringstream stream;
	stream << num;
	stream >> std::hex >> buffer;
	if (buffer == 0)
		throw StatusCodeExcept(HttpStatus::NoContent);

	body = body.substr(body.find("\r\n") + 2);
	this->length = body.length();
	if (this->length > buffer){
		std::stringstream stream;
		stream << body.substr(body.find("\r\n") + 2);
		stream >> std::hex >> buffer;
		if (buffer == 0){
			ftype << body.substr(0, body.find("\r\n"));
			throw StatusCodeExcept(HttpStatus::Created);
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
		throw StatusCodeExcept(HttpStatus::Created);

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
	// change all the url to path of upload |!|
	if (HeadReq.find("Transfer-Encoding") != HeadReq.end())
		PostChunked(req);
	else {
		if (!body.empty()){
			std::string fileName = url + getNewName() + type;
			std::cout << "upload >> " << fileName << std::endl;
			ftype.open(fileName.c_str(), std::ios::binary);
			if (!ftype.is_open())
				throw StatusCodeExcept(HttpStatus::NotFound);
				
			ftype << body;
			this->length = body.length();
			if (this->length >= ContentLength)
				throw StatusCodeExcept(HttpStatus::Created);
			body.clear();
		}
		else {
			ftype << req;
			this->length += req.length();
			if (this->length >= ContentLength)
				throw StatusCodeExcept(HttpStatus::Created);
		}
	}

}
