/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:37:16 by mmoumani          #+#    #+#             */
/*   Updated: 2024/03/09 14:58:35 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mutiplixing/Webserv.hpp"
 
int main(int ac, char **av) {
    if (ac <= 2) {
        try {
            std::string file = "conf.yml";
            if (av[1])
                file = av[1];
            
            Webserv Webs(file);
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
    else
        std::cout << "Error : Invalid arguments" << std::endl;
    return 0;
}