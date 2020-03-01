/*
 * circular_buffer.c
 * Author: Fabio Crestani
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circular_buffer.h"

void buffer_init(CircularBuffer *buf, size_t element_size)
{
    buf->element_counter = 0;
    buf->idx_prod = 0;
    buf->idx_cons = 0;
	buf->element_size = element_size;
	buf->data = malloc(BUFFER_SIZE * element_size);
}

void buffer_denit(CircularBuffer *buf)
{
	free(buf->data);
}

void buffer_put(CircularBuffer *buf, void *new_element)
{
	if (buf->element_counter < BUFFER_SIZE)
	{
		void *target = (char *) buf->data + (buf->element_size * buf->idx_prod++);
		memcpy(target, new_element, buf->element_size);
		buf->element_counter++;
		if (buf->idx_prod >= BUFFER_SIZE)
		{
			buf->idx_prod = 0;
		}
	}
}

uint8_t buffer_pop(CircularBuffer *buf, void *target)
{
	if (buf->element_counter)
	{
		void *source = (char*) buf->data + (buf->element_counter * buf->element_size);
		buf->element_counter--;
		memcpy(target, source, buf->element_size);
		if (buf->idx_cons >= BUFFER_SIZE)
		{
			buf->idx_cons = 0;
		}
		return 1;
	}
    return 0;
}

uint8_t buffer_is_empty(CircularBuffer *buf)
{
	uint8_t isEmpty = 0;
	if (buf->element_counter == 0)
	{
		isEmpty = 1;
	}
	return isEmpty;
}

uint8_t buffer_has_room_for(CircularBuffer *buf, uint32_t len)
{
	return ((buf->element_counter + len) < BUFFER_SIZE);
}




