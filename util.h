#ifndef UTIL_H
#define UTIL_H

#include "queue.h"

void decrypt(char *dest, char *source, int len);

int read_line(FILE *fd, char *buffer);

void dump_payload(char *payload);

int get_port_string(char *src);

void get_ip_string(char* dest,char *src);

int get_sock_fd(char *src);

#endif /* util.h */
