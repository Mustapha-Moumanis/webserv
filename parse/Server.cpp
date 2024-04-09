/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 21:31:17 by mmoumani          #+#    #+#             */
/*   Updated: 2024/04/04 23:42:07 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){
	port = 0;
	host = "";
	methods = "";
	root = "";
	upload = "on";
	uploadPath = "";
	autoIndex = "off";
	clientMaxBodySize = 2147483648;
	timeOut = 30;
	realPath = "";
	char actualpath [PATH_MAX + 1];
	if (realpath("./", actualpath))
		realPath = realpath("./", actualpath);
	std::cout << realPath << std::endl;
}

Server::~Server(){}

// set variables

void Server::setRoot(std::string value) {
	size_t pos = value.find_last_not_of(" ");
	char actualpath [PATH_MAX + 1];
	std::string fullPath = "";
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	if (realpath(value.c_str(), actualpath))
		fullPath = realpath(value.c_str(), actualpath);
	if (fullPath.size() < realPath.size())
		throw std::runtime_error("root : invalid path " + value);
	if (fullPath.substr(0, realPath.size()) != realPath)
		throw std::runtime_error("root : invalid path " + value);
	root = fullPath;
}

void Server::setMethods(std::string value) {
	std::stringstream ss(value);
	std::string token;
	methods = "";
	
	while (ss >> token) {
		if (token != "POST" && token != "GET" && token != "DELETE")
			throw std::runtime_error("methode : " + token + " not valide");
		else if (!methods.empty() && methods.find(token) != std::string::npos)
			continue ;
		else
			methods += token + " ";
	}
	if (methods.empty())
		throw std::runtime_error("methods : undifind value " + value);
}

void Server::setPort(std::string value) {
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	if (value.length() > 5 || value.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("port : invalid value " + value);
	
	port = atoi(value.c_str());
	
	if (port < 1024 || port > 65535)
		throw std::runtime_error("port : invalid value " + value);
}

void Server::setHost(std::string value) {
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	if (value.find_first_not_of(".0123456789") != std::string::npos)
		throw std::runtime_error("host : invalid value " + value);
	std::stringstream ss(value);
	std::string tmp;
	int nb;
	int i = 0;
	while (1337) {
		if (ss.eof())
			break;
		getline(ss, tmp, '.');
		nb = atoi(tmp.c_str());
		if (nb > 255 || nb < 0 || tmp.length() > 3 || tmp.length() < 1)
			throw std::runtime_error("host : invalid value " + value);
		i++;
	}
	if (i != 4)
		throw std::runtime_error("host : invalid value " + value);
	if (nb == 0)
		throw std::runtime_error("host : invalid value " + value);

	host = value;
}

void Server::initHostPort() {
	std::stringstream ss;
	std::string servPort;
	ss << port;
	ss >> servPort;
	hostPort = host + ":" + servPort;
}

void Server::setServNames(std::string value) {
	std::stringstream ss(value);
	std::string token;

	while (ss >> token) {
		if (find(serverName.begin(), serverName.end(), token) == serverName.end())
			serverName.push_back(token);
	}
	if (find(serverName.begin(), serverName.end(), "_") != serverName.end() && serverName.size() != 1)
		throw std::runtime_error("server_name : cannot join '_' with other server names.");
}

void Server::setAutoIndex(std::string value) {
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	if (value == "on" || value == "ON" || value == "On")
    	autoIndex = "on";
	else if (value == "OFF" || value == "off" || value == "Off")
    	autoIndex = "off";
	else
		throw std::runtime_error("autoindex : invalide value");
}

void Server::setUpload(std::string value) {
	std::stringstream ss;
	std::string token;
	std::string path;
	std::ifstream ifs;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);

	ss << value;
	ss >> token;
	// get token
	if (token == "on" || token == "ON" || token == "On")
		upload = "on";
	else if (token == "OFF" || token == "off" || token == "Off")
    	upload = "off";
	else
		throw std::runtime_error("upload : invalide value " + value);
	// get path
	getline(ss, path, '\0');
	pos = path.find_first_not_of(" ");
	if (pos != std::string::npos)
		path = path.substr(pos);
	if (path.empty())
		return ;
	if (!isDir(path))
		return ;
	ifs.open(path.c_str());
	if (!ifs.is_open())
		return ;
	ifs.close();
	uploadPath = path;
}

void Server::setClientMaxBodySize(std::string value) {
	size_t myPos = value.find_last_not_of(" ");
	std::string sUnit;
	int convert = 1;

	if (myPos != std::string::npos)
		value = value.substr(0, myPos + 1);
	myPos = value.find_first_not_of("0123456789");
	if (myPos == std::string::npos || myPos == 0)
		throw std::runtime_error("client_max_body_size invalide specify values in units");
	sUnit = value.substr(myPos, value.length() - myPos);
	if (sUnit.length() != 1 || sUnit.find_first_of("bkmgBKMG") == std::string::npos)
		throw std::runtime_error("client_max_body_size units [B, K, M, G, b, k, m, g]");
	if (sUnit == "K" || sUnit == "k")
		convert = 1024;
	else if (sUnit == "M" || sUnit == "m")
		convert = 1048576;
	else if (sUnit == "G" || sUnit == "g")
		convert = 1073741824;
	value = value.substr(0, myPos);
	
	std::stringstream ss(value);
	ss >> clientMaxBodySize;
	clientMaxBodySize *= convert;
	if (value.size() > 10)
		throw std::runtime_error("Client_max_body_size too long");
}

void Server::setTimeOut(std::string value) {
	std::string sUnit;
	int convert = 1;
	
	size_t myPos = value.find_last_not_of(" ");
	if (myPos != std::string::npos)
		value = value.substr(0, myPos + 1);
	myPos = value.find_first_not_of("0123456789");
	if (myPos == std::string::npos || myPos == 0)
		throw std::runtime_error("time_out : invalide specify values in units");

	sUnit = value.substr(myPos, value.length() - myPos);
	if (sUnit.length() != 1 || sUnit.find_first_of("smSM") == std::string::npos)
		throw std::runtime_error("time_out : units seconds/minute [S, s, M, m]");
	if (sUnit == "M" || sUnit == "m")
		convert = 60;
	value = value.substr(0, myPos);

	std::stringstream ss(value);
	ss >> timeOut;
	timeOut *= convert;
	if (value.size() > 10) // max 20min
		throw std::runtime_error("time_out too long");
}

// get variables

std::string Server::getRoot() {
	return root;
}

std::string Server::getRealPath() {
	return realPath;
}

int Server::getPort() {
	return port;
}

std::string Server::getHost() {
	return host;
}

std::string Server::getHostPort() {
	return hostPort;
}

std::string Server::getMethods() {
    return methods;
}

std::vector<std::string> &Server::getServNames() {
	return serverName;
}

long long Server::getClientMaxBodySize() {
	return clientMaxBodySize;
}

double Server::getTimeOut() {
	return timeOut;
}

std::vector<Location> &Server::getLocation() {
    return locations;
}

std::map<int, std::string> &Server::getErrorPages() {
	return errorPages;
}

std::string Server::getErrorPagesByKey(int key) {
	if (errorPages.find(key) != errorPages.end())
		return errorPages.at(key);
	return "";
}

std::map<std::string, std::string> &Server::getCgiPaths() {
	return cgiPaths;
}

std::string Server::getCgiByKey(std::string key) {
	if (cgiPaths.find(key) != cgiPaths.end())
		return cgiPaths.at(key);
	return "";
}

std::vector<std::string> &Server::getIndex() {
	return index;
}

std::string Server::getAutoIndex() {
    return autoIndex;
}

std::string Server::getUpload() {
    return upload;
}

std::string Server::getUploadPath() {
    return uploadPath;
}

void Server::setIndex(std::string value) {
	std::stringstream ss;
	std::string token;

	ss << value;
	while (ss >> token) {
		if (std::find(index.begin(), index.end(), token) == index.end())
			index.push_back(token);
	}
}

void Server::insertErrorPages(std::string line, std::string value) {
	std::stringstream ss;
	std::ifstream ifs;
	std::string token;
	std::string path;
	int statusCode;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	pos = value.find_last_of(",");
	if (pos == std::string::npos)
		throw std::runtime_error("error_pages : invalid value " + line);
	path = value.substr(pos + 1, value.length() - pos);
	value = value.substr(0, pos);
	pos = path.find_first_not_of(" ");
	if (pos != std::string::npos)
		path = path.substr(pos);

	if (!isRegFile(path))
		return ;
	ifs.open(path.c_str());
	if (!ifs.is_open())
		return ;
	ifs.close();

	pos = value.find_first_not_of(" ");
	if (pos == std::string::npos)
		throw std::runtime_error("error_pages : invalid value " + line);
	value = value.substr(pos);

	ss << value;

	while (ss >> token) {
		if (token.length() != 3 || token.find_first_not_of("0123456789") != std::string::npos || !isVadilCode(atoi(token.c_str())))
			throw std::runtime_error("error_pages : invalide Status Code " + token);
		statusCode = atoi(token.c_str());
		if (errorPages.find(statusCode) != errorPages.end())
			errorPages.at(statusCode) = path;
		else
			errorPages.insert(std::make_pair(statusCode, path));
	}
}

void Server::setErrorPages(std::string value) {
	std::string token;
	std::string str;
	
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		str = value.substr(0, pos + 1);
	if (str.empty() || str.size() < 3 || str.at(0) != '[' || str.at(str.size() - 1) != ']')
		throw std::runtime_error("error_pages : invalid foramt " + value);
	
	str = str.substr(1, str.length() - 2);

	while (!str.empty()) {
		if (str.at(0) != '[')
			throw std::runtime_error("error_pages : invalid foramt " + value);
		
		pos = str.find(']');
		if (pos == std::string::npos)
			throw std::runtime_error("error_pages : invalid foramt " + value);
		else {
			token = str.substr(1, pos - 1);
			str = str.substr(pos + 1);
			insertErrorPages(value, token);
			pos = str.find_first_not_of(" ,");
			if (pos == std::string::npos) {
				if (str.find(",") != std::string::npos)
					throw std::runtime_error("error_pages : invalid foramt " + value);
				break;
			}
			if (str.substr(0, pos).find(",") == std::string::npos)
				throw std::runtime_error("error_pages : invalid foramt " + value);
			str = str.substr(pos);
		}
	}
}

void Server::insertCgiPath(std::string line, std::string value) {
	std::stringstream ss;
	std::string token;
	std::string path;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	pos = value.find_last_of(",");
	if (pos == std::string::npos)
		throw std::runtime_error("cgi_paths : invalid value " + line);
	path = value.substr(pos + 1, value.length() - pos);
	value = value.substr(0, pos);
	pos = path.find_first_not_of(" ");
	if (pos != std::string::npos)
		path = path.substr(pos);
	pos = value.find_first_not_of(" ");
	if (pos == std::string::npos)
		throw std::runtime_error("cgi_paths : invalid value " + line);
	value = value.substr(pos);
	
	ss << value;
	ss >> token;
	
	if (cgiPaths.find(token) != cgiPaths.end())
		cgiPaths.at(token) = path;
	else
		cgiPaths.insert(std::make_pair(token, path));
	
	token = "";
	ss >> token;
	
	if (!token.empty())
		throw std::runtime_error("cgi_paths : invalid value " + line);
}

void Server::setCgiPath(std::string value) {
	std::string token;
	std::string str;
	
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		str = value.substr(0, pos + 1);
	if (str.empty() || str.size() < 3 || str.at(0) != '[' || str.at(str.size() - 1) != ']')
		throw std::runtime_error("cgi_paths : invalid foramt " + value);
	
	str = str.substr(1, str.length() - 2);

	while (!str.empty()) {
		if (str.at(0) != '[')
			throw std::runtime_error("cgi_paths : invalid foramt " + value);
		
		pos = str.find(']');
		if (pos == std::string::npos)
			throw std::runtime_error("cgi_paths : invalid foramt " + value);
		else {
			token = str.substr(1, pos - 1);
			str = str.substr(pos + 1);
			insertCgiPath(value, token);
			pos = str.find_first_not_of(" ,");
			if (pos == std::string::npos) {
				if (str.find(",") != std::string::npos)
					throw std::runtime_error("cgi_paths : invalid foramt " + value);
				break;
			}
			if (str.substr(0, pos).find(",") == std::string::npos)
				throw std::runtime_error("cgi_paths : invalid foramt " + value);
			str = str.substr(pos);
		}
	}
}

void Server::setLocValue(Location &locat, std::string key, std::string value) {
	if (key == "root")
		locat.setRoot(value);
	else if (key == "path")
		locat.setPath(value);
	else if (key == "methods")
		locat.setMethods(value);
	else if (key == "return")
		locat.setRediraction(value);
	else if (key == "index")
		locat.setIndex(value);
	else if (key == "error_pages")
		locat.setErrorPages(value);
	else if (key == "cgi_paths")
		locat.setCgiPath(value);
	else if (key == "autoindex")
		locat.setAutoIndex(value);
	else if (key == "upload")
		locat.setUpload(value);
	else
		throw std::runtime_error("invalide key : " + key);
}

void Server::addLocat(Location &locat) {
	locations.push_back(locat);
}

void Server::initEmptyData() {
	if (methods.empty())
		methods = "POST GET DELETE";
	if (hostPort.empty())
		initHostPort();
	// if (uploadPath.empty())
	// 	uploadPath = root;
	// if (!index.empty()) {
	// 	std::ifstream ifs;
	// 	std::string path;
	// 	for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++) {
	// 		if (root.at(root.length() - 1) != '/')
	// 			path = root + "/" + *it;
	// 		else
	// 			path = root + *it;
	// 		if (!isRegFile(path)) {
	// 			it->erase();
	// 			continue ;
	// 		}
	// 		ifs.open(path.c_str());
	// 		if (!ifs.is_open()) {
	// 			it->erase();
	// 			continue ;
	// 		}
	// 		ifs.close();
	// 	}
	// }
}

void Server::checkArg() {
	if (port == 0  || root.empty() || host.empty())
		throw std::runtime_error("importent data : port | host | root...");
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		it->checkLocation();
	}
}

// void Server::checkArg() {
//	 for (std::map<std::string, std::string>::iterator it = arg.begin() ; it != arg.end(); it++) {
//		 if (it->second.empty())
//			 throw std::runtime_error("importent data : port | servername ...");
//	 }
//	 for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
//		 it->checkLocation();
// 	}
// }

void Server::printArg() {
	std::cout << "	port : *" << getPort() << "*" << std::endl;
	std::cout << "	host : *" << getHost() << "*" << std::endl;
	std::cout << "	server_name : ";
	for (std::vector<std::string>::iterator it = getServNames().begin(); it != getServNames().end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << "	root : " << getRoot() << "*" << std::endl;
	std::cout << "	upload : *" << getUpload() << "*" << std::endl;
	std::cout << "	upload Path : *" << getUploadPath() << "*" << std::endl;
	std::cout << "	client_max_body_size : *" << getClientMaxBodySize() << "*" << std::endl;
	std::cout << "	time_out : *" << getTimeOut() << "*" << std::endl;
	std::cout << "	autoindex : *" << getAutoIndex() << "*" << std::endl;
	std::cout << "	methods : *" << getMethods() << "*" << std::endl;
	if (!getIndex().empty()) {
		std::cout << "	index : ";
		for (std::vector<std::string>::iterator it = getIndex().begin(); it != getIndex().end(); it++) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
	if (!getErrorPages().empty()) {
		std::cout << "	errorpages : " << std::endl;
		for (std::map<int, std::string>::iterator it = getErrorPages().begin(); it != getErrorPages().end(); it++) {
			std::cout << "            - " << (*it).first << " : " << (*it).second << std::endl;
		}
	}
	if (!getCgiPaths().empty()) {
		std::cout << "	cgi_paths : " << std::endl;
		for (std::map<std::string, std::string>::iterator it = getCgiPaths().begin(); it != getCgiPaths().end(); it++) {
			std::cout << "            - " << (*it).first << " : " << (*it).second << std::endl;
		}
	}
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		std::cout << "	location : " << std::endl;
		it->printArg();
	}
}