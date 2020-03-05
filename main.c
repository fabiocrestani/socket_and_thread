/*
	main.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "udp.h"
#include "circular_buffer.h"

int running = 1;
CircularBuffer buffer;

void message_receiver_init(void);
void message_handler_init(void);

#define MAX_MESSAGE_DATA_LEN (256)

typedef struct {
	unsigned char data[MAX_MESSAGE_DATA_LEN];
	uint32_t len;
} Message;

void usage()
{
	printf("Invalid parameters.\n\n");
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	uint32_t period_ms = 1000;

	buffer_init(&buffer, sizeof(Message));
	udp_init("localhost", 1234);
	message_receiver_init();
	message_handler_init();
	
	while (running)
	{
		usleep(1000 * period_ms);
	}

	udp_deinit();
	buffer_denit(&buffer);

	return 0;
}

// Receives one message from UDP and stores it into a circular buffer
void * message_receiver_thread_handler(void * args)
{
	(void) args;
	unsigned char received_data[MAX_MESSAGE_DATA_LEN];
	uint32_t received_len = 0;
	uint8_t message_was_received = 0;
	
	while (running) 
	{
		message_was_received = 
			udp_receive(received_data, &received_len);
	
		if (message_was_received)
		{
			Message message;
			message.len = received_len;
			memcpy(message.data, received_data, received_len);
			buffer_put(&buffer, (void *) &message);
			buffer_set_ready(&buffer, 1);
			printf(" Message was received. Buffer size is %d %s\n", 
				buffer.element_counter, 
				buffer.element_counter == buffer.buffer_size ? 
				"Buffer is full!" : "");
		}
	}

	return 0;
}

// Receives one message from the circular buffer
void * message_handler_thread_handler(void * args)
{
	int thread_id = *((int *) args);
	free(args);
	int counter_messages = 0;

	printf("Starting message handler %d\n", thread_id);

	while (running) 
	{
		if (buffer_is_empty(&buffer))
		{
			buffer_wait_ready(&buffer);
		}
		else
		{
			Message message;
			buffer_pop(&buffer, (void *) &message);
			logger_log_message("Message handler", message.data, message.len);
			buffer_set_ready(&buffer, 0);
			counter_messages++;
			printf(" Message was handled by handler %d (total: %d). Buffer size is %d\n",
				thread_id, counter_messages, buffer.element_counter);

			// Dummy delay, for testing only
			//usleep(1000 * 100);
		}
	}

	return 0;
}

void message_receiver_init(void)
{
	printf("Starting message receiver\n");
	pthread_t thread;
	pthread_create(&thread, NULL, message_receiver_thread_handler, 0);
}

void message_handler_init(void)
{
	const int num_of_threads = 3;
	pthread_t thread_message_handler[num_of_threads];

	for (int i = 0; i < num_of_threads; i++)
	{
		int * index = malloc(sizeof(* index));
		*index = i;
		pthread_create(&thread_message_handler[i], NULL, 
			message_handler_thread_handler, index);
	}
}



