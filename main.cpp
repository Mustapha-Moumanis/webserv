/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shilal <shilal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:37:16 by mmoumani          #+#    #+#             */
/*   Updated: 2024/02/25 15:37:10 by shilal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/ParsConfigFile.hpp"

int main(int ac, char **av) {
    if (ac == 2) {
        try {
            ParsConfigFile PCF(av[1]);

        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}