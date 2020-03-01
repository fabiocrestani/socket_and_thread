/*
 * circular_buffer.h
 * Author: Fabio Crestani
 */

#ifndef __CIRCULARBUFFER_H__
#define __CIRCULARBUFFER_H__

#include <stdint.h>

#define BUFFER_SIZE 1024*2

#define DEBUG_CIRCULAR_BUFFER

typedef struct _circularBuffer {
	uint16_t    idx_prod;
	uint16_t    idx_cons;
	uint16_t    element_counter;
	void     	*data;
	size_t 		element_size;
} CircularBuffer;

void buffer_init(CircularBuffer *buf, size_t element_size);
void buffer_put(CircularBuffer *buf, void *new_element);
uint8_t buffer_pop(CircularBuffer *buf, void *target);
uint8_t buffer_is_empty(CircularBuffer *buf);
uint8_t buffer_has_room_for(CircularBuffer *buf, uint32_t len);

#endif
