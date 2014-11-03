#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //write
#include <netdb.h> // gethostbyname

#define EXTRACT_FILE_LEN(client) ((strrchr(client, '/')) ? strlen(strrchr(client, '/')) : strlen(client))

#define HANDSHAKE 100
#define HANDSHAKE_RESPONSE 101
#define DECRYPTION_REQUEST 102
#define RESPONSE_MESSAGE 103
#define END_OF_REQUEST 104
#define ERROR_MESSAGE 105

void send_string(int sock, char *data);
void read_string(int sock, char *data);

// Simetric
int receive_msg(int sock, char *data);
void send_error_message(int sock, char *data, int len);

// Server side
void send_handshake(int sock);
void send_response_message(int sock, char *data, int len);

// Client side
void send_handshake_response(int sock);
void send_decryption_request(int sock, char *data, int len);
void send_end_of_request(int sock);
#endif
