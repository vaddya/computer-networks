#ifndef SIMPLECLIENTSERVER_READN_H
#define SIMPLECLIENTSERVER_READN_H

#include <sys/types.h>
#include <sys/socket.h>

/**
 * Read {n} bytes from socket {s} into {buff}
 * @return the number read, if an error occurred returns m < n
 */
int readn(int s, char *buf, int n);

#endif //SIMPLECLIENTSERVER_READN_H
