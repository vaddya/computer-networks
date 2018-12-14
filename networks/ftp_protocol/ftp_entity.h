#ifndef FTP_PROTOCOL_FTP_ENTITY_H
#define FTP_PROTOCOL_FTP_ENTITY_H

#include <string>
#include <utility>

class FTPEntity {
public:
    FTPEntity(std::string name, bool isDirectory) : _name(std::move(name)), _isDirectory(isDirectory) {}

    std::string name() const {
        return _name;
    }

    bool isDirectory() const {
        return _isDirectory;
    }

private:
    std::string _name;

    bool _isDirectory;
};

#endif //FTP_PROTOCOL_FTP_ENTITY_H
