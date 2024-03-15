/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:23 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/15 16:09:39 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(Server &serv){
    path = "";
    methods = "";
	rediraction = "";
    root = serv.getRoot();
    autoIndex = serv.getAutoIndex();
    upload = serv.getUpload();
}

Location::~Location(){}

std::string Location::getRediraction() {
    return rediraction;
}
// change here
std::map<std::string, std::string> Location::getErrorPages(){
	return errorPages;
}

// souad helps
std::string Location::getErrorPagesPath(std::string key) {
	if (errorPages.find(key) != errorPages.end())
		return errorPages[key];
	return "";
}

std::vector<std::string> &Location::getIndex() {
	return index;
}

std::string Location::getAutoIndex() {
    return autoIndex;
}

std::string Location::getUpload() {
    return upload;
}

std::string Location::getRoot() {
    return root;
}

std::string Location::getPath() {
    return path;
}

std::string Location::getMethods() {
    return methods;
}

void Location::setRoot(std::string value) {
	root = value;
}

void Location::setPath(std::string value) {
	path = value;
}

void Location::setMethods(std::string value) {
	methods = value;
}

void Location::setRediraction(std::string value) {
    rediraction = value;
}

void Location::setAutoIndex(std::string value) {
	if (value == "on" || value == "ON")
    	autoIndex = "on";
	else if (value == "OFF" || value == "off")
    	autoIndex = "off";
	else
		throw std::runtime_error("autoindex : invalide value");
}

void Location::setUpload(std::string value) {
	if (value == "on" || value == "ON")
    	upload = "on";
	else if (value == "OFF" || value == "off")
    	upload = "off";
	else
		throw std::runtime_error("upload : invalide value");
}

void Location::setIndex(std::string value) {
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

void Location::insertErrorPages(std::string str, std::string value) {
	std::stringstream ss;
	std::string token;
	std::string path;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		path = value.substr(0, pos + 1);
	pos = path.find_last_of(" ");
	if (pos == std::string::npos)
		throw std::runtime_error("invalid value " + str);
	
	path = path.substr(pos + 1, value.length() - pos);

	value = value.substr(0, pos);

	ss << value;
	
	while (ss >> token) {
		if (token.length() > 3 || token.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error("invalid value " + str);
		if (errorPages.find(token) != errorPages.end())
			errorPages.at(token) = path;
		else
			errorPages.insert(std::make_pair(token, path));
	}
}

void Location::setErrorPages(std::string value) {
	std::stringstream ss1;
	std::string token_1;
	std::string str;

	size_t pos = value.find_last_not_of(" ");
	if (pos != std::string::npos)
		str = value.substr(0, pos + 1);
	if (str.empty() || str.at(0) != '[' || str.at(str.size() - 1) != ']')
		throw std::runtime_error("---error_pages : invalid foramt " + value);
	str = str.substr(1, str.size() - 2);
	if(str.empty() || str.find_first_of("[]") != std::string::npos)
		throw std::runtime_error("+++error_pages : invalid foramt " + value);

	ss1 << str;
	while (getline(ss1, token_1, ','))
		insertErrorPages(str, token_1);
}

void Location::checkLocation() {
	if (path.empty() || methods.empty())
		throw std::runtime_error("location importent data : path | methods ...");
}

// void Location::checkLocation(){
    
//     for (std::map<std::string, std::string>::iterator it = arg.begin() ; it != arg.end(); it++) {
//         if (it->second.empty())
//             throw std::runtime_error("location importent data : path | methods ...");
//     }
// }

void Location::printArg() {
    std::cout << "            path : *" << path << "*" << std::endl;
    std::cout << "            root : *" << root << "*" << std::endl;
    std::cout << "            methods : *" << methods << "*" << std::endl;
	if (!getIndex().empty()) {
		std::cout << "            index : ";
		for (std::vector<std::string>::iterator it = getIndex().begin(); it != getIndex().end(); it++) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
}

