#include "Request.hpp"
#include <fstream>
#include <cctype>
#include <algorithm>
#include <cstring>

Request::Request() : HeaderIsDone(0), body(""), url("") {}

Request::~Request() {}

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
		throw std::runtime_error("414 URI Too Long");
    else {
		// separate url from query "?key=value&..."
		size_t pos = b.find("?");
		if (pos != std::string::npos)
			matchingURL(b.substr(0, pos));
		else
			matchingURL(b);
	}
	HeadReq.insert(std::pair<std::string,std::string>("Location",b));

	if ((a != "GET" && a != "DELETE" && a != "POST") 
		|| this->location->getmethods().find(a) == std::string::npos)
		throw std::runtime_error("405 Method Not Allowed");
	HeadReq.insert(std::pair<std::string,std::string>("Methode", a));

	if (version != "HTTP/1.1")
		throw StatusCodeExcept(HttpStatus::HTTPVersionNotSupported);
}

void Request::setRequest(std::string req) {
	// throw StatusCodeExcept(HttpStatus::HTTPVersionNotSupported);
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
	if (HeaderIsDone == 1){
		if (HeadReq.find("Methode")->second == "POST")
        	body += req;
    	else if (HeaderIsDone == 1 && HeadReq.find("Methode")->second == "GET")
			Get();
		else
			return ;
	}
}

void Request::CheckRequest(){

	std::map<std::string, std::string>::iterator it;
	if (HeadReq.find("Methode")->second == "POST"){
		if ((it = HeadReq.find("Content-Length")) != HeadReq.end())
			std::cout << "Not chencked" << std::endl;
		else if ((it = HeadReq.find("Transfer-Encoding")) != HeadReq.end()){
			if (it->second != "chunked")
				throw std::runtime_error("501 Not Implemanted");	
		}
		else
			throw std::runtime_error("400 Bad request");
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
		throw std::runtime_error("404 Not Found");
}

void Request::Get(){

	struct stat buffer;
	int st;

	st = stat(url.c_str(), &buffer);
	if (st == -1)
		throw std::runtime_error("404 Not Found");

	if (S_ISDIR(buffer.st_mode)){
		std::cout << "is Dir" << std::endl;
		std::string index = url + "index.html";
		if(stat(index.c_str(), &buffer) != -1){
			throw std::runtime_error("200 OK");
		}
		std::cout << "No index" << std::endl;
	}
	else {
		std::cout << "is file" << std::endl;
		throw std::runtime_error("200 OK");
	}
}