#include "protocol.h"
#include "util.h"

void send_string(int sock, char *data)
{
	int n;
	int bytes_count = 0;
	char buffer[200]; strcpy(buffer,data);
	
	while(bytes_count < strlen(buffer)+1)
	{
		bytes_count += (n = write(sock , buffer+bytes_count , strlen(buffer+bytes_count)+1));
	}
	//printf("Data of size %d sent: %s\n",n,data);
}

void read_string(int sock, char *data)
{
	char buffer[169] = {0}; // 32/8 ID bytes + 32/8 LENGHT bytes bits + 160 PAYLOAD bytes + 1 '\0' bytes
	char aux = '.';
	int byte_count = 0;
	int n = 0;
	
	while(aux != '\0')
	{
		byte_count += (n = recv(sock, &aux, 1, 0));
		if(n)
		{
			buffer[byte_count-1] = aux;
		}

	}
	strcpy(data,buffer);
}


// SIMETRIC FUNCTIONS

int receive_msg(int sock, char *data)
{
	char buffer[169] = {0}; // 32/8 ID bytes + 32/8 LENGHT bytes bits + 160 PAYLOAD bytes + 1 '\0' bytes
	char aux = '.';
	int bytes_count = 0;
	int n = 0;
	
	while(bytes_count < 169)
	{
		bytes_count += (n = recv(sock, &aux, 1, 0));
		if(n)
		{
			buffer[bytes_count-1] = aux;
		}
	}
	
	int type = *((int *)buffer);
	int size = *((int *)(buffer+sizeof(int)));
	
	
	memcpy(data,buffer,8+size);
	
	return type;
}

void send_error_message(int sock, char *data, int len)
{
	int code = ERROR_MESSAGE;
	
	char buffer[169] = {0};
	
	*((int *)buffer) = code;
	*((int *)(buffer+sizeof(int))) = len;
	memcpy(buffer+8,data,len);
	
	int byte_count = 0;
	int n = 0;
	
	while(byte_count < 169)
	{
		byte_count += (n = write(sock, buffer+byte_count, 169-byte_count));
	}
}

// SERVER FUNCTIONS
void send_handshake(int sock)
{
	int code = HANDSHAKE;
	int len = 0;
	
	char buffer[169] = {0};
	
	*((int *)buffer) = code;
	*((int *)(buffer+sizeof(int))) = len;
	
	int byte_count = 0;
	int n = 0;
	
	while(byte_count < 169)
	{
		byte_count += (n = write(sock, buffer+byte_count, 169-byte_count));
	}
}

void send_response_message(int sock, char *data, int len)
{
	int code = RESPONSE_MESSAGE;
	
	char buffer[169] = {0};
	
	*((int *)buffer) = code;
	*((int *)(buffer+sizeof(int))) = len;
	memcpy(buffer+8,data,len);
	
	int byte_count = 0;
	int n = 0;
	
	while(byte_count < 169)
	{
		byte_count += (n = write(sock, buffer+byte_count, 169-byte_count));
	}
}


// Client functions

void send_handshake_response(int sock)
{
	int code = HANDSHAKE_RESPONSE;
	int len = 0;
	
	char buffer[169] = {0};
	
	*((int *)buffer) = code;
	*((int *)(buffer+sizeof(int))) = len;
	
	int byte_count = 0;
	int n = 0;
	
	while(byte_count < 169)
	{
		byte_count += (n = write(sock, buffer+byte_count, 169-byte_count));
	}
}

void send_decryption_request(int sock, char *data, int len)
{
	int code = DECRYPTION_REQUEST;
	
	char buffer[169] = {0};
	
	*((int *)buffer) = code;
	*((int *)(buffer+sizeof(int))) = len;
	memcpy(buffer+8,data,len);
	
	int byte_count = 0;
	int n = 0;
	
	while(byte_count < 169)
	{
		byte_count += (n = write(sock, buffer+byte_count, 169-byte_count));
	}
}

void send_end_of_request(int sock)
{
	int code = END_OF_REQUEST;
	int len = 0;
	
	char buffer[169] = {0};
	
	*((int *)buffer) = code;
	*((int *)(buffer+sizeof(int))) = len;
	
	int byte_count = 0;
	int n = 0;
	
	while(byte_count < 169)
	{
		byte_count += (n = write(sock, buffer+byte_count, 169-byte_count));
	}
}
