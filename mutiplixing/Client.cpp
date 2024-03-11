/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:53 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/11 15:39:37 by mmoumani         ###   ########.fr       */
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

Client::~Client() {}



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

std::string Client::getResponse() {
    return Response;
}


void Client::setStatus(bool status) {
    this->status = status;
}

std::string Client::generateResponse(HttpStatus::StatusCode Code, std::string Msg, std::string mimeType) {
    std::string resp;
    std::stringstream ss;
    std::string sCode;
    ss << Code;
    ss >> sCode;

    resp = "HTTP/1.1 " + sCode + " " + Msg + "\r\n";
    resp += "Content-Type: " + mimeType + "\r\n\r\n";
    resp += "<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='UTF-8'>\n<title>";
    resp += sCode + " " + Msg;
    resp += "</title>\n<style>\nbody {\nfont-family: Arial, sans-serif;\nbackground-color: #f8f9fa;\ncolor: #212529;\nmargin: 0;\npadding: 0;\n}\n.container {\ntext-align: center;\nmargin-top: 20%;\n}\nh1 {\nfont-size: 3em;\n}\np {\nfont-size: 1.2em;\n}\n</style>";
    resp += "</head>\n<body>\n<div class='container'>\n<h1>";
    resp += sCode + " " + Msg;
    resp += "</h1>\n</div>\n</body>\n</html>";

    return resp;
}

void Client::SentRequest(std::string tmp){
    try {
        request.setRequest(tmp);
    }
    catch (const StatusCodeExcept &e) {

        Response = generateResponse(e.getStatusCode(), e.what(), "text/html");
        setStatus(0);
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        setStatus(0);
    }
}
