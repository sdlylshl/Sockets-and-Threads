#include "decryption_server.h"

extern char done;
queue_t *q;

void perform_actions(int sock)
{
	send_handshake(sock);
	int hand_shaked = 0;
	char buffer[169] = {0};
	while(1)
	{
		memset(buffer,0,169);
		int x = receive_msg(sock,buffer);
		switch(x)
		{
			case HANDSHAKE_RESPONSE:
			{
				if(hand_shaked == 0)
				{
					hand_shaked = 1;
					//printf("Handshake response has arrived at server\n");
				}
				else
				{
					//printf("Duplicated handshake response has arrived at server\n");
				}
				break;	
			}
			case DECRYPTION_REQUEST:
			{
				if(!hand_shaked)
				{
					char aux[] = "Not hand_shaked request is denied";
					send_error_message(sock,aux,strlen(aux));
					return;
				}
				else
				{
					//printf("Decryption request has arrived at server\n");
					char buffer_aux[169];
					decrypt(buffer_aux, buffer+2*sizeof(int), strlen(buffer+2*sizeof(int)));
					send_response_message(sock, buffer_aux, strlen(buffer+2*sizeof(int)));
				}
				break;	
			}
			case END_OF_REQUEST:
			{
				if(!hand_shaked)
				{
					char aux[] = "Not hand_shaked end of request is meaningless";
					send_error_message(sock,aux,strlen(aux));
					return;
				}
				else
				{
					hand_shaked = 0;
					send_handshake(sock);
					//printf("End of request has arrived at server\n");
					return;
				}
				break;
			}
			case ERROR_MESSAGE:
			{
				printf("Error message has arrived on server\n");
				dump_payload(buffer);
				return;
				break;
			}
			default:
			{
				printf("Unknown %d msg type has arrived on server\n",x);
				char aux[] =  "Unknown msg type has arrived on client";
				send_error_message(sock,aux,strlen(aux));
				return;
				break;
			}
		}
	} // End while
}

void *service_handler(void *d)
{
	int tid = *(int*) d;
	char cid[100]; cid[0] = '\0';
	//char buffer[160];
	while(1)
	{
		char *client_description = NULL;
		client_description = queue_pop(q);
		if(client_description != NULL)
		{
			int client_sock = get_sock_fd(client_description);

			get_ip_string(cid,client_description);
			int portno = get_port_string(client_description);
			
			printf("Thread %d is handling client %s,%d\n",tid,cid,portno);
			
			// Do hard work here!
			perform_actions(client_sock);
			
			close(client_sock);
			free(client_description);
			
			printf("Thread %d finished handling client %s,%d\n",tid,cid,portno);
		}
	}
	return NULL;
}

int main(int argc, char **argv)
{
	queue_init(&q,3000);
	int numThreads = 5;
	if(argc < 2)
	{
		printf("Usage: ./decryption_server <PORTNO> [numThreads]\n");
		return 0;
	}
	if(argc == 3)
	{
		numThreads = atoi(argv[2]);
		if(numThreads < 1)
		{
			printf("You must specify a number of threads greater than 0.\nThe server will run with default thread number\n");
			numThreads = 5;
		}
	}
	// Launching #numThreads threads.
	pthread_t sniffer_threads[numThreads];
	for(int i = 0; i < numThreads; i++)
	{
		int tid = i; // Just to be more explicit.
		if( pthread_create( &(sniffer_threads[i]) , NULL ,  service_handler , (void*) &tid) < 0)
        {
            perror("could not create thread");
            return 1;
        }
	}
	int socket_desc, client_sock, c;
	struct sockaddr_in server, client;
	// Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc < 0)
	{
		perror("Could not create socket\n");
		return 1;
	}
	//printf("Socket created\n");
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(argv[1]));
	// set SO_REUSEADDR on a socket to true (1):
	int optval = 1;
	setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) 
	{
		perror("ERROR on binding");
		exit(0);
	}
	// Listen
	if (listen(socket_desc, 5) < 0) 
	{
		/* error handling */
		perror("listen failure");
	}
	// Accept an incoming connection
	// printf("Waiting for incoming connections...\n");
	while(1)
	{
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if(client_sock < 0)
		{
			perror("accept failed\n");
			return 1;
		}
		// assume s is a connected socket
		int s = client_sock;
		socklen_t len;
		struct sockaddr_storage addr;
		char ipstr[INET6_ADDRSTRLEN];
		int port;

		len = sizeof addr;
		getpeername(s, (struct sockaddr*)&addr, &len);
		// deal with both IPv4 and IPv6:
		if (addr.ss_family == AF_INET) 
		{
			struct sockaddr_in *s = (struct sockaddr_in *)&addr;
			port = ntohs(s->sin_port);
			inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
		}
		else
		{ // AF_INET6
			struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
			port = ntohs(s->sin6_port);
			inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
		}
		
		char string_temp[200];
		sprintf(string_temp,"%d %s %d",client_sock,ipstr,port);
		
		queue_push(q,string_temp);
	}
	// Main thread runs forever.
	// done = 1;
	// clean_up(q);
	return 0;	
}
