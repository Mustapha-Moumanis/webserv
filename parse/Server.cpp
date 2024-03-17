/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 21:31:17 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/17 22:16:50 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){
	port = 0;
	host = "";
	serverName = "";
	root = "www/";
	clientMaxBodySize = 2147483648;
	methods = "GET POST DELETE";
	autoIndex = "off";
	upload = "on";
}

Server::~Server(){}

// set variables

void Server::setRoot(std::string value) {
	root = value;
}

void Server::setMethods(std::string value) {
	methods = value;
}

void Server::setPort(std::string value) {
	if (value.length() > 5 || value.find_first_not_of("0123456789") != std::string::npos)
		throw std::runtime_error("port : invalid value " + value);
	
	port = atoi(value.c_str());
	
	if (port < 1 || port > 65535)
		throw std::runtime_error("port : invalid value " + value);
}

void Server::setHost(std::string value) {
	if (value.find_first_not_of(".0123456789") != std::string::npos)
		throw std::runtime_error("port : invalid value " + value);
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
			throw std::runtime_error("port : invalid value " + value);
		i++;
	}
	if (i != 4)
		throw std::runtime_error("port : invalid value " + value);
	host = value;
}

void Server::setServNames(std::string value) {
	serverName = value;
}

void Server::setAutoIndex(std::string value) {
	if (value == "on" || value == "ON" || value == "On")
    	autoIndex = "on";
	else if (value == "OFF" || value == "off" || value == "Off")
    	autoIndex = "off";
	else
		throw std::runtime_error("autoindex : invalide value");
}

void Server::setUpload(std::string value) {
	if (value == "on" || value == "ON" || value == "On")
		upload = "on";
	else if (value == "OFF" || value == "off" || value == "Off")
    	upload = "off";
	else
		throw std::runtime_error("upload : invalide value");
}

void Server::setClientMaxBodySize(std::string value) {
	size_t myPos;
	std::string sUnit;
	int convert = 1;

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
	
	std::stringstream ss(value.substr(0, myPos));
	ss >> clientMaxBodySize;
	clientMaxBodySize *= convert;
	if (clientMaxBodySize > 2147483648)
		throw std::runtime_error("Client_max_body_size too long maximum 2G");
}

// get variables

std::string Server::getRoot() {
	return root;
}

int Server::getPort() {
	return port;
}

std::string Server::getHost() {
	return host;
}

std::string Server::getMethods() {
    return methods;
}

std::string Server::getServNames() {
	return serverName;
}

long long Server::getClientMaxBodySize() {
	return clientMaxBodySize;
}

std::vector<Location> &Server::getLocation() {
    return locations;
}

void Server::checkArg() {
	if (port == 0 || host.empty())
		throw std::runtime_error("importent data : port | host ...");
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		it->checkLocation();
	}
}
std::map<std::string, std::string> &Server::getErrorPages() {
	return errorPages;
}

std::string Server::getErrorPagesByKey(std::string key) {
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

void Server::setIndex(std::string value) {
	std::stringstream ss;
	std::string token;
	
	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		value = value.substr(0, pos + 1);
	ss << value;

	while (ss >> token) {
		if (std::find(index.begin(), index.end(), token) == index.end())
			index.push_back(token);
	}
}

void Server::insertErrorPages(std::string line, std::string value) {
	std::stringstream ss;
	std::string token;
	std::string path;

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
	pos = value.find_first_not_of(" ");
	if (pos == std::string::npos)
		throw std::runtime_error("error_pages : invalid value " + line);
	value = value.substr(pos);
	
	ss << value;
	while (ss >> token) {
		if (token.length() > 3 || token.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error("error_pages : invalid value " + line);
		if (errorPages.find(token) != errorPages.end())
			errorPages.at(token) = path;
		else
			errorPages.insert(std::make_pair(token, path));
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
		throw std::runtime_error("cgi_pathsssss : invalid value " + line);
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
	std::stringstream ss(value);
	std::string validValue;
	std::string checkMultValue;

	if (key == "methods") {
		std::string token;
		validValue = "";
		while (ss >> token) {
			if (token.empty())
				throw std::runtime_error(key + " : undifind value");
			else if (token != "POST" && token != "GET" && token != "DELETE")
				throw std::runtime_error("methode : " + token + " not valide");
			else if (!validValue.empty() && validValue.find(token) != std::string::npos)
				throw std::runtime_error("methode : " + token + " allready seted");
			else
				validValue += token + " ";
		}
		if (validValue.empty())
			throw std::runtime_error(key + " : undifind value");
		locat.setMethods(validValue);
		return ;
	}
	else if (key == "retrun") {
		locat.setRediraction(value);
		return ;
	}
	else if (key == "index") {
		locat.setIndex(value);
		return ;
	}
	else if (key == "error_pages") {
		locat.setErrorPages(value);
		return ;
	}
	else if (key == "cgi_paths") {
		locat.setCgiPath(value);
		return ;
	}
	ss >> validValue;
	ss >> checkMultValue;
	if (validValue.empty() || !checkMultValue.empty())
		throw std::runtime_error(key + " : invalide value");
	if (key == "root")
		locat.setRoot(validValue);
	else if (key == "path")
		locat.setPath(validValue);
	else if (key == "autoindex")
		locat.setAutoIndex(validValue);
	else if (key == "upload")
		locat.setUpload(validValue);
	else
		throw std::runtime_error("invalide key : " + key);
}

void Server::addLocat(Location &locat) {
	locations.push_back(locat);
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
	std::cout << "	port : " << getPort() << std::endl;
	std::cout << "	host : " << getHost() << std::endl;
	std::cout << "	server_name : " << getServNames() << std::endl;
	std::cout << "	root : " << getRoot() << std::endl;
	std::cout << "	client_max_body_size : " << getClientMaxBodySize() << std::endl;
	std::cout << "	autoindex : " << getAutoIndex() << std::endl;
	std::cout << "	methods : " << getMethods() << std::endl;
	if (!getIndex().empty()) {
		std::cout << "	index : ";
		for (std::vector<std::string>::iterator it = getIndex().begin(); it != getIndex().end(); it++) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
	if (!getErrorPages().empty()) {
		std::cout << "	errorpages : " << std::endl;
		for (std::map<std::string, std::string>::iterator it = getErrorPages().begin(); it != getErrorPages().end(); it++) {
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