#ifndef SIMPLEFTPONUDP_PACKAGE_H
#define SIMPLEFTPONUDP_PACKAGE_H

#include <cstring>
#include <cassert>
#include <algorithm>
#include <vector>
#include <memory>
#include "package_type.h"
#include "response.h"
#include "request.h"

class Package {

public:
    static Package parse(char *buffer, ssize_t size);

    static Package request(size_t counter, Request request, const char *data, size_t data_size);

    static Package request(size_t counter, Request request);

    static Package response(size_t counter, Response response, const char *data, size_t data_size);

    static Package response(size_t counter, Response response, std::vector<std::string> list);

    static Package response(size_t counter, Response response);

    static Package ack(size_t counter, size_t n);

    static Package empty();

    size_t fillBuffer(char *buffer, size_t buffer_size) const;

    size_t getCounter() const;

    PackageType getType() const;

    Request getRequest() const;

    Response getResponse() const;

    char *getData() const;

    size_t getDataSize() const;

    size_t extractAck() const;

    std::vector<std::string> extractList() const;

    std::string extractString() const;

    std::string toString() const;

private:
    Package(size_t counter, PackageType type, unsigned char reqresp, const char *data, size_t date_size);

    Package(char *buffer, ssize_t size);

    size_t counter;
    size_t counter_size = sizeof counter;
    PackageType type;
    size_t type_size = sizeof type;
    unsigned char reqresp;
    size_t reqresp_size = sizeof reqresp;
    std::shared_ptr<char[]> data;
    size_t data_size;
};

#endif //SIMPLEFTPONUDP_PACKAGE_H
