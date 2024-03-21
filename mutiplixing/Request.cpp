/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:36:28 by shilal            #+#    #+#             */
/*   Updated: 2024/03/21 00:48:54 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cstring>

Request::Request() : HeaderIsDone(0), body(""), url(""), queryString("") , length(0){}

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
    else {
		size_t pos = b.find("?");
		if (pos != std::string::npos) {
			queryString = b.substr(pos + 1);
			b = b.substr(0, pos);
		}
		// this->url = b;
		matchingURL(b);
	}
	HeadReq.insert(std::pair<std::string,std::string>("Location",b));
	if ((a != "GET" && a != "DELETE" && a != "POST") 
		|| this->location->getMethods().find(a) == std::string::npos)
		throw StatusCodeExcept(HttpStatus::MethodNotAllowed);
	HeadReq.insert(std::pair<std::string,std::string>("Methode", a));

	if (version != "HTTP/1.1")
		throw StatusCodeExcept(HttpStatus::HTTPVersionNotSupported);
}

void Request::CheckRequest(){

	std::map<std::string, std::string>::iterator it;
	if (HeadReq.find("Methode")->second == "POST"){
		if ((it = HeadReq.find("Content-Length")) != HeadReq.end())
			std::cout << "Not chencked" << std::endl;
		else if ((it = HeadReq.find("Transfer-Encoding")) != HeadReq.end()){
			if (it->second != "chunked")
				throw StatusCodeExcept(HttpStatus::NotImplemented);	
		}
		else
			throw StatusCodeExcept(HttpStatus::BadRequest);
		std::cout << "Thers POST methode" << std::endl;
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

void Request::matchingURL(std::string url) {
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
	// std::cout << req << std::endl;
    if (HeaderIsDone == 0){
		// if (doublicateServer.size() > 1) {
		// 	std::vector<Server *>::iterator it = doublicateServer.begin();
		// 	for (; it != doublicateServer.end(); it++) {
		// 		std::vector <std::string> serv_names = (*it)->getServNames();

		// 		if (find(serv_names.begin(), serv_names.end(), HeadReq["Host"]) != serv_names.end()) {
		// 			std::cout << "hello " << std::endl;
		// 			server = *it;
		// 			break;
		// 		}
		// 	}
		// }
		// server->printArg();
		CheckFirstLine(req.substr(0, req.find("\r\n")));
		req.erase(0, req.find("\r\n") + 2);
		while (404) {
			std::string key = req.substr(0, req.find(": "));
			if (req.substr(0, req.find("\r\n")) == ""){
				req.erase(0, req.find("\r\n") + 2);
				break;
			}
			req.erase(0, req.find(": ") + 2);
			std::string val =  req.substr(0, req.find("\r\n"));
			HeadReq.insert(std::pair<std::string,std::string>(key,val));
			req.erase(0, req.find("\r\n") + 2);
		}
        body = req;
        HeaderIsDone = 1;
		std::cout << "change this -_-" << std::endl;
		// matchingURL(this->url);
        CheckRequest();
	}
	if (HeadReq.find("Methode")->second == "POST"){
		Post(req);
	}
	else if (HeadReq.find("Methode")->second == "GET")
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

void Request::hextodec(std::string str){
	
	std::stringstream stream;
	stream << str.substr(0, str.find("\r\n"));
	stream >> std::hex >> buffer;
}


void Request::Post(std::string req) {

	std::string type = MimeTypes::getExtension(HeadReq.find("Content-Type")->second.c_str());
	
	if (HeadReq.find("Transfer-Encoding") != HeadReq.end()){
		if (!body.empty()) {
			ftype.open((url + "image." + type).c_str(), std::ios::binary);
			if (ftype.is_open() == 0)
				throw StatusCodeExcept(HttpStatus::NotFound);
			hextodec(body);
			body = body.substr(body.find("\r\n") + 2);
			ftype << body;
			this->length += body.length();
			body.clear();
		}
		else {
			oldlen = this->length;
			this->length += req.length();
			if (this->length >= buffer) {
				std::string tmp = req.substr(0, (buffer - oldlen));
				ftype << tmp;
				if (req.length() == 1023){
					req = req.substr((buffer - oldlen));
					std::cout << req << std::endl;	
				}
				req = req.substr((buffer - oldlen) + 2);
				hextodec(req);
				if (buffer == 0)
					throw StatusCodeExcept(HttpStatus::OK);
				req = req.substr(req.find("\r\n") + 2);
				this->length = req.length();
			}
			// else if (this->length == buffer){
			// 	this->length = 0;
			// }
			ftype << req;
		}
	}
	else {
		if (!body.empty()){
			ftype.open((url + "image." + type).c_str(), std::ios::binary);
			if (!ftype.is_open())
				throw StatusCodeExcept(HttpStatus::NotFound);
				
			ftype << body;
			this->length = body.length();
			body.clear();
		}
		else{
			this->length += req.length();
			ftype << req;
			if (this->length >= atol(HeadReq.find("Content-Length")->second.c_str()))
				throw StatusCodeExcept(HttpStatus::OK);
		}
	}
}