/*
 * queue.h
 *
 *  Created on: Apr 5, 2014
 *      Author: Abhijeet
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define ERR_Q_FULL 2

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //write
#include <pthread.h>

typedef struct queue_node {
	char* client;
	struct queue_node *next;
} queue_node_t;

typedef struct queue {
	queue_node_t *head;
	queue_node_t *tail;
	//these sizes are bounded by Maximum an Int can contain. This is assumed.
	int size; //size of queue 
	int max_size; //MAX queue size if bounded.
} queue_t;

int print_queue(queue_t *q);
void queue_init(queue_t **q, int max_size);
int queue_push(queue_t *q, char* client);
char* queue_pop(queue_t *q);
void set_done();
void clean_up(queue_t *q);

#endif /* QUEUE_H_ */
