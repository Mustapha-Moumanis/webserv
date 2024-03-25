#ifndef REDIRACTIONEXCEPT_HPP
#define REDIRACTIONEXCEPT_HPP

#include <iostream>
#include <exception>
#include "../include/Utils.hpp"

class rediractionExcept: public std::exception {
    int statusCode;
    std::string url;
    
    public :
        rediractionExcept(int code, std::string url) : statusCode(code), url(url) {}
        virtual ~rediractionExcept() throw() { return ; }
        
        int getStatusCode() const {
            return statusCode;
        };
        
        std::string getURL() const {
            return url;
        };

        const char* what() const throw() {
            return reasonPhrase(this->statusCode);
        }

};

#endif