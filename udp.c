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

uint8_t udp_init(char server_ip_address[], uint32_t port_number)
{
	uint32_t local_port_number = port_number;
	uint32_t remote_port_number = (port_number == 1235) ?
		(1236) : (1235);

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
	their_addr.sin_port = htons(remote_port_number);
	their_addr.sin_addr = *((struct in_addr *) he->h_addr);
	bzero(&(their_addr.sin_zero), 8);

	// Local server
	my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(local_port_number);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 8);

	if (bind(socket_udp, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) 
		== -1) 
	{
		return FALSE;
    }

	logger_log("UDP initialized. Local server port: %d \
Remote server address and port: %s:%d\n", 
local_port_number, server_ip_address, remote_port_number);

	return TRUE;
}

uint8_t udp_deinit(void)
{
	close(socket_udp);
	return TRUE;
}

uint8_t udp_send(unsigned char data[], uint32_t len)
{
	int numbytes = sendto(socket_udp, data, len, 0, 
				(struct sockaddr *) &their_addr, sizeof(struct sockaddr));

    if (numbytes == -1) 
	{
		logger_log("Error sending UDP message\n");
        return FALSE;
    }

	return TRUE;
}

uint8_t udp_receive(unsigned char data[], uint32_t *len)
{
	unsigned int addr_len = 0;

	int numbytes = recvfrom(socket_udp, data, UDP_RECEIVE_BUFFER_SIZE, 0, 
				(struct sockaddr *) &their_addr, &addr_len);
	if (numbytes == -1) 
	{
		return FALSE;
    }

	*len = numbytes;

#ifdef LOG_UDP_MESSAGES
	logger_log_message("Received UDP", data, numbytes);
#endif

	return TRUE;
}
