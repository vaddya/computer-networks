#ifndef SIMPLEFTPONUDP_PEER_H
#define SIMPLEFTPONUDP_PEER_H

class FTPPeer {

public:
    explicit FTPPeer(const sockaddr_in &peer) {
        ip = inet_ntoa(peer.sin_addr);
        port = ntohs(peer.sin_port);
    }

    bool operator<(const FTPPeer &rhs) const {
        return ip == rhs.ip ? port < rhs.port
                            : ip < rhs.ip;
    }

    std::string toString() const {
        return ip + ":" + std::to_string(port);
    }

    bool equals(const FTPPeer &rhs) const {
        return (ip == rhs.ip) && (port == rhs.port);
    }

private:
    std::string ip;
    int port;
};

#endif //SIMPLEFTPONUDP_PEER_H
