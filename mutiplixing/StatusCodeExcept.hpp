#ifndef STATUSCODEEXCEPT_HPP
#define STATUSCODEEXCEPT_HPP

#include <iostream>
#include <exception>
#include "../include/Utils.hpp"

class StatusCodeExcept: public std::exception {
    int statusCode;
    
    public :
        StatusCodeExcept(int code) : statusCode(code) {}
        virtual ~StatusCodeExcept() throw() { return ; }
        
        int getStatusCode() const {
            return statusCode;
        };

        const char* what() const throw() {
            return reasonPhrase(this->statusCode);
        }

};

#endif