/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:56 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/07 19:06:42 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
// #include "Request.hpp"
#include <fstream>

class Client {
	private :
		Server *serv;
		bool status;
		// Request request;
		// response atribuite
		int HeaderIsDone;
		std::map<std::string, std::string> HeadReq;
		std::string body;
		std::string fileName;
		std::fstream fsBody;

	public :
		Client();
		~Client();
		
		void setServ(Server &serv);
		void setStatus(bool status);
		bool getStatus();
		std::string getNewName();
		
		void setRequest(std::string req);
		void CheckFirstLine(std::string);
		void CheckRequest();
};

#endif