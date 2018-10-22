#ifndef SIMPLEFTPONUDP_FTP_CLIENT_H
#define SIMPLEFTPONUDP_FTP_CLIENT_H

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

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ftp_client.h"

#endif //SIMPLEFTPONUDP_FTP_CLIENT_H
