/*
	can_visual_send_receive.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <locale.h>
#include <wchar.h>

#include "udp.h"
#include "gui.h"
#include "can.h"

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
void message_to_string(unsigned char message[], 
	char string[]);

void usage()
{
	wprintf(L"Invalid parameters.\n\n\
Usage: can_visual_ping_pong [dut] [period] [l]\n\
  dut: 1 or 2\n\
  period: period in ms to send messages\n\
  l (optional): to set the visualization on the left side of the terminal\n\n\
DUT 1 will send a counter with CAN ID 0x01\n\
DUT 1 will send a counter with CAN ID 0x02\n\
Default period is 2 seconds\n");
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	fwide(stdout, 1);

	uint8_t left = 0;
	uint8_t dut = 1;
	uint32_t period_ms = 1000;
	
	if ((argc > 1) && (strcmp(argv[1], "1") == 0))
	{
		dut = 1;
		local_can_id = 0x01;
		remote_can_id = 0x02;
	}
	else if ((argc > 1) && (strcmp(argv[1], "2") == 0))
	{
		dut = 2;
		local_can_id = 0x02;
		remote_can_id = 0x01;
	}
	else
	{
		usage();
		return 1;
	}

	if (argc > 2)
	{
		period_ms = (atoi(argv[2]) > 10) ? atoi(argv[2]) : 1000;
	}

	if ((argc > 3) && (strcmp(argv[3], "l") == 0))
	{
		left = 1;
	}

	gui_init(dut, left);
	udp_init("localhost", 1234 + dut);
	message_receiver_init();

	unsigned char message_sent[8];
	char message_sent_string[512];

	wprintf(L"Starting with DUT: %d left: %d period: %d \
local_can_id: %d remote_can_id: %d\n",
		dut, left, period_ms, local_can_id, remote_can_id);

	while (running)
	{
		// Produces new message and sends it to other DUT
		get_next_message(message_sent);
		can_send(remote_can_id, message_sent, 8);

		// Update screen 
		if (message_was_received)
		{
			gui_increment_pos();
			message_to_string(message_received, message_received_string);
			gui_set_last_message_received(message_received_string);
			message_was_received = 0;
		}
	
		message_to_string(message_sent, message_sent_string);
		gui_set_last_message_sent(message_sent_string);

		gui_update();
		usleep(1000 * period_ms);
	}

	return 0;
}

void get_next_message(unsigned char * message)
{
	counter++;	

	message[7] = rand() % 255;
	message[6] = rand() % 255;
	message[5] = rand() % 255;
	message[4] = rand() % 255;
	message[3] = (counter >> 24) & 0xFF;
	message[2] = (counter >> 16) & 0xFF;
	message[1] = (counter >> 8) & 0xFF;
	message[0] = (counter >> 0) & 0xFF;
}

void message_to_string(unsigned char message[], 
	char string[])
{
	sprintf(string, "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", 
		message[7], message[6], message[5], message[4], 
		message[3], message[2], message[1], message[0]);
}

void * message_receiver_thread_handler(void * args)
{
	(void) args;
	
	while (running) 
	{
		message_was_received = 
			can_receive(&received_id, message_received, &received_len);
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



