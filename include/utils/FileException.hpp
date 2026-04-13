#pragma once

#include "utils/NimonspoliException.hpp"
class FileException : public NimonspoliException {
private:
    std::string filename;
public:
    FileException(std::string msg, std::string fname) 
        : NimonspoliException(msg + " pada file: " + fname), 
          filename(fname) {}
    
    std::string getFilename() const { return filename; }
};