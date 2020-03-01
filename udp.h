/*
	udp.h
	Author: Fabio Crestani
*/

#ifndef __UDP_H__
#define __UDP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "logger.h"

#define LOG_UDP_MESSAGES

uint8_t udp_init(char server_ip_address[], uint32_t port_number);
uint8_t udp_send(uint32_t can_id, unsigned char data[], uint32_t len);
uint8_t udp_receive(uint32_t *can_id, unsigned char data[], uint32_t *len);

#endif // __UDP_H__
