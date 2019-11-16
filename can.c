/*
	can.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <locale.h>
#include <wchar.h>

#define TRUE (1)
#define FALSE (0)

#include "can.h"
#include "udp.h"
#include "logger.h"

int socket_udp;
struct sockaddr_in their_addr;
struct sockaddr_in my_addr;
struct hostent *he;

uint8_t can_send(uint32_t can_id, unsigned char data[], uint32_t len)
{
#ifdef LOG_CAN_MESSAGES
	logger_log("Sending CAN (0x%02x): 0x%02x 0x%02x 0x%02x \
0x%02x 0x%02x 0x%02x 0x%02x 0x%02x (%d)\n", can_id, data[0], data[1], 
data[2], data[3], data[4], data[5], data[6], data[7], len);
#endif

	return udp_send(can_id, data, len);
}

uint8_t can_receive(uint32_t *can_id, unsigned char data[], uint32_t *len)
{
	uint8_t ret = udp_receive(can_id, data, len);

#ifdef LOG_CAN_MESSAGES
	logger_log("data UDP message:\n\t\t0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \
 0x%02x 0x%02x 0x%02x (%s) \t (%d)\n", 
	data[0], data[1], data[2], data[3], data[4], data[5], 
	data[6], data[7], data, *len);
#endif

	return ret;
}

