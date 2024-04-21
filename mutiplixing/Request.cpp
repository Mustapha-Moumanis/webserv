
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

Request::Request() : body(""), queryString(""), url(""), Method(""), length(0){
	dir = NULL;
	location = NULL;
	contentType = "";
	nextchunk = "";
	fileName = "";
	type = "";
	path = "";
	ContentLength = 0;
	HeaderIsDone = 0;
	IsChunked = 0;
	ftype = NULL;
	fCgi = NULL;
	parentDir = 0;
	headFlag = 0;
}

Request::~Request() {
	if (waitpid(p, NULL, WNOHANG) == 0){
		kill (p,9);
		waitpid(p, NULL,0);
	}
	if (ftype != NULL)
		fclose(ftype);
	if (fCgi != NULL){
		std::remove("cgi.txt");
		fclose(fCgi);
	}
	if (dir != NULL)
        closedir(dir);
	// *ptrIsCgi = 0;
}

Location *Request::getLocation() {
	return location;
}

bool Request::getHeadFlag() {
	return headFlag;
}

void Request::setServ(Server &serv) {
	this->server = &serv;
}

void Request::setDoublicateServer(std::vector<Server *> &vec) {
    doublicateServer = vec;
}

void Request::setPtrTime(clock_t *time) {
	ptrTime = time;
}

void Request::setPtrIsCgi(bool *isCgi) {
	ptrIsCgi = isCgi;
}

void Request::checkCgiTimeOut() {

	int	status;
	int j = waitpid(p, &status, WNOHANG);
	if (j != 0) {
		kill(p,9);
		waitpid(p, NULL, 0);
		*ptrIsCgi = 0;
		if (WIFEXITED(status)){
			if (WEXITSTATUS(status) != 0)
				throw StatusCodeExcept(500);
		}
		fseek(fCgi, 0, SEEK_SET);
		parssRspCGI();
	}
	clock_t time = clock() - *ptrTime;
	// printf ("It took me %ld clicks (%f seconds).\n",time,((float)time)/CLOCKS_PER_SEC);
	if (((double)time)/CLOCKS_PER_SEC >= server->getTimeOut()){
		kill(p,9);
		waitpid(p, NULL, 0);
		*ptrIsCgi = 0;
		throw StatusCodeExcept(504);
	}
}

void Request::checkTimeOut() {

	clock_t time = clock() - *ptrTime;
	// printf ("It took me %ld clicks (%f seconds).\n",time,((float)time)/CLOCKS_PER_SEC);
	if (((double)time)/CLOCKS_PER_SEC >= server->getTimeOut()){
		std::remove(fileName.c_str());	
		throw StatusCodeExcept(408);
	}
}

void Request::CheckFirstLine(std::string Fline){

	std::stringstream ss(Fline);
	std::string version;
	std::string a;
	std::string b;
	ss >> a >> b >> version;

    if (b.length() > 2048)
		throw StatusCodeExcept(414);
	if (a == "HEAD") {
		headFlag = 1;
		throw StatusCodeExcept(501);
	}
	if ((a != "GET" && a != "DELETE" && a != "POST"))
		throw StatusCodeExcept(501);
	Method = a;
	url = b;

	if (version != "HTTP/1.1")
		throw StatusCodeExcept(505);
}

void Request::CheckRequest(){

	std::map<std::string, std::string>::iterator it;
	if (Method == "POST"){
		path = this->location->getUploadPath();
		if (this->location->getUpload() == "off" || path.empty())
			throw StatusCodeExcept(403);

		if ((it = HeadReq.find("Transfer-Encoding")) != HeadReq.end()){
			if (it->second != "chunked")
				throw StatusCodeExcept(501);
		}
		else if ((it = HeadReq.find("Content-Length")) != HeadReq.end()){
			if (it->second.find_first_not_of("0123456789") != std::string::npos)
				throw StatusCodeExcept(411);
			ContentLength = atol(it->second.c_str());
			if (ContentLength > server->getClientMaxBodySize())
				throw StatusCodeExcept(413);
		}
		else
			throw StatusCodeExcept(400);
		// Upload
		if (this->location->getUpload() == "off" || path.empty())
			throw StatusCodeExcept(403);
		// if no body in POST
		if (body.empty())
			throw StatusCodeExcept(204);
	}

	it = HeadReq.find("Content-Type");
	if (it != HeadReq.end()){
		if (it->second != "application/x-www-form-urlencoded"){
			type = ".";
			if (MimeTypes::getExtension(it->second).empty())
				throw StatusCodeExcept(415);
			type += MimeTypes::getExtension(it->second);
		}
		if (!it->second.empty())
			contentType = it->second;
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

void Request::parseURL(std::string &url) {
	std::stringstream ss(url);
	std::vector<std::string> vec;
	std::string token;
	std::string res = "/";
	bool haveSlash = 0;

	if (url.at(url.size() - 1) == '/')
		haveSlash = 1;

	while (getline(ss, token, '/')) {
		if (token.empty())
			continue ;
		if (token == "..") {
			if(vec.empty())
				throw StatusCodeExcept(403);
			vec.pop_back();
		}
		else
			vec.push_back(token);
	}

	if (vec.empty()) {
		url = "/";
		return ;
	}

	for(std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++) {
		res += *it;
		if (it + 1 != vec.end())
			res += "/";
	}
	
	if (haveSlash == 1)
		res += "/";

	url = res;
}

void Request::matchingURL(std::string url) {
	std::string res;
	std::string root;

	size_t pos = url.find("?");
	if (pos != std::string::npos) {
		queryString = url.substr(pos);
		url = url.substr(0, pos);
	}

	pos = url.find_first_not_of("/");
	if (pos != 0) {
		if (pos != std::string::npos)
			url = url.substr(pos - 1);
		else
			url = "/";
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

		root = location->getRoot();

		if (!root.empty() && root[root.length() - 1] == '/')
			root = root.substr(0, root.length() - 1);

		if (!res.empty() && res[res.length() - 1] == '/')
			res = res.substr(0, res.length() - 1);

		pos = url.find("/", res.length());
		if (pos != std::string::npos)
			url.erase(0, pos);
		else
			url = "";

		if (!url.empty())
			parseURL(url);
		this->url = root + url;
		reqURL = res + url;
	}
	else
		throw StatusCodeExcept(404);
}

void Request::setRequest(std::string req) {

    if (HeaderIsDone == 0){
		if (!body.empty())
			req = body + req;
		size_t pos = req.find("\r\n\r\n");
		if (pos == std::string::npos){
			body = req;
			return ;
		}
		CheckFirstLine(req.substr(0, req.find("\r\n")));
		req.erase(0, req.find("\r\n") + 2);
		while (404) {
			if (req.substr(0, req.find("\r\n")) == ""){
				req.erase(0, req.find("\r\n") + 2);
				break;
			}
			if (req.find(": ") == std::string::npos)
				throw StatusCodeExcept(400);
			std::string key = req.substr(0, req.find(": "));
			req.erase(0, req.find(": ") + 2);
			std::string val =  req.substr(0, req.find("\r\n"));
			if (val.empty())
				throw StatusCodeExcept(400);
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
	if (Method == "POST")
		Post(req);
	else if (Method == "GET")
		Get();
	else
		Delete();
}
