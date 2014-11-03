CC=gcc
CFLAGS=-g -Wall -std=gnu99 -o
LIBS=-lpthread

all: client_server

client_server: decryption_server.o decryption_client.o

decryption_server.o: decryption_server.c
	$(CC) $(CFLAGS) decryption_server decryption_server.c queue.c protocol.c util.c $(LIBS)

decryption_client.o: decryption_client.c
	$(CC) $(CFLAGS) decryption_client decryption_client.c protocol.c util.c $(LIBS)

clean:
	rm -rf *o decryption_server
	rm -rf *o decryption_client
