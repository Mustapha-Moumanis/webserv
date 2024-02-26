/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 21:31:17 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/26 14:35:35 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){
	port = "";
	host = "";
	serverName = "";
	root = "/var/www/";
	clientMaxBodySize = 2147483648;
	// errorPage = kkjsgdfjsd;
}

Server::~Server(){}

// set variables

void Server::setRoot(std::string value) {
	root = value;
}

void Server::setPort(std::string value) {
	port = value;
}

void Server::setHost(std::string value) {
	host = value;
}

void Server::setServNames(std::string value) {
	serverName = value;
}

void Server::setClientMaxBodySize(std::string value) {
	size_t myPos;
	std::string sUnit;

	myPos = value.find_first_not_of("0123456789");
	if (myPos == std::string::npos || myPos == 0)
		throw std::runtime_error("client_max_body_size invalide specify values in units");
	sUnit = value.substr(myPos, value.length() - myPos);
	if (sUnit.length() != 1 || sUnit.find_first_of("BKMG") == std::string::npos)
		throw std::runtime_error("client_max_body_size units [B, K, M, G]");
	
	std::stringstream ss(value.substr(0, myPos));
	ss >> clientMaxBodySize;
	// had l3iba fchkel
	if (clientMaxBodySize > 2147483648)
		throw std::runtime_error("client_max_body_size too long maximum 2G");
		
	
}

// get variables

std::string Server::getRoot() {
	return root;
}

std::string Server::getPort() {
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

void Server::checkArg() {
	if (port.empty() || host.empty())
		throw std::runtime_error("importent data : port | host ...");
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		it->checkLocation();
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
	ss >> validValue;
	ss >> checkMultValue;
	if (validValue.empty() || (!checkMultValue.empty() &&  checkMultValue[0] != '#'))
		throw std::runtime_error(key + " : invalide value");
	if (key == "root")
		locat.setRoot(validValue);
	else if (key == "path")
		locat.setPath(validValue);
	else
		std::cout << "invalide key : " << key << std::endl;
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
	// for (std::vector<std::string>::iterator it = serverName.begin(); it != serverName.end(); it++) {
	// 	std::cout << *it << " ";
	// }
	// std::cout << std::endl;
	std::cout << "	root : " << getRoot() << std::endl;
	std::cout << "	client_max_body_size : " << getClientMaxBodySize() << std::endl;
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		std::cout << "	location : " << std::endl;
		it->printArg();
	}
}
