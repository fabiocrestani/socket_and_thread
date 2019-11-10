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
#include "string_to_can_producer.h"
#include "gui.h"

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "");
	fwide(stdout, 1);

	uint32_t can_id = 0x62;
	char input[] = "sudo date --set \"20190521 20:54\"";
	char left = 0;

	if ((argc > 1) && (strcmp(argv[1], "l") == 0))
	{
		left = 1;
	}

	gui_init(left);
	gui_update();

	return 0;

	udp_init("localhost", 1234);

	if (argc > 1)
	{
		logger_log("Sending string \"%s\" to CAN ID 0x%02x\n", argv[1], can_id);
		can_send_string(can_id, argv[1]);
	} 
	else 
	{
		logger_log("Sending string \"%s\" to CAN ID 0x%02x\n", input, can_id);
		can_send_string(can_id, input);
	}	

	return 0;
}
