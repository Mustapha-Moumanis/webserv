/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:56 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/07 16:27:36 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include <fstream>

class Client {
	private :
		Server *serv;
		bool status;
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
		void setRequest(std::string req);
		bool getStatus();
		void CheckFirstLine(std::string);
		
		std::string getNewName();
};

#endif