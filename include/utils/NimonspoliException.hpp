#pragma once

#include <bits/stdc++.h>

class NimonspoliException : public std::exception {
protected:
    std::string message;
public:
    NimonspoliException(std::string msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};