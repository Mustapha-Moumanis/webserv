/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:53 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/09 17:30:19 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <fstream>
#include <cctype>

Client::Client() : status(1) {
    // fileName = "Data/" + getNewName() + ".txt";
    // fsBody.open(fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
}

Client::Client(Client const &other) {
    *this = other;
}

Client &Client::operator=(Client const &other) {
    if (this != &other) {
        this->serv = getServ();
        this->status = getStatus();
        this->request = getRequest();
    }
    return *this;
}

Client::~Client() {
    // fsBody.close();
    //remove(fileName.c_str());
}



void Client::setServ(Server &serv) {
    request.setServ(serv);
    this->serv = &serv;
    // std::cout << this->serv->getPort() << std::endl;
}

// std::string Client::getNewName()
// {
//     std::string name;
//     char c;

//     std::ifstream randomFile("/dev/random", std::ios::binary);
//     if (!randomFile.is_open())
//         return "";
    
//     while (name.length() < 8)
//     {
//         randomFile.read(&c, 1);
//         if (randomFile.fail())
//             return "";
//         if (std::isalpha(c))
//             name += c;
//     }
//     randomFile.close();
//     return name;
// }

bool Client::getStatus() {
    return status;
}

Server *Client::getServ() {
    return serv;
}

Request &Client::getRequest() {
    return request;
}

void Client::setStatus(bool status) {
    this->status = status;
}

void Client::SentRequest(std::string tmp){
    try {
        request.setRequest(tmp);
    }
    catch (const StatusCodeExcept &e) {
        std::cout << "StatusCodeExcept : " << e.what() << std::endl;
        setStatus(0);
    }
    catch (const std::exception &e) {
        // HttpStatus::StatusCode st;
        std::cout << e.what() << std::endl;
        setStatus(0);
    }
}