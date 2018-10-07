#ifndef SIMPLEFTPONTCP_READN_H
#define SIMPLEFTPONTCP_READN_H

#include <sys/types.h>
#include <sys/socket.h>

/**
 * Read {n} bytes from socket {s} into {buff}
 * @return the number read, if an error occurred returns m < n
 */
ssize_t readn(int s, char *buf, size_t n);

#endif //SIMPLEFTPONTCP_READN_H
