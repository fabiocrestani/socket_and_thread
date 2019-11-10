/*
	logger.h
	Author: Fabio Crestani
*/

#ifndef __LOGGER_H__
#define __LOGGER_H__

//#define DEBUG_SHOW_FILE_PATH	// Para mostrar também o nome do arquivo

#define DEBUG_BUFFER_SIZE 4096

void logger_dump_hex(char *buffer, unsigned int size);

void loggerDo(const char * file_name, const char * function_name, 
	unsigned int line_number, const char * format_string, ...);

// Cuidado: a string formatada pela função debug não deverá ultrapassar o 
// valor DEBUG_BUFFER_SIZE
#define logger_log(...) loggerDo(__FILE__, __func__, __LINE__, __VA_ARGS__)

#endif // __LOGGER_H__
