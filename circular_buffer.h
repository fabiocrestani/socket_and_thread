/*
 * circular_buffer.h
 * Author: Fabio Crestani
 */

#ifndef __CIRCULARBUFFER_H__
#define __CIRCULARBUFFER_H__

#include <stdint.h>
#include <pthread.h>

#define DEFAULT_BUFFER_SIZE 256

#define DEBUG_CIRCULAR_BUFFER

typedef struct _circularBuffer {
	uint32_t buffer_size;

	uint16_t idx_prod;
	uint16_t idx_cons;
	uint16_t element_counter;
	void *data;
	size_t element_size;

	int ready_flag;
	pthread_mutex_t thread_ready_mutex;
	pthread_cond_t thread_ready_cond;

} CircularBuffer;

void buffer_init(CircularBuffer *buf, size_t element_size);
void buffer_denit(CircularBuffer *buf);
void buffer_put(CircularBuffer *buf, void *new_element);
uint8_t buffer_pop(CircularBuffer *buf, void *target);
uint8_t buffer_is_empty(CircularBuffer *buf);
uint8_t buffer_has_room_for(CircularBuffer *buf, uint32_t len);

void buffer_wait_ready(CircularBuffer *buf);
void buffer_set_ready(CircularBuffer *buf, int ready);

#endif
