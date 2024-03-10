/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:23 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/27 12:26:24 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Server.hpp"

Location::Location(Server &serv){
    root = serv.getRoot();
    path = "";
    methods = "";
	rediraction = "";
    autoIndex = "off";
}

Location::~Location(){}

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

std::string Location::getRediraction() {
    return rediraction;
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
}

