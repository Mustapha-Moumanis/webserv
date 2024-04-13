/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:21 by mmoumani          #+#    #+#             */
/*   Updated: 2024/04/03 03:47:44 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(std::ifstream &ifs) : response(""){
	try {
		ParsConfigFile PCF(ifs, dataServers);
		initDoublicateServer();
		initDefaultServer();
		multiplixing();
	}
	catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

Webserv::~Webserv(){
	for (std::vector<Server *>::iterator it = dataServers.begin(); it != dataServers.end(); it++) {
		delete *it;
	}
	for (std::map<int, Client *>::iterator it = Clients.begin(); it != Clients.end(); it++) {
		delete it->second;
	}
}

void Webserv::requestPart(int fd) {
	char buffer[1024] = {0};

	ssize_t valueRead = recv(fd, buffer, 1023, 0);
	if (valueRead == -1) {
		// std::cout << "filed : read " << std::endl;
		close(fd);
		delete Clients[fd];
		Clients.erase(fd);
		return ;
	}
	std::string tmp(buffer, valueRead);
	Clients[fd]->SentRequest(tmp);
				
}

bool Webserv::responseFile(int fd) {
	response = Clients[fd]->getHeader();
	if (!response.empty()) {
		Clients[fd]->setHeader("");
		if(send(fd, response.c_str(), response.length(), 0) == -1) {
			// std::cout << "Error: sending header response" << std::endl;
			return 0;
		}
	}
	else {
		std::ifstream &ifs = Clients[fd]->getInFileStream();

		char buffer[1024] = {0};
		size_t valueRead = ifs.read(buffer, sizeof(buffer)).gcount();

		if (valueRead > 0) {
			if (send(fd, buffer, valueRead, 0) == -1) {
				// std::cout << "Error: send field" << std::endl;
				return 0;
			}
			if (ifs.eof()) {
				// std::cout << "Success: File transmission complete" << std::endl;
				return 0;
			}
		}
		else
			return 0;
	}
	return 1;
}

bool Webserv::responseFolder(int fd) {
	response = Clients[fd]->getHeader();
	if (!response.empty()) {
		// get header from client 
		Clients[fd]->setHeader("");
		if(send(fd, response.c_str(), response.length(), 0) == -1) {
			// std::cout << "Error: sending header response" << std::endl;
			return 0;
		}
	}
	else {
		DIR *dir = Clients[fd]->getDirPtr();
		struct dirent* directoryEntries;
		std::string name;
		std::string buffer;
		Request &req = Clients[fd]->getRequest();

		for (int i = 0; i < 5; i++) {
			std::string item;
			directoryEntries = readdir(dir);
			if (directoryEntries == NULL) {
				buffer += "</body>\n</html>";
				break;
			}
			name = directoryEntries->d_name;
			item = req.genDirItem(name);
			if (item.empty())
				continue ;
			buffer += item;
		}
		if (!buffer.empty()) {
			if (send(fd, buffer.c_str(), buffer.size(), 0) == -1) {
				// std::cout << "Error: send field" << std::endl;
				return 0;
			}
		}
		if (directoryEntries == NULL) {
			// std::cout << "Success: File transmission complete" << std::endl;
			return 0;
		}
	}
	return 1;
}

void Webserv::multiplixing() {
	std::cout << "________________________________________________      " << std::endl;
	std::cout << "     _    _      _     _____                          " << std::endl;
	std::cout << "    | |  | |    | |   /  ___|                         " << std::endl;
	std::cout << "    | |  | | ___| |__ \\ `--.  ___ _ ____   __        " << std::endl;
	std::cout << "    | |/\\| |/ _ \\ '_ \\ `--. \\/ _ \\ '__\\ \\ / /  " << std::endl;
	std::cout << "    \\  /\\  /  __/ |_) /\\__/ /  __/ |   \\ V /      " << std::endl;
	std::cout << "     \\/  \\/ \\___|_.__/\\____/ \\___|_|    \\_/     " << std::endl;
	std::cout << "________________________________________________\n    " << std::endl;
	
	int epfd = epoll_create(1);
	if (epfd == -1)
		throw std::runtime_error("cannot create an epoll");

	struct epoll_event event, events[MAX_EVENTS];
	std::vector<int> ServsFD;
	int tmpFD;

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

	int numEvents;
	while (404) {
		// add pip ignord get problem program exit when i refresh page |!|
		signal(SIGPIPE, SIG_IGN);
		if ((numEvents = epoll_wait(epfd, events, MAX_EVENTS, -1)) == -1)
			throw std::runtime_error("epoll wait field");
		for (int i = 0; i < numEvents; i++) {
			if (std::find(ServsFD.begin(), ServsFD.end(), events[i].data.fd) != ServsFD.end()){
				int newSocket = accept(events[i].data.fd, NULL, NULL);
				if (newSocket == -1) {
					perror("accept");
					continue;
				}
				// std::cout << "\n------  wait for new connection  ------\n\n";
				// event.events = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP | EPOLLERR;
				event.events = EPOLLIN | EPOLLOUT;
				event.data.fd = newSocket;
				
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, newSocket, &event) == -1) {
					perror("epoll_ctl");
					close(newSocket);
					continue ;
				}
				
				Clients.insert(std::make_pair(newSocket, new Client()));
				Server *serv = getDefaultServer(dataServers[indexFD[events[i].data.fd]]->getHostPort());
				
				std::vector<Server *> &servVec = getDoublicateServer(serv->getHostPort());
				if (servVec.size() > 1)
					Clients[newSocket]->setDoublicateServer(servVec);
				
				Clients[newSocket]->setServ(serv);
				// Clients[events[i].data.fd]->setTime(clock());
			}
			else {
				// if ((events[i].events & EPOLLHUP) || (events[i].events & EPOLLRDHUP) || (events[i].events & EPOLLERR)){
				// 	std::cout << "EPOLL ERRORS" << std::endl;
				// 	close(events[i].data.fd);
				// 	delete Clients[events[i].data.fd];
				// 	Clients.erase(events[i].data.fd);
				// }
				// else
				if ((events[i].events & EPOLLIN) && Clients[events[i].data.fd]->getStatus() == 1) {
					// ------ request ------
					requestPart(events[i].data.fd);
				}
				else if ((events[i].events & EPOLLOUT) && Clients[events[i].data.fd]->getStatus() == 0)
				{
					// ------ response ------
					if (Clients[events[i].data.fd]->getThingsToRes() == _FILE) {
						// ------ response file ------
						if (responseFile(events[i].data.fd) == 0) {
							close(events[i].data.fd);
							delete Clients[events[i].data.fd];
							Clients.erase(events[i].data.fd);
						}
					}
					else if (Clients[events[i].data.fd]->getThingsToRes() == _FOLDER) {
						// ------ response folder ------
						if (responseFolder(events[i].data.fd) == 0) {
							close(events[i].data.fd);
							delete Clients[events[i].data.fd];
							Clients.erase(events[i].data.fd);
						}
					}
					else {
						response = Clients[events[i].data.fd]->getResponse();
						
						send(events[i].data.fd, response.c_str(), response.length(), 0);
						close(events[i].data.fd);
						delete Clients[events[i].data.fd];
						Clients.erase(events[i].data.fd);
					}
				}
				else {
					clock_t time = Clients[events[i].data.fd]->getTime();
					time = clock() - time;
 					// printf ("It took me %ld clicks (%f seconds).\n",time,((float)time)/CLOCKS_PER_SEC);
					if (((double)time)/CLOCKS_PER_SEC >= Clients[events[i].data.fd]->getServ()->getTimeOut()){
						Clients[events[i].data.fd]->setIfTimeOut(1);
						Clients[events[i].data.fd]->SentRequest("");
					}
				}
			}
		}
	}
}

void Webserv::initDoublicateServer() {
	for (std::vector<Server *>::iterator it = dataServers.begin(); it != dataServers.end(); it++) {
		std::string token = (*it)->getHostPort();
		std::map<std::string, std::vector<Server *> >::iterator it2 = doublicateServer.find(token);
		if (it2 == doublicateServer.end()) {
			std::vector<Server *> servVec;
			servVec.push_back(*it);
			doublicateServer.insert(std::make_pair(token, servVec));
		}
		else
			doublicateServer[token].push_back(*it);
	}
	// for (std::map<std::string, std::vector<Server *> >::iterator it = doublicateServer.begin(); it != doublicateServer.end(); it++) {
	// 	std::cout << (*it).first << std::endl;
	// }
}

void Webserv::initDefaultServer() {
	std::map<std::string, std::vector<Server *> >::iterator it1 = doublicateServer.begin();
	
	for (; it1 != doublicateServer.end(); it1++) {
		std::vector <Server *> servVec = it1->second;
		Server *def = servVec.at(0);
		if (servVec.size() != 1) {
			for (std::vector<Server *>::iterator it2 = servVec.begin(); it2 != servVec.end(); it2++) {
				std::vector <std::string> serv_names = (*it2)->getServNames();
				if (!serv_names.empty() && serv_names.at(0) == "_") {
					def = *it2;
					break;
				}
			}
		}
		defaultServer.insert(std::make_pair(it1->first, def));
	}
}

std::vector<Server *> &Webserv::getDoublicateServer(std::string host) {
	return doublicateServer[host];
}

Server *Webserv::getDefaultServer(std::string host) {
	if (defaultServer.find(host) != defaultServer.end())
		return defaultServer[host];
	return NULL;
}
