/*
	string_to_can_producer.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "string_to_can.h"
#include "logger.h"
#include "udp.h"
#include "crc.h"

uint8_t can_send_string_start(uint32_t can_id, uint32_t len);
uint8_t can_send_string_continue(uint32_t can_id, char input[], uint32_t len);
uint8_t can_send_string_stop(uint32_t can_id, char input[], uint32_t len);

uint8_t can_send_string(uint32_t can_id, char input[])
{
	uint32_t len = strlen(input);
	char temp[7];

	if (len >= STRING_TO_CAN_MAX_STRING_SIZE)
	{
		logger_log("Error: String is too big! (%d bytes)", len);
		return FALSE;
	}

	uint32_t sublen = 0;
#ifdef STRING_TO_CAN_LOG_PROGRESS
	uint32_t sent_packets = 0;
	uint32_t total_packets = 2 + ((len + 7 - 1)/ 7.0);
#endif

	//logger_log("Preparing string: %s\n", input);
	//logger_log("Will split string in %d packets\n", total_packets);

	if (!can_send_string_start(can_id, len)) 
	{
		return FALSE;
	}
#ifdef STRING_TO_CAN_LOG_PROGRESS
	logger_log("Packet %d / %d sent\n", ++sent_packets, total_packets);
#endif

	for (uint32_t i = 0; i < len; i++)
	{
		temp[sublen++] = input[i];

		if (sublen >= STRING_TO_CAN_PAYLOAD_CAN_SIZE)
		{
			if (!can_send_string_continue(can_id, temp, sublen))
			{
				return FALSE;
			}
			sublen = 0;
#ifdef STRING_TO_CAN_LOG_PROGRESS
			logger_log("Packet %d / %d sent\n", ++sent_packets, total_packets);
#endif
		}
	}

	if (sublen > 0)
	{
		if (!can_send_string_continue(can_id, temp, sublen))
		{
			return FALSE;
		}
#ifdef STRING_TO_CAN_LOG_PROGRESS
		logger_log("Packet %d / %d sent\n", ++sent_packets, total_packets);
#endif
	}
	
	if (!can_send_string_stop(can_id, input, len))
	{
		return FALSE;
	}

#ifdef STRING_TO_CAN_LOG_PROGRESS
	logger_log("Packet %d / %d sent\n", ++sent_packets, total_packets);
#endif

	return TRUE;

}

uint8_t can_send_string_start(uint32_t can_id, uint32_t len)
{
	char data[8] = {STRING_TO_CAN_OPCODE_START, 0, 0, 0, 0, 0, 0, 0};

	data[4] = (len >> 8*3) & 0xFF;
	data[5] = (len >> 8*2) & 0xFF;
	data[6] = (len >> 8*1) & 0xFF;
	data[7]	= (len >> 8*0) & 0xFF;

	//logger_log("Sending start (len %d)\n", len);

	return can_send(can_id, (unsigned char *) data, 8);
}

uint8_t can_send_string_continue(uint32_t can_id, char input[], uint32_t len)
{
	char data[8] = {STRING_TO_CAN_OPCODE_CONTINUE, 0, 0, 0, 0, 0, 0, 0};
	
	if (len > STRING_TO_CAN_PAYLOAD_CAN_SIZE)
	{
		logger_log("Error: len > STRING_TO_CAN_PAYLOAD_CAN_SIZE (%d bytes)", 
						len);
		return FALSE;
	}

	for (uint32_t i = 0; i < len; i++)
	{
		data[i + 1] = input[i];
	}

#ifdef STRING_TO_CAN_LOG_PROGRESS
	logger_log("Sending continue (len %d)\n", len);
#endif

	return can_send(can_id, (unsigned char *) data, 8);
}

uint8_t can_send_string_stop(uint32_t can_id, char input[], uint32_t len)
{
	uint16_t crc = crc_16((unsigned char *) input, len);

	unsigned char data[8] = {STRING_TO_CAN_OPCODE_STOP, 0, 0, 0, 0, 0, 
		((crc >> 8) & 0xFF), ((crc >> 0*8) & 0xFF)};

#ifdef STRING_TO_CAN_LOG_PROGRESS
	logger_log("Sending stop. CRC is 0x%02x 0x%02x\n", data[6], data[7]);
#endif

	return can_send(can_id, data, 8);
}

