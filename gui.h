/*
	gui.h
	Author: Fabio Crestani
*/

#ifndef __GUI_H__
#define __GUI_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "logger.h"

void gui_init(uint8_t dut, uint8_t left);
void gui_update(void);
void gui_increment_pos(void);
void gui_set_last_message_sent(char message[]);
void gui_set_last_message_received(char message[]);

#endif // __GUI_H__
