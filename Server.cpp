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
	port = "";
	host = "";
	root = "/var/www/";
	clientMaxBodySize = "200M";
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
	std::string var;
	while (ss >> var) {
		if (var[0] == '#')
			break;
		serverName.push_back(var);
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
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		it->checkLocation();
	}
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
	std::cout << "	server_names : ";
	for (std::vector<std::string>::iterator it = serverName.begin(); it != serverName.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << "	root : " << getRoot() << std::endl;
	std::cout << "	client_max_body_size : " << getClientMaxBodySize() << std::endl;
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		std::cout << "	location : " << std::endl;
		it->printArg();
	}
}
