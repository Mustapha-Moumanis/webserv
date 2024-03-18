/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:38 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/18 20:51:08 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "../parse/ParsConfigFile.hpp"
#include "Client.hpp"
#include <algorithm>
#include <cstring>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#define MAX_EVENTS 64

class Webserv {
	private :
		std::vector<Server *> dataServers;
		std::map<int, Client *> Clients;
		std::vector<int> fds; // close
		std::map<int, int> indexFD;

	public :
		Webserv(std::ifstream &ifs);
		~Webserv();
		
		void exec();
		void multiplixing();
};

#endif