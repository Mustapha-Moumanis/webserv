/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:58:56 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/07 19:42:09 by shilal           ###   ########.fr       */
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
		~Client();
		
		void setServ(Server &serv);
		void setStatus(bool status);
		bool getStatus();
		void SentRequest(std::string);
		std::string getNewName();
};

#endif