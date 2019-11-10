/*
	can.h
	Author: Fabio Crestani
*/

#ifndef __CAN_H__
#define __CAN_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "logger.h"

//#define LOG_CAN_MESSAGES

uint8_t can_send(uint32_t can_id, unsigned char data[], uint32_t len);
uint8_t can_receive(uint32_t *can_id, unsigned char data[], uint32_t *len);

#endif // __CAN_H__
