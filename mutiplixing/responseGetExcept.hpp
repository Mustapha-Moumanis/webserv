#ifndef RESPONSEGETEXCEPT_HPP
#define RESPONSEGETEXCEPT_HPP

#include <iostream>
#include <exception>
#include "../include/Utils.hpp"

class responseGetExcept: public std::exception {
    std::string header;
    std::string stock;
    bool isFile;
    
    public :
        responseGetExcept(std::string header, std::string stock, bool isFile) : header(header), stock(stock), isFile(isFile){}
        virtual ~responseGetExcept() throw() { return ; }

        std::string getHeader() const {
            return header;
        };

        std::string getStock() const {
            return stock;
        };

        bool getIsFile() const {
            return isFile;
        };

        // const char* what() const throw() {
        //     return reasonPhrase(this->statusCode);
        // }

};

#endif