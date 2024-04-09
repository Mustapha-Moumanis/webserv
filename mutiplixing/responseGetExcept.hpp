#ifndef RESPONSEGETEXCEPT_HPP
#define RESPONSEGETEXCEPT_HPP

#include <iostream>
#include <exception>
#include "../include/Utils.hpp"

class responseGetExcept: public std::exception {
    std::string header;
    std::string stock;
    int isFile;
    size_t pos;
    
    public :
        responseGetExcept(std::string header, std::string stock, int isFile, size_t pos) : header(header), stock(stock), isFile(isFile), pos(pos){}
        virtual ~responseGetExcept() throw() { return ; }

        std::string getHeader() const {
            return header;
        };

        std::string getStock() const {
            return stock;
        };

        int getIsFile() const {
            return isFile;
        };

        size_t getPos() const {
            return pos;
        };
        
        const char* what() const throw() {
            return "Respose exception";
        }

};

#endif