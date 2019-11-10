/*
	string_to_can_consumer.h
	Author: Fabio Crestani
*/

#ifndef __STRING_TO_CAN_CONSUMER_H__
#define __STRING_TO_CAN_CONSUMER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t can_receive_string_handle_message(uint32_t can_id, 
	unsigned char buffer[], uint32_t len, unsigned char received_string[]);

#endif // __STRING_TO_CAN_CONSUMER_H__
