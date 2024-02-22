/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Server.cpp										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mmoumani <mmoumani@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/02/21 13:50:49 by mmoumani		  #+#	#+#			 */
/*   Updated: 2024/02/22 18:38:27 by mmoumani		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){
	// arg.insert(std::pair<std::string, std::string>("server_names", ""));
	// arg.insert(std::pair<std::string, std::string>("root", "/var/www/")); 
	// arg.insert(std::pair<std::string, std::string>("port", ""));
	root = "/var/www/";
	port = "";
	host = "";
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

void Server::setClientMaxBodySize(std::string value) {
	clientMaxBodySize = value;
}

void Server::setServNames(std::string value) {
	std::stringstream ss(value);
	std::string test;
	while (ss >> test) {
		if (test[0] == '#')
			break;
		serverName.push_back(test);
		// std::cout << "*" << test << "*" << std::endl;
	}
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

std::string Server::getClientMaxBodySize() {
	return clientMaxBodySize;
}

std::vector<std::string> Server::getServNames() {
	return serverName;
}

void Server::checkArg() {
	if (port.empty() || host.empty())
		throw std::runtime_error("importent data : port | host ...");
	// for (std::map<std::string, std::string>::iterator it = arg.begin() ; it != arg.end(); it++) {
	// 	if (it->second.empty())
	// 		throw std::runtime_error("importent data : port | servername ...");
	// }
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		it->checkLocation();
	}
}

// sdsdfsdf

// std::string Server::getValue(std::string key) {
//	 return arg.find(key)->second;
// }
// void Server::addArg(std::string key, std::string value)
// {
//	 std::map<std::string, std::string>::iterator it = arg.find(key);
//	 if (it == arg.end())
//		 throw std::runtime_error("unknow arg : " + key);
//	 else
//		 it->second = value;
// }

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
	// std::vector<std::string> vec = getServNames();
	std::cout << "	port : " << getPort() << std::endl;
	std::cout << "	host : " << getHost() << std::endl;
	std::cout << "	server_names : ";
	for (std::vector<std::string>::iterator it = serverName.begin(); it != serverName.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << " " << std::endl;
	std::cout << "	root : " << getRoot() << std::endl;
	std::cout << "	client_max_body_size : " << getClientMaxBodySize() << std::endl;
	std::cout << "		Location : " << std::endl;
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		it->printArg();
	}
}
// void Server::printArg() {
//	 // for (std::map<std::string, std::string>::iterator it = arg.begin(); it != arg.end(); it++) {
//	 //	 std::cout << "	" << it->first << " : " << it->second << std::endl;
//	 // }
//	 std::cout << "		Location : " << std::endl;
//	 for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
//		 it->printArg();
//	 }
// }