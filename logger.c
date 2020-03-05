/*
	logger.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>

#include "logger.h"

void logger_dump_hex(char *buffer, unsigned int size) 
{
	unsigned int i;
	for (i = 0; i < size; i++) 
	{
		if (((i % 16) == 0) && i > 1) 
		{
			printf("\r");
		}
		printf("%02x ", buffer[i]);
	}
	printf("\r");
}

void loggerDo(const char * file_name, const char * function_name, 
	unsigned int line_number, const char * format_string, ...) 
{
	va_list marker;
	char temp[DEBUG_BUFFER_SIZE];
	char format[DEBUG_BUFFER_SIZE];

	memset(temp, 0, sizeof(temp));
	memset(format, 0, sizeof(format));

	#ifdef DEBUG_SHOW_FILE_PATH
		sprintf(format, "[%s][%s][%d]: ",file_name, function_name, line_number);
	#else
		#ifdef LOGGER_SIMPLE
			sprintf(format, " ");
			(void) file_name;
			(void) function_name;
			(void) line_number;
		#else
			sprintf(format, "[%s][%d]: ", function_name, line_number);
			(void) file_name;
		#endif
	#endif

	strcat(format, format_string);

	va_start(marker, format_string);
	vsprintf(temp, format, marker);
	va_end(marker);

	printf("%s", temp);
}

void logger_log_message(char header[], unsigned char buffer[], int numbytes)
{
	unsigned int log_max_len = 2048;
	char log_temp[log_max_len];
	char temp2[8];
	sprintf(log_temp, "[%s] (%d bytes) ", header, numbytes);
	for (int i = 0; i < numbytes; i++)
	{
		if ((strlen(log_temp) + 8) >= log_max_len)
		{
			strcat(log_temp, "...");
			break;
		}
		sprintf(temp2, "0x%02x ", buffer[i]);
		strcat(log_temp, temp2);
	}
	strcat(log_temp, "\n");
	logger_log(log_temp);
}

