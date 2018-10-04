#ifndef SIMPLEFTPONTCP_FTPENTITY_H
#define SIMPLEFTPONTCP_FTPENTITY_H

#include <string>
#include <utility>

class FTPEntity {
public:
    FTPEntity(const std::string &&name, bool is_directory) : _name(name), _is_directory(is_directory) {}

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
