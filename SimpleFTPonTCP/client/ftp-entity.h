#ifndef SIMPLEFTPONTCP_FTPENTITY_H
#define SIMPLEFTPONTCP_FTPENTITY_H

#include <string>
#include <utility>

class FTPEntity {
public:
    FTPEntity(std::string name, bool is_directory) : name_(std::move(name)), is_directory_(is_directory) {}

    std::string name() const {
        return name_;
    }

    bool is_directory() const {
        return is_directory_;
    }

private:
    std::string name_;
    bool is_directory_;
};


#endif //SIMPLEFTPONTCP_FTPENTITY_H
