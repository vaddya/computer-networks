#ifndef SIMPLEFTPONUDP_READN_H
#define SIMPLEFTPONUDP_READN_H

#include <sys/types.h>
#include <sys/socket.h>

/**
 * Read {n} bytes from socket {s} into {buff}
 * @return the number read, if an error occurred returns m < n
 */
ssize_t readn(int s, char *buf, size_t n);

#endif //SIMPLEFTPONUDP_READN_H
