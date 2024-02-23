/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsConfigFile.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:08:58 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/23 15:45:08 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsConfigFile.hpp"

ParsConfigFile::ParsConfigFile(std::string fileName) {
	if (!isRegFile(fileName))
		throw std::runtime_error(fileName + " is not a regular file");
	ifs.open(fileName);
	if (!ifs.is_open())
		throw std::runtime_error("Unable to open \"" + fileName + "\"");
	spaces = 0;
	while (1337) {
		if (ifs.eof())
			break;
		std::getline(ifs, line);
		
		pos = line.find_first_not_of(" ");
		if (pos == std::string::npos)
			continue;
		line = line.substr(pos, line.length());
		
		if (line.empty() || line[0] == '#')
			continue ;

		getKeyValue(line);
		
		// if (key != "server")
		// 	throw std::runtime_error("invalid line : " + key);

		if (key == "server") {
			std::stringstream ss(value);
			std::string tmp;
			ss >> tmp;
			if (!tmp.empty() || pos != 0)
				throw std::runtime_error("invalid line : " + line);
			newServer();
			break;
		}
		else
			throw std::runtime_error("invalid line : ");
			
	}
	for (std::vector<Server>::iterator it1 = servers.begin(); it1 != servers.end(); it1++) {
		it1->checkArg();
		// check doublicate 
		for (std::vector<Server>::iterator it2 = it1 + 1; it2 < servers.end(); it2++) {
			if (it1->getPort() == it2->getPort())
				throw std::runtime_error("doublicate post");
		}
	}
	for (std::vector<Server>::iterator i = servers.begin(); i != servers.end(); i++) {
		std::cout << "server : " << std::endl;
		i->printArg();
	}
}

ParsConfigFile::~ParsConfigFile() {}

bool ParsConfigFile::isRegFile(std::string path) {
	struct stat s;

	if (stat(path.c_str(), &s) != 0)
		return 0;
	return S_ISREG(s.st_mode);
}

void ParsConfigFile::getKeyValue(std::string line) {
	std::stringstream ss(line);
	if (line.find(":") == std::string::npos)
		throw std::runtime_error("invalid line : " + line);
	
	getline(ss, key, ':');
	key = key.substr(0, key.find(" "));
	getline(ss, value, '\0');
	
}

void ParsConfigFile::setServValue(Server &serv, std::string key, std::string value) {
	std::stringstream ss(value);
	std::string validValue;
	std::string checkMultValue;

	if (key == "server_names") {
		serv.setServNames(value);
		return ;
	}
	ss >> validValue;
	ss >> checkMultValue;
	if (validValue.empty() || (!checkMultValue.empty() &&  checkMultValue[0] != '#'))
		throw std::runtime_error(key + " : invalide value");
	if (key == "root")
		serv.setRoot(validValue);
	else if (key == "port")
		serv.setPort(validValue);
	else if (key == "host")
		serv.setHost(validValue);
	else if (key == "client_max_body_size")
		serv.setClientMaxBodySize(validValue);
	else
		std::cout << "invalide key : " << key << std::endl;
}

void ParsConfigFile::newServer() {
	Server serv;
	spaces = 4;
	
	while (1337) {
		if (ifs.eof())
			break;
		std::getline(ifs, line);
		pos = line.find_first_not_of(" ");
		if (pos == std::string::npos)
			continue;
		line = line.substr(pos, line.length());
			
		if (line.empty() || line[0] == '#')
			continue ;
		
		getKeyValue(line);
		
		if (key != "server" && pos != spaces )
			throw std::runtime_error("invalid line : " + line);
		
		if (key == "server") {
			std::stringstream ss(value);
			std::string tmp;
			ss >> tmp;
			if (!tmp.empty() || pos != 0)
				throw std::runtime_error("invalid line : " + line);
			servers.push_back(serv);
			newServer();
			return;
		}
		else if (key == "location") {
			if (!value.empty())
				throw std::runtime_error("invalid line : " + line);
			newLocation(serv);
		}
		else {
			setServValue(serv, key, value);
			// if (key == "root")
			// 	serv.setRoot(value);
			// else if (key == "port")
			// 	serv.setPort(value);
			// else if (key == "host")
			// 	serv.setHost(value);
			// else if (key == "server_names")
			// 	serv.setServNames(value);
			// else if (key == "client_max_body_size")
			// 	serv.setClientMaxBodySize(value);
			// else
			// 	std::cout << "invalide key : " << key << std::endl;
		}
	}
}

void ParsConfigFile::newLocation(Server &serv) {
	Location locat(serv);
	spaces = 8;
	
	while (1337) {
		if (ifs.eof()) {
			break;
		}
		std::getline(ifs, line);

		pos = line.find_first_not_of(" ");
		if (pos == std::string::npos)
			continue;
		line = line.substr(pos, line.length());
		
		if (line.empty() || line[0] == '#')
			continue ;

		getKeyValue(line);
		
		// if (key != "server" && key != "location" && pos != spaces) {
		// 	throw std::runtime_error("3 = invalid .line : " + line);
		// }
		std::stringstream ss(value);
		std::string tmp;
		ss >> tmp;
		if (key == "server") {
			if (!tmp.empty() || pos != 0)
				throw std::runtime_error("invalid line : " + line);
			
			serv.addLocat(locat);
			servers.push_back(serv);
			newServer();
			return;
		}
		else if (key == "location") {
			if (!value.empty() || pos != 4)
				throw std::runtime_error("invalid line : " + line);
			serv.addLocat(locat);
			newLocation(serv);
			return;
		}
		else if (pos != spaces) {
			throw std::runtime_error("3 = invalid .line : " + line);
		}
		else
			locat.addArg(key, value);
	}
	serv.addLocat(locat);
	servers.push_back(serv);
}