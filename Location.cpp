/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:50:23 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/22 18:17:55 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Server.hpp"

Location::Location(Server &serv){
    arg.insert(std::pair<std::string, std::string>("root", serv.getRoot()));
    arg.insert(std::pair<std::string, std::string>("path", ""));
}

Location::~Location(){}

void Location::addArg(std::string key, std::string value){
    std::map<std::string, std::string>::iterator it = arg.find(key);
    if (it == arg.end()) {
        std::cout << "key *"<< key << "* not found " << std::endl;
    }
    else {
        it->second = value;
    }
}

void Location::checkLocation(){
    for (std::map<std::string, std::string>::iterator it = arg.begin() ; it != arg.end(); it++) {
        if (it->second.empty())
            throw std::runtime_error("location importent data : path ...");
    }
}

void Location::printArg() {
    for (std::map<std::string, std::string>::iterator it = arg.begin(); it != arg.end(); it++) {
        std::cout << "            "<< it->first << " : " << it->second << std::endl;
    }
}