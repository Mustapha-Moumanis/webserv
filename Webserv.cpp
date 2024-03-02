/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:21 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/02 16:48:27 by mmoumani         ###   ########.fr       */
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
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1)
		throw std::runtime_error("cannot create a socket");
	struct sockaddr_in addr;
	int addrLen = sizeof(addr);
	memset((char *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, dataServers.begin()->getHost().c_str(), &addr.sin_addr) != 1)
		throw std::runtime_error("Invalid IP address!!");
	addr.sin_port = htons(dataServers.begin()->getPort());
	if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		throw std::runtime_error("bind faild : ");
	if (listen(sfd, 3) == -1)
		throw std::runtime_error("listen faild");
	int newSocket;
	long valueRead;
	while (1337) {
		std::cout << "\n------  wait for new connection  ------\n\n";
		if ((newSocket = accept(sfd, (struct sockaddr *)&addr, (socklen_t *)&addrLen)) == -1)
			throw std::runtime_error("not accepted");
		char buffer[1024] = {0};
		valueRead = read (newSocket, buffer, 1024);
		if (valueRead < 0)
			std::cout << "no bytes are there to read" << std::endl;
		std::cout << buffer << std::endl;
		close(newSocket);
	}
	fds.push_back(sfd);

}
