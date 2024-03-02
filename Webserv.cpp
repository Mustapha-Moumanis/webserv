/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:21 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/02 16:56:06 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(std::string file){
	ParsConfigFile PCF(file, dataServers);
	exec();
	// multiplixing();
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


void Webserv::multiplixing() {
	std::cout << "multiplixing part" << std::endl;

	// Structures for handling internet addresses
	struct epoll_event event, events[MAX_EVENTS];
	
	// Open an epoll fd
	int epfd = epoll_create(1);
	std::vector<int> AllFD;
	int fd;

	if (epfd == -1)
		throw std::runtime_error("cannot create an epoll");
	for(std::vector<Server>::iterator it = dataServers.begin(); it != dataServers.end(); it++) {
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
			throw std::runtime_error("cannot create a socket");
		
		
		// start TCP Socket //
		struct sockaddr_in addr;
		
		// int addrLen = sizeof(addr);
		memset((char *)&addr, 0, sizeof(addr));
		
		addr.sin_family = AF_INET;
		// addr.sin_addr.s_addr = htons(INADDR_ANY);
		if (inet_pton(AF_INET, it->getHost().c_str(), &addr.sin_addr) != 1)
			throw std::runtime_error("Invalid IP address!!");
		addr.sin_port = htons(it->getPort());
		
		int level = 1;
		setsockopt(fd, level, SO_REUSEADDR, &level, sizeof(addr));
		
		if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
			throw std::runtime_error("bind faild : ");
		
		if (listen(fd, 3) == -1)
			throw std::runtime_error("listen faild");
		
		// end TCP Socket //
		event.events = EPOLLIN;
		event.data.fd = fd;
		
		if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) == -1)
			throw std::runtime_error("epoll_ctl field");
		
		AllFD.push_back(fd);
	}
	
	int newSocket;
	long valueRead;
	std::string respons = "HTTP/1.x 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
	int evfd;
	while (404) {
		
		if ((evfd = epoll_wait(epfd, events, MAX_EVENTS, 0)) == -1)
			throw std::runtime_error("epoll wait field");
		for (int i = 0; i < evfd; i++) {
			
			if (std::find(AllFD.begin(), AllFD.end(), events[i].data.fd) != AllFD.end()){
				newSocket = accept(events[i].data.fd, NULL, NULL);
				std::cout << "\n------  wait for new connection  ------\n\n";
				event.events = EPOLLOUT | EPOLLIN;
				event.data.fd = newSocket;
				
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, newSocket, &event) == -1)
					throw std::runtime_error("epoll_ctl field");
			}
			else {
				if (events[i].data.fd & EPOLLIN) {
					char buffer[1024] = {0};
					valueRead = read (newSocket, buffer, 1024);
					if (valueRead < 0)
						std::cout << "no bytes are there to read" << std::endl;
					std::cout << buffer << std::endl;
				}
				else if (events[i].data.fd & EPOLLOUT) {
					write (newSocket, respons.c_str(), respons.length());
					close(newSocket);
				}
			}
		}
	}
}