/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:37:16 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/26 21:40:28 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParsConfigFile.hpp"

int main(int ac, char **av) {
    if (ac <= 2) {
        try {
            std::string file = "conf.yml";
            if (av[1])
                file = av[1];
            ParsConfigFile PCF(file);

        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}