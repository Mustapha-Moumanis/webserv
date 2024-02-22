/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsConfigFile.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:08:58 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/21 22:20:55 by mmoumani         ###   ########.fr       */
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
		
		std::cout << "--------------begin-----------------" << std::endl;
		std::cout << "pos : *" << pos << "*"<< std::endl;
		std::cout << "key : *" << key << "*"<< std::endl;
		std::cout << "value : *" << value << "*" << std::endl;
		std::cout << "------------------------------------" << std::endl;	
			
		if (key == "server" && value.empty())
		{
			spaces++;
			newServer();
		}
		// throw std::runtime_error("invalid config file");
		
		// 	servers.push_back(Server);
		// 	// servers.end()->addArg(key, value);
		// else
		// 	std::cout << "error\n";
		
	}
	std::cout << ">> " << spaces << std::endl;
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
	// Server serv;
	
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
		std::cout << "------------insind server--------------" << std::endl;
		std::cout << "pos : *" << pos << "*"<< std::endl;
		std::cout << "key : *" << key << "*"<< std::endl;
		std::cout << "value : *" << value << "*" << std::endl;
		std::cout << "---------------------------------------" << std::endl;
		if (key == "server" && value.empty())
		{
			spaces++;
			newServer();
		}
	}
}