/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:53 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/05 21:05:11 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::~Client() {}

void Client::setServ(Server &serv) {
    this->serv = &serv;
    status = 1;
    std::cout << this->serv->getPort() << std::endl;
}

void Client::setRequest(std::string req) {
    std::cout << "request : " << req << std::endl;
}

bool Client::getStatus() {
    return status;
}

void Client::setStatus(bool status) {
    this->status = status;
}
