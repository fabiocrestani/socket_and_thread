/*
	test_producer.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>

#include "udp.h"
#include "gui.h"
#include "can.h"

int counter = 0;

void get_next_message(unsigned char * message);
void message_to_string(unsigned char message[], 
	char string[]);

void usage()
{
	wprintf(L"Invalid parameters.\n\n\
Usage: can_visual_ping_pong [dut] [l]\n\
  dut: 1 or 2\n\
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
	uint32_t can_id = 1;

	if ((argc > 1) && (strcmp(argv[1], "1") == 0))
	{
		dut = 1;
		can_id = 0x01;
	}
	else if ((argc > 1) && (strcmp(argv[1], "2") == 0))
	{
		dut = 2;
		can_id = 0x02;
	}
	else
	{
		usage();
		return 1;
	}

	if ((argc > 2) && (strcmp(argv[2], "l") == 0))
	{
		left = 1;
	}

	gui_init(dut, left);

	udp_init("localhost", 1234);

	unsigned char message[8];
	char message_string[512];

	while (1)
	{
		get_next_message(message);
		can_send(can_id, message, 8);
		// can_receive();

		gui_increment_pos();
		gui_set_last_message_received("hi");
		message_to_string(message, message_string);
		gui_set_last_message_sent(message_string);
		gui_update();
		sleep(2);
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


