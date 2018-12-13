#ifndef SIMPLEFTPONUDP_FTP_CLIENT_H
#define SIMPLEFTPONUDP_FTP_CLIENT_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ftp_client_udp.h"

const int PORT = 7000;

const char *CLIENT_HELP = R"(
Supported commands:
 - connect
 - pwd
 - ls
 - cd
 - get
 - put
 - disconnect
 - exit
)";

#endif //SIMPLEFTPONUDP_FTP_CLIENT_H
