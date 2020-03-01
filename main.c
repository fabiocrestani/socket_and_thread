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
			printf(" Message was received. Buffer size is %d\n", buffer.element_counter);
		}
	}

	return 0;
}

// Receives one message from the circular buffer
void * message_handler_thread_handler(void * args)
{
	(void) args;
	
	while (running) 
	{
		buffer_wait_ready(&buffer);

		if (!buffer_is_empty(&buffer))
		{
			Message message;
			buffer_pop(&buffer, (void *) &message);
			logger_log_message("Message handler", message.data, message.len);
			buffer_set_ready(&buffer, 0);
			printf(" Message was handled. Buffer size is %d\n", buffer.element_counter);
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
	printf("Starting message handler\n");
	pthread_t thread;
	pthread_create(&thread, NULL, message_handler_thread_handler, 0);
}



