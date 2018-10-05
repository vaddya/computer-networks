#ifndef SIMPLEFTPONTCP_FTPENTITY_H
#define SIMPLEFTPONTCP_FTPENTITY_H

#include <string>
#include <utility>

class FTPEntity {
public:
    FTPEntity(std::string name, bool is_directory) : _name(std::move(name)), _is_directory(is_directory) {}

    std::string name() const {
        return _name;
    }

    bool is_directory() const {
        return _is_directory;
    }

private:
    std::string _name;

    bool _is_directory;
};


#endif //SIMPLEFTPONTCP_FTPENTITY_H
