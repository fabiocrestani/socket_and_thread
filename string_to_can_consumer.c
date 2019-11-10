/*
	string_to_can_consumer.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "string_to_can_consumer.h"
#include "string_to_can.h"
#include "logger.h"
#include "crc.h"

unsigned char st_string[STRING_TO_CAN_MAX_STRING_SIZE];
uint32_t st_len;
uint32_t st_received_len;
uint32_t st_received_packets;
uint32_t st_total_packets;

uint8_t can_receive_string_start(unsigned char buffer[]);
uint8_t can_receive_string_continue(unsigned char buffer[], uint32_t len);
uint8_t can_receive_string_stop(unsigned char buffer[]);

uint8_t can_receive_string_handle_message(uint32_t can_id, 
			unsigned char buffer[], uint32_t len, 
			unsigned char received_string[])
{
	(void) can_id;

	uint8_t opcode = buffer[0];
	uint8_t ret = 0;

	if (len > 8) return FALSE;

	switch (opcode)
	{
		case STRING_TO_CAN_OPCODE_START:
			can_receive_string_start(buffer);
			return FALSE;

		case STRING_TO_CAN_OPCODE_CONTINUE:
			can_receive_string_continue(buffer, len);
			return FALSE;

		case STRING_TO_CAN_OPCODE_STOP:
			ret = can_receive_string_stop(buffer);
			strcpy((char *) received_string, (char *) st_string);
			return ret;

		default:
			logger_log("Unknow opcode: %d\n", opcode);
			return FALSE;
	}
}

uint8_t can_receive_string_start(unsigned char buffer[])
{
	st_received_packets = 1;
	st_received_len = 0;
	st_string[0] = 0;

	st_len = (buffer[7] & 0xFF) | ((buffer[6] & 0xFF) << 8) | 
			((buffer[5] & 0xFF) << 16) | ((buffer[4] & 0xFF) << 24);

	st_total_packets = 2 + ((st_len + 7 - 1)/ 7.0);

	//logger_log("Expected len is %d. Total packets: %d\n", st_len, 
			//st_total_packets);

	return TRUE;
}

uint8_t can_receive_string_continue(unsigned char buffer[], uint32_t len)
{
	for (uint32_t i = 0; i < len - 1; i++)
	{
		st_string[st_received_len++] = buffer[i + 1];

		if (st_received_len >= st_len)
		{
			break;
		}
	}

	st_received_packets++;

#ifdef STRING_TO_CAN_LOG_PROGRESS
	logger_log("Receving packet %d / %d\n", st_received_packets,
			st_total_packets);
#endif

	return TRUE;
}

uint8_t can_receive_string_stop(unsigned char buffer[])
{
	int crc = 0;

	st_string[st_received_len] = 0;
	
	crc = crc_16(st_string, st_received_len);

	//logger_log("String received. Expected len: %d Received len: %d\n
//Expected CRC: 0x%02x 0x%02x Received CRC: 0x%02x 0x%02x\n\n",
//			st_len, st_received_len, (crc & 0xff), ((crc >> 8) & 0xff),
//			buffer[7], buffer[6]);

	if (((crc & 0xFF) == buffer[7]) && 
	   (((crc >> 8) & 0xFF) == buffer[6]))
	{
		return TRUE;
	}
	else
	{
		logger_log("Invalid CRC!\n");
		return FALSE;
	}
}

