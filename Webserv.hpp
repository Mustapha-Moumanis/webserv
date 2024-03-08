/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:38 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/08 10:35:16 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "ParsConfigFile.hpp"
#include "Client.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <map>

#define MAX_EVENTS 64

class Webserv {
	private :
		std::vector<Server> dataServers;
		std::map<int, Client> Clients;
		std::vector<int> fds;
		std::map<int, int> indexFD;

	public :
		Webserv(std::string file);
		~Webserv();
		
		void exec();
		void multiplixing();
};

#endif