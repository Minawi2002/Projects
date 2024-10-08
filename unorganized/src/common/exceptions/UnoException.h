//
// Created by Manuel on 09.03.2021.
//

#ifndef UNO_UNOEXCEPTION_H
#define UNO_UNOEXCEPTION_H

#include <string>

class UnoException : public std::exception {
private:
    std::string _msg;
public:
    explicit UnoException(const std::string& message) : _msg(message) { };

    const char* what() const noexcept override {
        return _msg.c_str();
    }
};

#endif //UNO_UNOEXCEPTION_H
