/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:36:28 by shilal            #+#    #+#             */
/*   Updated: 2024/03/15 14:39:01 by shilal           ###   ########.fr       */
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
		CheckFirstLine(req.substr(0, req.find("\r\n")));
		req.erase(0, req.find("\r\n") + 2);
		while (404){
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
        CheckRequest();
	}
	else if (HeadReq.find("Methode")->second == "POST")
		Post(req);
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

void Request::Post(std::string req) {

	std::string type = MimeTypes::getExtension(HeadReq.find("Content-Type")->second.c_str());

	if (HeadReq.find("Transfer-Encoding") != HeadReq.end()){
		std::cout << "Is chunked" << std::endl;
		// body += req;
		throw StatusCodeExcept(HttpStatus::OK);
	}
	else {
		if (!body.empty()){
			ftype.open((url + "image." + type).c_str(), std::ios::binary);
			ftype << body;
			this->length = body.length();
			body.clear();
		}
		this->length += req.length();
		ftype << req;
		if (this->length >= atol(HeadReq.find("Content-Length")->second.c_str()))
			throw StatusCodeExcept(HttpStatus::OK);
	}
}