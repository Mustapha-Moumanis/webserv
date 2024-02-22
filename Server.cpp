/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 13:50:49 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/22 14:05:00 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){
    arg.insert(std::pair<std::string, std::string>("server_name", ""));
    arg.insert(std::pair<std::string, std::string>("root", "/var/www/")); 
    arg.insert(std::pair<std::string, std::string>("port", ""));
}

Server::~Server(){}

std::string Server::getValue(std::string key) {
    return arg.find(key)->second;
}

void Server::addArg(std::string key, std::string value)
{
    std::map<std::string, std::string>::iterator it = arg.find(key);
    if (it == arg.end()) {
        std::cout << "key *"<< key << "* not found " << std::endl;
    }
    else {
        it->second = value;
    }
}

void Server::addLocat(Location &locat) {
    locations.push_back(locat);
}

void Server::printArg() {
    for (std::map<std::string, std::string>::iterator it = arg.begin(); it != arg.end(); it++) {
        std::cout << "    " << it->first << " : " << it->second << std::endl;
    }
    std::cout << "        Location : " << std::endl;
    for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
        it->printArg();
    }
}