/*
	udp.c
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

#define TRUE (1)
#define FALSE (0)

#define UDP_RECEIVE_BUFFER_SIZE (512)

#include "udp.h"
#include "logger.h"

int socket_udp;
struct sockaddr_in their_addr;
struct sockaddr_in my_addr;
struct hostent *he;

uint8_t can_send(uint32_t can_id, unsigned char data[], uint32_t len)
{
#ifdef LOG_CAN_MESSAGES
	logger_log("Sending CAN message (id 0x%02x):\n\t\t0x%02x 0x%02x 0x%02x \
0x%02x 0x%02x 0x%02x 0x%02x 0x%02x (%s) \t (%d)\n", can_id, data[0], data[1], 
data[2], data[3], data[4], data[5], data[6], data[7], data, len);
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

// ---------------------------------------------------------------------------

uint8_t udp_init(char server_ip_address[], uint32_t port_number)
{
	if ((he = gethostbyname(server_ip_address)) == NULL)
	{
		return FALSE;
	}

	if ((socket_udp = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{
		return FALSE;
	}

	// Remote client
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(port_number);
	their_addr.sin_addr = *((struct in_addr *) he->h_addr);
	bzero(&(their_addr.sin_zero), 8);

	// Local server
	my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(1234);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 8);

	if (bind(socket_udp, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) 
		== -1) 
	{
		return FALSE;
    }

	//logger_log("UDP initialized on %s:%d\n", server_ip_address, port_number);

	return TRUE;
}

uint8_t udp_deinit(void)
{
	close(socket_udp);
	return TRUE;
}

uint8_t udp_send(uint32_t can_id, unsigned char data[], uint32_t len)
{
	unsigned char buffer[256];
	
	buffer[0] = (unsigned char) can_id;
	buffer[1] = (unsigned char) len;
	
	for (uint32_t i = 0; i < len; i++)
	{
		buffer[i + 2] = data[i];
	}

	len = len + 2;

#ifdef LOG_UDP_MESSAGES
	logger_log("Sending UDP message:\n\t\t0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\
 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x (%s) \t (%d)\n", 
		buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], 
		buffer[6], buffer[7], buffer[8], buffer[9], data, len);
#endif

	int numbytes = sendto(socket_udp, buffer, len, 0, 
				(struct sockaddr *)&their_addr, sizeof(struct sockaddr));

    if (numbytes == -1) 
	{
		logger_log("Error sending UDP message\n");
        return FALSE;
    }

	return TRUE;
}

uint8_t udp_receive(uint32_t *can_id, unsigned char data[], uint32_t *len)
{
	unsigned char buffer[UDP_RECEIVE_BUFFER_SIZE];
	unsigned int addr_len = 0;

	int numbytes = recvfrom(socket_udp, buffer, UDP_RECEIVE_BUFFER_SIZE, 0, 
				(struct sockaddr *) &their_addr, &addr_len);
	if (numbytes == -1) 
	{
		return FALSE;
    }

    buffer[numbytes] = '\0';

#ifdef LOG_UDP_MESSAGES
	logger_log("Received UDP message:\n\t\t0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \
0x%02x 0x%02x 0x%02x 0x%02x 0x%02x (%s) \t (%d)\n", 
		buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], 
		buffer[6], buffer[7], buffer[8], buffer[9], buffer, numbytes);
#endif

	*can_id = buffer[0];
	*len = buffer[1];
	memset(data, 0, 8);
	for (unsigned int i = 0; i < *len; i++)
	{
		data[i] = buffer[i + 2];
	}

	return FALSE;
}
