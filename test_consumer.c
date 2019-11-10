/*
	test_consumer.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "udp.h"
#include "string_to_can_consumer.h"

int main(int argc, char **argv)
{
	unsigned char received_string[2048];
	unsigned char buffer[8];
	uint32_t len = 0;
	uint32_t can_id;

	(void) argc;
	(void) argv;

	printf("\n\n");

	udp_init("localhost", 1234);

	while (1)
	{
		can_receive(&can_id, buffer, &len);

		if (can_receive_string_handle_message(can_id, buffer, len, received_string))
		{
			logger_log("String received: %s\n", received_string);
		}
	}

	printf("\n\n");

	return 0;
}
