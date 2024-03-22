/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:36:28 by shilal            #+#    #+#             */
/*   Updated: 2024/03/22 17:05:58 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cstring>

Request::Request() : body(""), queryString(""), url(""), Methode(""), length(0){
	nextchunk = "";
	ContentLength = 0;
	HeaderIsDone = 0;
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
	Methode = a;
	url = b;

	HeadReq.insert(std::pair<std::string,std::string>("Methode", a)); // its will be removed
	HeadReq.insert(std::pair<std::string,std::string>("Location",b)); // its will be removed

	if (version != "HTTP/1.1")
		throw StatusCodeExcept(HttpStatus::HTTPVersionNotSupported);
}

void Request::CheckRequest(){

	std::map<std::string, std::string>::iterator it;
	if (Methode == "POST"){
		if ((it = HeadReq.find("Content-Length")) != HeadReq.end())
			ContentLength = atol(it->second.c_str());
		else if ((it = HeadReq.find("Transfer-Encoding")) != HeadReq.end()){
			if (it->second != "chunked")
				throw StatusCodeExcept(HttpStatus::NotImplemented);	
		}
		else
			throw StatusCodeExcept(HttpStatus::BadRequest);
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
	// ------I add this from check firstline()
	size_t pos = url.find("?");
	if (pos != std::string::npos) {
		queryString = url.substr(pos + 1);
		url = url.substr(0, pos);
	}
	// ------- its for get queryString
	size_t i = 0;
	std::string res;
	std::string root = server->getRoot();
	for (std::vector<Location>::iterator it1 = server->getLocation().begin(); it1 != server->getLocation().end(); it1++) {
		if ((i = CompareURL(it1->getPath(), url))) {
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
			size_t pos = url.find("/", res.length());
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
		if (this->location->getMethods().find(Methode) == std::string::npos)
			throw StatusCodeExcept(HttpStatus::MethodNotAllowed);
        CheckRequest();
	}
	if (Methode == "POST"){
		Post(req);
	}
	else if (Methode == "GET")
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

void Request::setfirstBody(std::string type){

	ftype.open((url + "image." + type).c_str(), std::ios::binary);
	if (ftype.is_open() == 0)
		throw StatusCodeExcept(HttpStatus::NotFound);
	
	std::string num = body.substr(0, body.find("\r\n"));
	std::stringstream stream;
	stream << num;
	stream >> std::hex >> buffer;
	if (buffer == 0)
		throw StatusCodeExcept(HttpStatus::NoContent);

	body = body.substr(body.find("\r\n") + 2);
	ftype << body;
	len = body.length();
	body.clear();

}

void Request::getBuffer(std::string req){
		
	std::string num = req.substr(0, req.find("\r\n"));
	std::stringstream stream;
	stream << num;
	stream >> std::hex >> buffer;
	if (buffer == 0)
		throw StatusCodeExcept(HttpStatus::OK);

	req = req.substr(req.find("\r\n") + 2);
	ftype << req;
	len = req.length();
}

void Request::PostChunked(std::string req, std::string type){

	if (!body.empty())
		setfirstBody(type);
	else if (!nextchunk.empty()){
		req = nextchunk + req;
		if (req[0] == '\r')
			req = req.substr(req.find("\r\n") + 2);
		getBuffer(req);
		nextchunk.clear();
	}
	else {
		size_t chunkBuffer;
		len += req.length();
		if (len > buffer) {
			chunkBuffer = req.length() - (len - buffer); // calcul how much i should add to the chunke.
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
		else if (len == buffer){
			ftype << req;
			nextchunk = "";
		}
		else
			ftype << req;
	}
}

void Request::Post(std::string req) {

	std::string type;
	if (HeadReq.find("Content-Type") != HeadReq.end())
		type = MimeTypes::getExtension(HeadReq.find("Content-Type")->second.c_str());
	else
		throw StatusCodeExcept(HttpStatus::NoContent);

	if (HeadReq.find("Transfer-Encoding") != HeadReq.end())
		PostChunked(req, type);
	else {
		if (!body.empty()){
			ftype.open((url + "image." + type).c_str(), std::ios::binary);
			if (!ftype.is_open())
				throw StatusCodeExcept(HttpStatus::NotFound);
				
			ftype << body;
			this->length = body.length();
			body.clear();
		}
		else {
			this->length += req.length();
			ftype << req;
			if (this->length >= ContentLength){
				// std::cout << req << std::endl;
				throw StatusCodeExcept(HttpStatus::OK);
			}
		}
	}

}
