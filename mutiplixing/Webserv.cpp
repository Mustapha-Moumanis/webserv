/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:21 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/11 18:34:49 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(std::string file){
	ParsConfigFile PCF(file, dataServers);
	// exec();
	multiplixing();
}

Webserv::~Webserv(){
	for (std::vector<Server *>::iterator it = dataServers.begin(); it != dataServers.end(); it++) {
		delete *it;
	}
		
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

	// for(std::vector<Server *>::iterator it = dataServers.begin(); it != dataServers.end(); it++) {
	for(size_t i = 0; i != dataServers.size(); i++) {
		
		tmpFD = socket(AF_INET, SOCK_STREAM, 0);
		if (tmpFD == -1)
			throw std::runtime_error("cannot create a socket");
		
		
		// start TCP Socket //
		struct sockaddr_in addr;
		
		memset((char *)&addr, 0, sizeof(addr));
		
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(dataServers[i]->getHost().c_str());
		addr.sin_port = htons(dataServers[i]->getPort());
		
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

	// std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\n<html lang='en'>\n<head>\n<meta charset='UTF-8'>\n<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n<title>404 Not Found</title>\n<style>\nbody {\nfont-family: Arial, sans-serif;\nbackground-color: #f8f9fa;\ncolor: #212529;\nmargin: 0;\npadding: 0;\n}\n.container {\ntext-align: center;\nmargin-top: 20%;\n}\nh1 {\nfont-size: 3em;\n}\np {\nfont-size: 1.2em;\n}\n</style>\n</head>\n<body>\n<div class='container'>\n<h1>404 Not Found</h1>\n</div>\n</body>\n</html>";
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
				// const Client client();
				// // Clients.insert(std::make_pair(newSocket, client));
				// std::pair<int, Client const > pair1(newSocket, client);
				// Clients.insert(pair1);
				const Client client;

				Clients.insert(std::make_pair(newSocket, client));
				Clients[newSocket].setServ(dataServers[indexFD[events[i].data.fd]]);
			}
			else {
				if ((events[i].events & EPOLLIN) && Clients[events[i].data.fd].getStatus()) {
					// request
					char buffer[1024] = {0};
					ssize_t valueRead = read (events[i].data.fd, buffer, 1023);
					if (valueRead == 0 || valueRead == -1) {
						close(events[i].data.fd);
						Clients.erase(events[i].data.fd);
						// events[i].events = EPOLLOUT;
						continue ;
					}
					std::string tmp(buffer, valueRead);
					Clients[events[i].data.fd].SentRequest(tmp);
				}
				else if ((events[i].events & EPOLLOUT) && Clients[events[i].data.fd].getStatus() == 0) {
					// response
					std::cout << "response" << std::endl;
					write (events[i].data.fd, Clients[events[i].data.fd].getResponse().c_str(), Clients[events[i].data.fd].getResponse().length());
					close(events[i].data.fd);
					Clients.erase(events[i].data.fd);
				}
			}
		}
	}
}