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

int running = 1;
int counter = 0;

uint32_t local_can_id = 1;
uint32_t remote_can_id = 1;

unsigned char message_received[8];
char message_received_string[512];
uint32_t received_len = 0;
uint32_t received_id = 0;
uint8_t message_was_received = 0;

void message_receiver_init(void);
void get_next_message(unsigned char * message);
void message_to_string(unsigned char message[], char string[]);

void usage()
{
	printf("Invalid parameters.\n\n");
}

int main(int argc, char **argv)
{
	uint32_t period_ms = 1000;
	unsigned char message_sent[8];
	
	udp_init("localhost", 1234);
	message_receiver_init();

	printf("Starting listener\n");

	while (running)
	{
		// Produces new message and sends it to other DUT
		//udp_send(remote_can_id, message_sent, 8);

		// Update screen 
		if (message_was_received)
		{
			printf("Message received\n");
		}
		else
		{
			printf("No message received\n");
		}

		usleep(1000 * period_ms);
	}

	return 0;
}

void * message_receiver_thread_handler(void * args)
{
	(void) args;
	
	while (running) 
	{
		message_was_received = 
			udp_receive(&received_id, message_received, &received_len);

		if (received_id == local_can_id)
		{
			message_was_received = 1;
		}
	}

	return 0;
}


void message_receiver_init(void)
{
	pthread_t thread;
	pthread_create(&thread, NULL, message_receiver_thread_handler, 0);
}



