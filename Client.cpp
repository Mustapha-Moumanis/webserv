/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:53 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/07 16:28:29 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <fstream>
#include <cctype>

Client::Client() : status(1), HeaderIsDone(0), body("") {
    fileName = "Data/" + getNewName() + ".txt";
    
    fsBody.open(fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

}

Client::~Client() {
    // std::cout << "Client distractor" << std::endl;
    fsBody.close();
    // remove(fileName.c_str());
    
}

void Client::setServ(Server &serv) {
    this->serv = &serv;
    std::cout << this->serv->getPort() << std::endl;
}

void Client::setRequest(std::string req) {
    std::cout << "setRequest : " << std::endl;

    if (HeaderIsDone == 0){
		CheckFirstLine(req.substr(0, req.find("\r\n")));
		req.erase(0, req.find("\r\n") + 2);

		while (404){
			std::string key = req.substr(0, req.find(": "));
			if (req.substr(0, req.find("\r\n")) == ""){
				req.erase(0, req.find("\r\n") + 2);
				break;
			}
			req.erase(0, req.find(": ") + 2);
			std::string val =  req.substr(0, req.find("\r\n"));
			HeadReq.insert(std::pair<std::string,std::string>(key,val));
			req.erase(0, req.find("\r\n") + 2);
		}
        body = req;
        fsBody << req;
        HeaderIsDone = 1;
	}
	else if (HeaderIsDone == 1 && HeadReq.find("Methode")->second == "POST") {
        std::cout << "1 : " << std::endl;
        body += req;
        fsBody << req;
    }
}

bool Client::getStatus() {
    return status;
}

void Client::setStatus(bool status) {
    this->status = status;
}

void Client::CheckFirstLine(std::string Fline){

	std::stringstream ss(Fline);

	std::string version;
	std::string a;
	std::string b;
	ss >> a >> b >> version;
	if (a != "GET" && a != "DELETE" && a != "POST")
		throw::std::runtime_error("Error Methode not emplemented");
	else
		HeadReq.insert(std::pair<std::string,std::string>("Methode", a));
	HeadReq.insert(std::pair<std::string,std::string>("Location",b));
	if (version != "HTTP/1.1")
		throw("Error in HTTP/1.1");
}

std::string Client::getNewName()
{
    std::string name;
    char c;

    std::ifstream randomFile("/dev/random", std::ios::binary);
    if (!randomFile.is_open())
        return "";
    
    while (name.length() < 8)
    {
        randomFile.read(&c, 1);
        if (randomFile.fail())
            return "";
        if (std::isalpha(c))
            name += c;
    }
    randomFile.close();
    return name;
}
