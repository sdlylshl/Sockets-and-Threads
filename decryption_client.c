#include "protocol.h"
#include "util.h"

struct timespec ts_start;
struct timespec ts_end;
double s_elapsed;

void perform_actions(int sock,char *input_file)
{
	char buffer[169] = {0};
	
	char output_file[300] = {0};
	
    strcpy(output_file,input_file);
    strcat(output_file,".decrypted");
    //printf("Output_file: %s\n",output_file);
    
	while(1)
	{	
		memset(buffer,0,169);
		int x = receive_msg(sock,buffer);
		switch(x)
		{
			case HANDSHAKE:
			{
				//printf("Server handshake has arrived on client\n");
				send_handshake_response(sock);
				
				FILE *fd = fopen(input_file,"r");
				FILE *foutput = fopen(output_file,"w");
				
				// Here come all the requests.
				while(read_line(fd, buffer))
				{
					//printf("%s\n",buffer);
					send_decryption_request(sock,buffer,strlen(buffer));
					x = receive_msg(sock,buffer);
					if(x != RESPONSE_MESSAGE)
					{
						perror("Wrong server response");
						break;
					}
					int msg_len = *((int *)(buffer+sizeof(int)));
					for(int i = 0; i < msg_len; i++)
					{
						// printf("%c",buffer[i+2*sizeof(int)]);
						fprintf(foutput,"%c",buffer[i+2*sizeof(int)]);
					} fprintf(foutput,"\n");
				}
				
				fclose(foutput);
				fclose(fd);
				send_end_of_request(sock);
				return;
				break;
			}
//			case RESPONSE_MESSAGE:
//			{
//				printf("Server response message has arrived on client\n");
//				dump_payload(buffer);
//				break;
//			}
			case ERROR_MESSAGE:
			{
				printf("Error message has arrived on client\n");
				dump_payload(buffer);
				return;
				break;
			}
			default:
			{
				printf("Unknown %d msg type has arrived on client\n",x);
				char aux[] = "Unknown msg type has arrived on client";
				send_error_message(sock,aux,strlen(aux));
				return;
				break;
			}
		}
	} // End while
}

int main(int argc, char **argv)
{
	
    
    if(argc < 4)
    {
    	printf("Usage: ./decryption_client [ServerHost] [ServerPort] [FilePath]\n"); 
    	return 0;
    }
    

    
    //printf("argc %d\n",argc);
    
    
    
    
	struct sockaddr_in serv_addr;
	
	for(int k = 0; k < argc-3; k++)
	{
		int sockfd[argc-3], portno;
		struct hostent *server;
		char host[30];
    
	    strcpy(host,argv[1]);
		portno = atoi(argv[2]);
		char input_file[300];
	
		
		strcpy(input_file,argv[3+k]);
		
		sockfd[k] = socket(AF_INET, SOCK_STREAM, 0);
		
		if (sockfd[k] < 0) 
		{	
			char err_msg[30] = "ERROR opening socket";
		    perror(err_msg);
		}
		
		server = gethostbyname(host);
		
		if (server == NULL) {
		    fprintf(stderr,"ERROR, no such host\n");
		    exit(0);
		}
		
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		memcpy
		(
			(char *)&serv_addr.sin_addr.s_addr,
			(char *)server->h_addr,
		     server->h_length
		);
		serv_addr.sin_port = htons(portno);
		
		int k = connect(sockfd[k],(struct sockaddr *)&serv_addr,sizeof(serv_addr));

		if (k < 0) 
		{	
			char err_msg[30] = "ERROR connecting";
		    perror(err_msg);
		}	
	
		perform_actions(sockfd[k],input_file);
	
	
		close(sockfd[k]);
	}
    
	return 0;
}
