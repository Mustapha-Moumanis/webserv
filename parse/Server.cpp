/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 21:31:17 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/13 13:46:05 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){
	port = 0;
	host = "";
	serverName = "";
	root = "/var/www/";
	clientMaxBodySize = 2147483648;
	autoIndex = "off";
	// errorPage = kkjsgdfjsd;
}

Server::~Server(){}

// set variables

void Server::setRoot(std::string value) {
	// std::ifstream ifs;
	// if (!isDir(value))
	// 	throw std::runtime_error(value + " is not a Directory");
	// ifs.open(value);
	// if (!ifs.is_open())
	// 	throw std::runtime_error("Unable to open \"" + value + "\"");
	// ifs.close();

	root = value;
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

// change
void Server::setClientMaxBodySize(std::string value) {
	size_t myPos;
	std::string sUnit;
	int change = 1;

	myPos = value.find_first_not_of("0123456789");
	if (myPos == std::string::npos || myPos == 0)
		throw std::runtime_error("client_max_body_size invalide specify values in units");
	sUnit = value.substr(myPos, value.length() - myPos);
	if (sUnit.length() != 1 || sUnit.find_first_of("BKMG") == std::string::npos)
		throw std::runtime_error("client_max_body_size units [B, K, M, G]");
	if (sUnit == "K")
		change = 1024;
	else if (sUnit == "M")
		change = 1048576;
	else if (sUnit == "G")
		change = 1073741824;
	
	std::stringstream ss(value.substr(0, myPos));
	ss >> clientMaxBodySize;
	clientMaxBodySize *= change;
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

// souad helps
std::string Server::getErrorPagesPath(std::string key) {
	if (errorPages.find(key) != errorPages.end())
		return errorPages[key];
	return "";
}

std::vector<std::string> &Server::getIndex() {
	return index;
}

std::string Server::getAutoIndex() {
    return autoIndex;
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

void Server::setAutoIndex(std::string value) {
    autoIndex = value;
}

void Server::setErrorPages(std::string value) {
	std::stringstream ss;
	std::string token;
	std::string path;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		path = value.substr(0, pos + 1);
	pos = path.find_last_of(" ");
	if (pos == std::string::npos)
		throw std::runtime_error("invalid value " + value);
	
	path = path.substr(pos + 1, value.length() - pos);
	// if (!isRegFile(path))
	// 	throw std::runtime_error("invalid value " + value);

	value = value.substr(0, pos);

	ss << value;
	
	while (ss >> token) {
		if (errorPages.find(token) != errorPages.end())
			errorPages[token] = path;
		else
			errorPages.insert(std::make_pair(token, path));
	}
}

void Server::setLocValue(Location &locat, std::string key, std::string value) {
	std::stringstream ss(value);
	std::string validValue;
	std::string checkMultValue;

	if (key == "methods") {
		std::string var;
		validValue = "";
		while (1337) {
			if (ss.eof())
				break;
			ss >> var;
			// std::cout << "*" << var << "*" << std::endl;
			if (var[0] == '#')
				break;
			else if (var.empty())
				throw std::runtime_error(key + " : undifind value");
			else if (var != "POST" && var != "GET" && var != "DELETE")
				throw std::runtime_error("methode : " + var + " not valide");
			else if (!validValue.empty() && validValue.find(var) != std::string::npos)
				throw std::runtime_error("methode : " + var + " allready seted");
			else
				validValue = validValue + var + " ";
		}
		if (validValue.empty())
			throw std::runtime_error(key + " : undifind value");
		locat.setmethods(validValue);
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
	else if (key == "error_page") {
		locat.setErrorPages(value);
		return ;
	}
	ss >> validValue;
	ss >> checkMultValue;
	if (validValue.empty() || (!checkMultValue.empty() &&  checkMultValue[0] != '#'))
		throw std::runtime_error(key + " : invalide value");
	if (key == "root")
		locat.setRoot(validValue);
	else if (key == "path")
		locat.setPath(validValue);
	else if (key == "autoindex") {
		if (value != "on" && value != "ON" && value != "OFF" && value != "off")
			throw std::runtime_error(key + " : invalide value");
		else
			locat.setAutoIndex(validValue);
	}
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
	if (!getIndex().empty()) {
		std::cout << "	index : ";
		for (std::vector<std::string>::iterator it = getIndex().begin(); it != getIndex().end(); it++) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
	if (!getIndex().empty()) {
		std::cout << "	errorpages : " << std::endl;
		for (std::map<std::string, std::string>::iterator it = getErrorPages().begin(); it != getErrorPages().end(); it++) {
			std::cout << "	    - " << (*it).first << " : " << (*it).second << std::endl;
		}
	}
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		std::cout << "	location : " << std::endl;
		it->printArg();
	}
}