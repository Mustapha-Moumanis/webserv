/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:56 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/08 11:04:41 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Request.hpp"
#include <fstream>

class Client {
	private :
		Server *serv;
		bool status;
		Request request;
		// response atribuite
		// std::string fileName;
		// std::fstream fsBody;

	public :
		Client();
		Client(Client const &other);
		Client &operator=(Client const &other);
		~Client();
		
		void setServ(Server &serv);
		void setStatus(bool status);
		void SentRequest(std::string);
		
		Server *getServ();
		bool getStatus();
		Request &getRequest();

		// std::string getNewName();
};

#endif