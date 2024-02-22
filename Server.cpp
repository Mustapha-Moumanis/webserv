/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 13:50:49 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/21 19:31:33 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(){
    arg.insert(std::make_pair("server_name", ""));
    arg.insert(std::make_pair("root", "/var/www/")); 
    arg.insert(std::make_pair("post", ""));
    arg.insert(std::make_pair("index", ""));
}

Server::~Server(){}

void Server::addArg(std::string key, std::string value){
    // std::cout << arg.find(key)->second << std::endl;
    std::cout << key << std::endl;
    std::cout << value << std::endl;
    // arg.insert(std::make_pair(key, value));
}