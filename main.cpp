/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:37:16 by mmoumani          #+#    #+#             */
/*   Updated: 2024/04/21 17:09:38 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mutiplixing/Webserv.hpp"
 
int main(int ac, char **av) {

    if (ac <= 2) {
        std::string file = "defaultConfig.yml";
        if (av[1])
            file = av[1];
        std::ifstream ifs(file.c_str());
        if (!ifs.is_open())
            std::cerr << "Unable to open \"" << file << "\"" << std::endl;
        else {
            if (!isRegFile(file))
                std::cerr << file << " is not a regular file" << std::endl;
            else
                Webserv Webs(ifs);
        }
    }
    else
        std::cerr << "Error : Invalid arguments" << std::endl;
    return 0;
}