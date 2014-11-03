/*
 * queue
 *
 *      Author: Abhijeet
 */

#include "queue.h"

static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t queue_full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t queue_empty = PTHREAD_COND_INITIALIZER;
char done;

// For debug only
int print_queue(queue_t *q)
{	
	if (pthread_mutex_lock(&queue_mutex)) {
		return 0;
	}
	for(queue_node_t *temp = q->head; temp; temp = temp->next)
	{
		printf("%s\n",temp->client);
	}
	pthread_mutex_unlock(&queue_mutex);
	return 1;
}

void queue_init(queue_t **q, int max_size) {

	*q = (queue_t *) calloc(1, sizeof(queue_t));
	if (NULL == *q) {
		perror("ERROR");
		return;
	}

	(*q)->head = NULL;
	(*q)->tail = NULL;
	(*q)->max_size = max_size;
}

//1: error, 0: successful
int queue_push(queue_t *q, char* client) {

	char bool = 0;
	queue_node_t *temp;

	if (pthread_mutex_lock(&queue_mutex)) {
		return 1;
	}

	if (q->max_size) {
		while (q->size == q->max_size) {
			if (!bool){
				printf("INFO: Queue full. Waiting to add more clients.\n");
				bool = 1;
			}
			pthread_cond_wait(&queue_full, &queue_mutex);
		}
		++(q->size);
	}

	temp = (queue_node_t *) calloc(1, sizeof(queue_node_t));

	//+1 for NULL char, calloc initializes all to '\0'!
	temp->client = (char *) calloc(strlen(client) + 1, sizeof(char));
	if (NULL == temp->client) {
		perror("ERROR");
		pthread_mutex_unlock(&queue_mutex);
		return 1;
	}

	strcpy(temp->client, client);
	temp->next = NULL;

	if (NULL == q->head) {
		q->head = q->tail = temp;
	} else {
		q->tail->next = temp;
		q->tail = temp;
	}

	pthread_cond_signal(&queue_empty);
	pthread_mutex_unlock(&queue_mutex);
	return 0;
}

//TODO: poor err handling, a bit tricky
char* queue_pop(queue_t *q) {

	queue_node_t *temp;
	char* ret_client = NULL;

	pthread_mutex_lock(&queue_mutex);

	while (NULL == q->head && !done) {
		pthread_cond_wait(&queue_empty, &queue_mutex);
	}

	if (NULL == q->head) {
		//this was a 'done' from main thread
		//exit strategy
		pthread_mutex_unlock(&queue_mutex);
		pthread_cond_broadcast(&queue_empty);
		return NULL;
	}

	temp = q->head;
	ret_client = (char *) calloc(strlen(temp->client) + 1, sizeof(char));
	if (NULL == ret_client) {
		perror("ERROR");
		pthread_mutex_unlock(&queue_mutex);
		return NULL;
	}

	//TODO: check err
	strcpy(ret_client, temp->client);

	q->head = q->head->next;
	free(temp);

	if (q->max_size) {
		--(q->size);
	}

	pthread_cond_signal(&queue_full);
	pthread_mutex_unlock(&queue_mutex);
	return ret_client;
}

void set_done() {
	pthread_mutex_lock(&queue_mutex);
	done = 1;
	pthread_cond_broadcast(&queue_empty);
	pthread_mutex_unlock(&queue_mutex);
}

void clean_up(queue_t *q) {
	while (queue_pop(q));
	free(q);
}
