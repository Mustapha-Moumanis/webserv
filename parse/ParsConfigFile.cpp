/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsConfigFile.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:08:58 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/15 16:13:17 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsConfigFile.hpp"

ParsConfigFile::ParsConfigFile(std::ifstream &fs, std::vector<Server *> &serv) : dataServers(serv), ifs(fs) {
	spaces = 0;
	AutoSpaces = 0;
	while (1337) {
		if (ifs.eof())
			break;
		std::getline(ifs, line);
		pos = line.find("#");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
		pos = line.find_first_not_of(" ");
		if (pos == std::string::npos)
			continue;
		line = line.substr(pos, line.length());
		
		if (line.empty())
			continue ;

		getKeyValue(line);

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
			throw std::runtime_error("invalid line : " + line);
			
	}
	for (std::vector<Server *>::iterator it1 = dataServers.begin(); it1 != dataServers.end(); it1++) {
		(*it1)->checkArg();
		// check doublicate 
		for (std::vector<Server *>::iterator it2 = it1 + 1; it2 < dataServers.end(); it2++) {
			if ((*it1)->getPort() == (*it2)->getPort())
				throw std::runtime_error("doublicate post");
		}
	}
	if (dataServers.empty())
		throw std::runtime_error("No server available");
	// for (std::vector<Server *>::iterator it = dataServers.begin(); it != dataServers.end(); it++) {
	// 	std::cout << "server : " << std::endl;
	// 	(*it)->printArg();
	// }
}

ParsConfigFile::~ParsConfigFile() {}

void ParsConfigFile::getKeyValue(std::string line) {
	std::stringstream ss(line);
	std::string s1;
	key = "";
	value = "";
	if (line.find(":") == std::string::npos)
		throw std::runtime_error("invalid line : " + line);
	
	getline(ss, key, ':');
	key = key.substr(0, key.find(" "));
	ss >> s1;
	getline(ss, value, '\0');
	if (!value.empty())
		value = s1 + " " + value;
	else
		value = s1;
}

// void ParsConfigFile::getLocaKeyValue(std::string line) {
// 	std::stringstream ss;
// 	std::string s1;
// 	key = "";
// 	value = "";
	
// 	if (line.find(":") == std::string::npos || line.at(0) != '-')
// 		throw std::runtime_error("invalid line : " + line);
	
// 	ss << ;
// 	getline(ss, key, ':');
// 	key = key.substr(0, key.find(" "));
// 	ss >> s1;

//	getline(ss, value, '\0');
//	if (!value.empty())
//		value = s1 + " " + value;
//	else
//		value = s1;
// }

void ParsConfigFile::setServValue(Server &serv, std::string key, std::string value) {
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
				validValue = validValue + token + " ";
		}
		if (validValue.empty())
			throw std::runtime_error(key + " : undifind value");
		serv.setMethods(validValue);
		return ;
	}
	else if (key == "error_pages") {
		serv.setErrorPages(value);
		return ;
	}
	else if (key == "index") {
		serv.setIndex(value);
		return ;
	}
	ss >> validValue;
	ss >> checkMultValue;

	if (key == "server_name")
		serv.setServNames(validValue);
	else if (key == "root")
		serv.setRoot(validValue);
	else if (key == "port")
		serv.setPort(validValue);
	else if (key == "host")
		serv.setHost(validValue);
	else if (key == "client_max_body_size")
		serv.setClientMaxBodySize(validValue);
	else if (key == "autoindex")
		serv.setAutoIndex(validValue);
	else if (key == "upload")
		serv.setUpload(validValue);
	else if (key == "-")
		throw std::runtime_error("Bad format " + key);
	else
		throw std::runtime_error(key + " : invalide value");
}

void ParsConfigFile::newServer() {
	dataServers.push_back(new Server);
	Server *serv = dataServers.back();
	
	spaces = 0;
	
	while (1337) {
		if (ifs.eof())
			break;
		std::getline(ifs, line);
		pos = line.find("#");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
		pos = line.find_first_not_of(" ");
		if (pos == std::string::npos)
			continue;
		line = line.substr(pos, line.length());
		
		if (line.empty())
			continue ;
		if (AutoSpaces == 0) {
			if (pos % 2 != 0)
				throw std::runtime_error("spaces problem : " + line);
			AutoSpaces = pos;
		}
		spaces = AutoSpaces;

		getKeyValue(line);
		
		std::stringstream ss(value);
		std::string tmp;
		ss >> tmp;

		if (key == "server") {
			if (!tmp.empty() || pos != 0)
				throw std::runtime_error("invalid line : " + line);			
			newServer();
			return;
		}
		else if (key == "location") {
			if (!tmp.empty() || pos != spaces)
				throw std::runtime_error("Error : Bad spaces or empty value : " + line);
			newLocation(*serv);
			return ;
		}
		else if (pos != spaces)
			throw std::runtime_error("Error : Bad spaces : " + line);
		else {
			if (value.empty())
				throw std::runtime_error(key + " : empty value");
			setServValue(*serv, key, value);
		}
	}
}

void ParsConfigFile::newLocation(Server &serv) {
	Location	locat(serv);
	bool		check = 0;
	spaces = AutoSpaces * 2;
	
	while (1337) {
		if (ifs.eof())
			break;
		std::getline(ifs, line);
		pos = line.find("#");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
		pos = line.find_first_not_of(" ");
		if (pos == std::string::npos)
			continue;
		line = line.substr(pos, line.length());
		
		if (line.empty())
			continue ;

		check = 0;
		if (line.at(0) == '-') {
			size_t p = line.find_first_not_of("- ");
			if (p == std::string::npos)
				throw std::runtime_error("Empty line : " + line);
			line = line.substr(p, line.length() - 1);
			check = 1;
		}

		getKeyValue(line);

		std::stringstream ss(value);
		std::string tmp;
		ss >> tmp;
	
		if (key == "server") {
			if (pos != 0 || check == 1)
				throw std::runtime_error("invalid line : " + line);
			serv.addLocat(locat);
			newServer();
			return;
		}
		else if (key == "location") {
			if (pos != spaces / 2 || check == 1)
				throw std::runtime_error("Error : Bad spaces or empty value : " + line);
			serv.addLocat(locat);
			newLocation(serv);
			return;
		}
		else if (pos != spaces)
			throw std::runtime_error("Error : Bad spaces : " + line);
		else if (check != 1)
			throw std::runtime_error("Error : Bad format : " + line);
		else
			serv.setLocValue(locat, key, value);
	}
	serv.addLocat(locat);
}