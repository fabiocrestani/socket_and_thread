/*
 * circular_buffer.c
 * Author: Fabio Crestani
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "circular_buffer.h"

void buffer_init(CircularBuffer *buf, size_t element_size)
{
	buf->buffer_size = DEFAULT_BUFFER_SIZE;
    buf->element_counter = 0;
    buf->idx_prod = 0;
    buf->idx_cons = 0;
	buf->element_size = element_size;
	buf->data = malloc(buf->buffer_size * element_size);

	buf->ready_flag = 0;
	pthread_mutex_init(&(buf->thread_ready_mutex), NULL);
	pthread_cond_init(&(buf->thread_ready_cond), NULL);
}

void buffer_denit(CircularBuffer *buf)
{
	free(buf->data);

	pthread_mutex_destroy(&(buf->thread_ready_mutex));
	pthread_cond_destroy(&(buf->thread_ready_cond));
}

void buffer_put(CircularBuffer *buf, void *new_element)
{
	if (buf->element_counter < buf->buffer_size)
	{
		void *target = (char *) buf->data + (buf->element_size * buf->idx_prod);
		buf->idx_prod++;
		memcpy(target, new_element, buf->element_size);
		buf->element_counter++;
		if (buf->idx_prod >= buf->buffer_size)
		{
			buf->idx_prod = 0;
		}
	}
}

uint8_t buffer_pop(CircularBuffer *buf, void *target)
{
	if (buf->element_counter)
	{
		void *source = (char *) buf->data + (buf->element_size * buf->idx_cons);
		buf->element_counter--;
		memcpy(target, source, buf->element_size);
		buf->idx_cons++;
		if (buf->idx_cons >= buf->buffer_size)
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
	return ((buf->element_counter + len) < buf->buffer_size);
}

// Reference: 
// https://stackoverflow.com/questions/2551529/block-until-an-event-has-completed
void buffer_wait_ready(CircularBuffer *buf)
{
	pthread_mutex_lock(&(buf->thread_ready_mutex));
	while (!buf->ready_flag)
	{
    	pthread_cond_wait(&(buf->thread_ready_cond), &(buf->thread_ready_mutex));
	}
	pthread_mutex_unlock(&(buf->thread_ready_mutex));
}


void buffer_set_ready(CircularBuffer *buf, int ready)
{
	pthread_mutex_lock(&(buf->thread_ready_mutex));
	buf->ready_flag = ready;

	pthread_cond_signal(&(buf->thread_ready_cond)); // or using pthread_cond_broadcast(&ready_cond);
	pthread_mutex_unlock(&(buf->thread_ready_mutex));
}

