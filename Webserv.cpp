/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:21 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/27 18:29:41 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(std::string file){
    ParsConfigFile PCF(file, dataServers);
    exec();
}

Webserv::~Webserv(){}

void Webserv::exec() {
    std::cout << "Execution part" << std::endl;
}
