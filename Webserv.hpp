/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:38:38 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/27 18:45:46 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <fstream>
#include "ParsConfigFile.hpp"

class Webserv {
	private :
		std::vector<Server> dataServers;
		std::vector<int> fds;

	public :
		Webserv(std::string file);
		~Webserv();
		
		void exec();
};

#endif