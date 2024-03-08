/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:21 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/08 11:07:25 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(std::string file){
	ParsConfigFile PCF(file, dataServers);
	// exec();
	multiplixing();
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
	// if (inet_pton(AF_INET, dataServers.begin()->getHost().c_str(), &addr.sin_addr) != 1)
	// 	throw std::runtime_error("Invalid IP address!!");
	addr.sin_addr.s_addr = inet_addr(dataServers.begin()->getHost().c_str());
	addr.sin_port = htons(dataServers.begin()->getPort());
	int level = 1;
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &level, sizeof(int));	
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


void Webserv::multiplixing() {
	std::cout << "--------- Multiplixing part ---------" << std::endl;

	// Open an epoll fd
	int epfd = epoll_create(1);
	struct epoll_event event, events[MAX_EVENTS];
	
	if (epfd == -1)
		throw std::runtime_error("cannot create an epoll");

	std::vector<int> ServsFD;
	int tmpFD;

	// for(std::vector<Server>::iterator it = dataServers.begin(); it != dataServers.end(); it++) {
	for(size_t i = 0; i != dataServers.size(); i++) {
		
		tmpFD = socket(AF_INET, SOCK_STREAM, 0);
		if (tmpFD == -1)
			throw std::runtime_error("cannot create a socket");
		
		
		// start TCP Socket //
		struct sockaddr_in addr;
		
		memset((char *)&addr, 0, sizeof(addr));
		
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(dataServers[i].getHost().c_str());
		addr.sin_port = htons(dataServers[i].getPort());
		
		int level = 1;
		setsockopt(tmpFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &level, sizeof(int));
		
		if (bind(tmpFD, (struct sockaddr *)&addr, sizeof(addr)) == -1)
			throw std::runtime_error("bind faild");
		
		if (listen(tmpFD, 3) == -1)
			throw std::runtime_error("listen faild");
		
		// end TCP Socket //
		
		event.events = EPOLLIN;
		event.data.fd = tmpFD;
		
		if (epoll_ctl(epfd, EPOLL_CTL_ADD, tmpFD, &event) == -1)
			throw std::runtime_error("epoll_ctl field");
		
		ServsFD.push_back(tmpFD);
		indexFD[tmpFD] = i;
	}

	// std::string respons = "HTTP/1.x 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
	int numEvents;
	while (404) {
		if ((numEvents = epoll_wait(epfd, events, MAX_EVENTS, -1)) == -1)
			throw std::runtime_error("epoll wait field");
		for (int i = 0; i < numEvents; i++) {
			if (std::find(ServsFD.begin(), ServsFD.end(), events[i].data.fd) != ServsFD.end()){
				int newSocket = accept(events[i].data.fd, NULL, NULL);
				if (newSocket == -1) {
					perror("accept");
					continue;
				}
				std::cout << "\n------  wait for new connection  ------\n\n";
				event.events = EPOLLIN | EPOLLOUT;
				event.data.fd = newSocket;
				
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, newSocket, &event) == -1) {
					perror("epoll_ctl");
					close(newSocket);
				}
				std::cout << newSocket << std::endl;
				// const Client client();
				// // Clients.insert(std::make_pair(newSocket, client));
				// std::pair<int, Client const > pair1(newSocket, client);
				// Clients.insert(pair1);
				const Client client; // Declaring a const Client object, not a function

				std::pair<int, Client const> pair1(newSocket, client);
				Clients.insert(pair1);
				Clients[newSocket].setServ(dataServers[indexFD[events[i].data.fd]]);
				// client.setServ(dataServers[indexFD[events[i].data.fd]]);
				// std::cout << newSocket << std::endl;
				
			
			}
			else {
				if ((events[i].events & EPOLLIN) && Clients[events[i].data.fd].getStatus()) {
					// request
					// std::cout << "request 1" << std::endl;
					char buffer[1024] = {0};
					ssize_t valueRead = read (events[i].data.fd, buffer, 1023);
					if (valueRead == 0 || valueRead == -1) {
						close(events[i].data.fd);
						Clients.erase(events[i].data.fd);
						// close(events[i].data.fd);
						// Clients.erase(events[i].data.fd);
						// events[i].events = EPOLLOUT;
						// epoll_ctl(epfd, EPOLL_CTL_MOD, events[i].events, &event);
						continue ;
					}
					std::string tmp(buffer, valueRead);
					Clients[events[i].data.fd].SentRequest(tmp);
				}
				else if ((events[i].events & EPOLLOUT) && Clients[events[i].data.fd].getStatus() == 0) {
					// response
					std::cout << "response" << std::endl;
					std::cout << events[i].data.fd << std::endl;

					close(events[i].data.fd);
					Clients.erase(events[i].data.fd);
				}
			}
		}
	}
}