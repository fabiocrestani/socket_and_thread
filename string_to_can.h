/*
	string_to_can.h
	Author: Fabio Crestani
*/

#ifndef __STRING_TO_CAN_H__
#define __STRING_TO_CAN_H__

#ifndef TRUE
	#define TRUE (1)
#endif

#ifndef FALSE
	#define FALSE (0)
#endif

#define STRING_TO_CAN_MAX_STRING_SIZE (2147483648) // 2^31
#define STRING_TO_CAN_PAYLOAD_CAN_SIZE (7)

#define STRING_TO_CAN_OPCODE_START (1)
#define STRING_TO_CAN_OPCODE_CONTINUE (2)
#define STRING_TO_CAN_OPCODE_STOP (3)

//#define STRING_TO_CAN_LOG_PROGRESS

#endif // __STRING_TO_CAN_H__
