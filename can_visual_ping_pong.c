/*
	test_producer.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include "udp.h"
#include "gui.h"

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

	if ((argc > 1) && (strcmp(argv[1], "1") == 0))
	{
		dut = 1;
	}
	else if ((argc > 1) && (strcmp(argv[1], "2") == 0))
	{
		dut = 2;
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

	//udp_init("localhost", 1234);

	while (1)
	{
		// can_send();
		// can_receive();
		gui_increment_pos();
		gui_set_last_message_received("hi");
		gui_set_last_message_sent("hello");
		gui_update();
		sleep(2);
	}

	return 0;
}
