#ifndef RESPONSEGETEXCEPT_HPP
#define RESPONSEGETEXCEPT_HPP

#include <iostream>
#include <exception>
#include "../include/Utils.hpp"

class responseGetExcept: public std::exception {
    int statusCode;
    std::string stock;
    bool isFile;
    
    public :
        responseGetExcept(int code, std::string stock, bool isFile) : statusCode(code), stock(stock), isFile(isFile){}
        virtual ~responseGetExcept() throw() { return ; }
        
        int getStatusCode() const {
            return statusCode;
        };
        
        std::string getStock() const {
            return stock;
        };

        bool getIsFile() const {
            return isFile;
        };

        const char* what() const throw() {
            return reasonPhrase(this->statusCode);
        }

};

#endif