/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsConfigFile.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:08:58 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/22 14:45:24 by mmoumani         ###   ########.fr       */
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
		
		if (key == "server" && value.empty()) {
			newServer();
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
			if (!value.empty() || pos != 0)
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
		else
			serv.addArg(key, value);
		
		// if (pos != spaces)
		// 	throw std::runtime_error("invalid line : " + line);
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
		
		if (pos != spaces && key != "server") {
			throw std::runtime_error("2 = invalid .line : " + line);
		}
		
		if (key == "server") {
			if (!value.empty() || pos != 0)
				throw std::runtime_error("invalid line : " + line);
			
			serv.addLocat(locat);
			servers.push_back(serv);
			// spaces = 0;
			newServer();
			return;
		}
		else if (key == "location") {
			if (!value.empty())
				throw std::runtime_error("invalid line : " + line);
			serv.addLocat(locat);
			newLocation(serv);
		}
		else
			locat.addArg(key, value);
	}
	serv.addLocat(locat);
	servers.push_back(serv);
}