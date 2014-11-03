#include "util.h"

void decrypt(char *dest, char *source, int len)
{
	for(int i = 0; i < len; i++)
	{
		char c = source[i];
		
		if (c == 'y' || c == 'z' || c == 'Y' || c == 'Z') {
			dest[i] = c - 24;
		} else if (('a' <= c && 'x' >= c) || (('A' <= c && 'X' >= c))) {
			dest[i] = c + 2;
		} else {
			dest[i] = c;
		}
	}
}

int read_line(FILE *fd, char *buffer)
{
	
	int n = fscanf(fd,"%[^\n]s",buffer);
	char lixo;
	
	if(n < 1)
	{
		return 0;
	}
	else
	{
		fscanf(fd,"%c",&lixo);
		
		return n;
	}
}

void dump_payload(char *data)
{
	if(!data) return;
	if(!data[0]) return;
	int code = *((int *)data);
	int len = *((int *)(data+sizeof(int)));
	
	printf("%d %d [",code,len);
	for(int i = 0; i < len; i++)
	{
		printf("%c",data[i+8]);
	}
	printf("]\n");
}

int get_port_string(char *src)
{
	if(!src) perror("Invalid string");
	char port_str[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int i = 0;
	int j = 0;
	int count = 0;
	for(i = 0; i < strlen(src); i++)
	{	
		if(src[i] == ' ')
		{
			count++;
		}
		else if(count == 2)
		{
			port_str[j++] = src[i];
		}
		port_str[j] = '\0';
	}
	return atoi(port_str);
}

void get_ip_string(char* dest,char *src)
{
	if(!src) perror("Invalid string");
	int i = 0;
	int j = 0;
	while(src[i] != ' ')
	{
		i++;
	}
	i++;
	while(src[i] != ' ')
	{
		dest[j++] = src[i++];
	}
	dest[j] = '\0';
}

int get_sock_fd(char *src)
{
	char dest[30];
	if(!src) perror("Invalid string");
	int i = 0;
	int j = 0;
	while(src[i] != ' ')
	{
		dest[j++] = src[i++];
	}
	dest[j] = '\0';
	return atoi(dest);
}
