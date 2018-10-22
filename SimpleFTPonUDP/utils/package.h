#ifndef SIMPLEFTPONUDP_PACKAGE_H
#define SIMPLEFTPONUDP_PACKAGE_H

#include <cstring>
#include <cassert>
#include "package_type.h"
#include "response.h"
#include "request.h"

class Package {

public:
    Package(size_t counter, PackageType type, unsigned char reqresp, const char *data, size_t date_size) :
            counter(counter),
            type(type),
            reqresp(reqresp),
            data_size(date_size) {
        this->data = new char[data_size];
        memcpy(this->data, data, data_size);
    }

    Package(char *buffer, ssize_t size) {
        assert(size >= counter_size + type_size + reqresp_size);
        memcpy(reinterpret_cast<char *>(&counter), buffer, counter_size);
        memcpy(reinterpret_cast<char *>(&type), buffer + counter_size, type_size);
        memcpy(reinterpret_cast<char *>(&reqresp), buffer + counter_size + type_size, reqresp_size);
        data_size = size - counter_size - type_size - reqresp_size;
        data = new char[data_size];
        memcpy(data, buffer + counter_size + type_size + reqresp_size, data_size);
    }

    ~Package() {
        delete[] data;
    }

    static Package request(size_t counter, Request request, const char *data, size_t data_size) {
        return Package(counter, PackageType::REQUEST, request, data, data_size);
    }

    static Package request(size_t counter, Request request) {
        return Package(counter, PackageType::REQUEST, request, nullptr, 0);
    }

    static Package response(size_t counter, Response response, const char *data, size_t data_size) {
        return Package(counter, PackageType::RESPONSE, response, data, data_size);
    }

    static Package response(size_t counter, Response response) {
        return Package(counter, PackageType::RESPONSE, response, nullptr, 0);
    }

    static Package ack(size_t counter, size_t n) {
        return Package(counter, PackageType::ACK, 0, reinterpret_cast<char *>(&n), sizeof n);
    }

    size_t fillBuffer(char *buffer, size_t buffer_size) const {
        assert(buffer_size >= counter_size + type_size + reqresp_size + data_size);
        memcpy(buffer, reinterpret_cast<const char *>(&counter), counter_size);
        memcpy(buffer + counter_size, reinterpret_cast<const char *>(&type), type_size);
        memcpy(buffer + counter_size + type_size, reinterpret_cast<const char *>(&reqresp), reqresp_size);
        memcpy(buffer + counter_size + type_size + reqresp_size, data, data_size);
        return counter_size + type_size + reqresp_size + data_size;
    }

    size_t getCounter() const {
        return counter;
    }

    PackageType getType() const {
        return type;
    }

    Request getRequest() const {
        assert(type == PackageType::REQUEST);
        return static_cast<Request>(reqresp);
    }

    Response getResponse() const {
        assert(type == PackageType::RESPONSE);
        return static_cast<Response>(reqresp);
    }

    char *getData() const {
        return data;
    }

    size_t getDataSize() const {
        return data_size;
    }

    size_t extractAck() const {
        assert(type == PackageType::ACK);
        size_t ack;
        assert(data_size >= sizeof ack);
        memcpy(reinterpret_cast<char *>(&ack), data, sizeof ack);
        return ack;
    }

    std::string toString() const {
        switch (type) {
            case PackageType::ACK:
                return "[#" + std::to_string(counter) + ", " + type2string(type) + " for " +
                       std::to_string(extractAck()) + ", " + std::to_string(data_size) + "]";
            case PackageType::REQUEST:
                return "[#" + std::to_string(counter) + ", " + type2string(type) + ", " +
                       request2string(static_cast<Request>(reqresp)) + ", " + std::to_string(data_size) + "]";
            case PackageType::RESPONSE:
                return "[#" + std::to_string(counter) + ", " + type2string(type) + ", " +
                       response2string(static_cast<Response >(reqresp)) + ", " + std::to_string(data_size) + "]";
        }
    }

private:
    size_t counter;
    size_t counter_size = sizeof counter;
    PackageType type;
    size_t type_size = sizeof type;
    unsigned char reqresp;
    size_t reqresp_size = sizeof reqresp;
    char *data;
    size_t data_size;
};

#endif //SIMPLEFTPONUDP_PACKAGE_H
