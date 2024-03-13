/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:23 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/13 14:28:36 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(Server &serv){
    root = serv.getRoot();
    path = "";
    methods = "";
	rediraction = "";
    autoIndex = serv.getAutoIndex();
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

std::string Location::getRoot() {
    return root;
}

std::string Location::getPath() {
    return path;
}

std::string Location::getmethods() {
    return methods;
}

void Location::setRoot(std::string value) {
	root = value;
}

void Location::setPath(std::string value) {
	path = value;
}

void Location::setmethods(std::string value) {
	methods = value;
}
void Location::setRediraction(std::string value) {
    rediraction = value;
}

void Location::setAutoIndex(std::string value) {
    autoIndex = value;
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

void Location::setErrorPages(std::string value) {
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
	std::map<std::string, std::string>::iterator it;
	
	while (ss >> token) {
		it = errorPages.find(token);
		if (it != errorPages.end())
			errorPages[token] = path;
		else
			errorPages.insert(std::make_pair(token, path));
	}
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

