#ifndef FTP_UDP_SERVER_H
#define FTP_UDP_SERVER_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <map>
#include "ftp_server_udp.h"
#include "ftp_peer.h"

const char *SERVER_HELP = R"(
Supported commands:
 - list
 - kill x
 - exit
)";

pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

std::map<FTPPeer, FTPServerUDP *> servers;

void *processingThread(void *data);

#endif //FTP_UDP_SERVER_H
