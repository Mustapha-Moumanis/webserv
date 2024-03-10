#ifndef STATUSCODEEXCEPT_HPP
#define STATUSCODEEXCEPT_HPP

#include <iostream>
#include <exception>
#include "../include/StatusCode.hpp"

class StatusCodeExcept: public std::exception {
    HttpStatus::StatusCode statusCode;
    
    public :
        StatusCodeExcept(HttpStatus::StatusCode code) : statusCode(code) {}
        virtual ~StatusCodeExcept() throw() { return ; }
        
        HttpStatus::StatusCode getStatusCode() const {
            return statusCode;
        };

        const char* what() const throw() {
            return HttpStatus::reasonPhrase(this->statusCode);
        }

};

#endif