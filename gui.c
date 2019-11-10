/*
	gui.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>

#include "gui.h"

#define W (80)

#define LINE(x) for (int i = 0; i < x; i++) wprintf(L"\n")
#define SPACE(x) for (int i = 0; i < x; i++) wprintf(L" ")
#define BOX_B(x) for (int i = 0; i < x; i++) wprintf(L"%s█%s", COLOR_BLUE, COLOR_RESET)
#define BOX_R(x) for (int i = 0; i < x; i++) wprintf(L"%s█%s", COLOR_RED, COLOR_RESET)

#define COLOR_RED "\033[0;31m"
#define COLOR_RED_BOLD "\033[1;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_GREEN_BOLD "\033[1;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_YELLOW_BOLD "\033[1;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_BLUE_BOLD "\033[1;34m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_MAGENTA_BOLD "\033[1;35m"
#define COLOR_CYAN "\033[0;36m"
#define COLOR_CYAN_BOLD "\033[1;36m"
#define COLOR_RESET "\033[0m"

#define MAX_POS (60)

static int pos = 0;
static uint8_t dut = 1;
static uint8_t left = 0;
static char last_message_sent[512];
static char last_message_received[512];

void fill()
{
	wprintf(L"%s", COLOR_YELLOW);

	for (int i = 0; i < pos; i++)
		wprintf(L"┃");

	wprintf(L"%s", COLOR_RESET);
}

void gui_init(uint8_t new_dut, uint8_t new_left)
{
	dut = new_dut;
	left = new_left;
	gui_update();
}

void gui_increment_pos(void)
{
	pos++;
	if (pos > MAX_POS) pos = 0;	
}

void gui_set_last_message_sent(char message[])
{
	strcpy(last_message_sent, message);
}

void gui_set_last_message_received(char message[])
{
	strcpy(last_message_received, message);
}


void gui_update(void)
{
	// Input from left side of terminal
	if (left)
	{
		LINE(7);
		SPACE(5); wprintf(L"⇇"); SPACE(5); 
		wprintf(L"%sDUT %d Sending Data:%s %s", 
			COLOR_RED_BOLD, dut, COLOR_RESET, last_message_sent);
		LINE(4);
		SPACE(10); wprintf(L"%s DUT %d Input:%s %s", 
			COLOR_RED_BOLD, dut, COLOR_RESET, last_message_received);
		LINE(2);
		BOX_R(W-15); LINE(1);
		BOX_R(W-15); LINE(1);
		SPACE(W - pos - 19); fill(); BOX_R(4); LINE(1);
		SPACE(W - pos - 19); fill(); BOX_R(4); LINE(1);
		SPACE(W - pos - 19); fill(); BOX_R(4); LINE(1);
		SPACE(W - pos - 19); fill(); BOX_R(4); LINE(1);
		SPACE(W - pos - 19); fill(); BOX_R(4); LINE(1);
		BOX_R(W-15); LINE(1);
		BOX_R(W-15); LINE(1);
		LINE(1);
		return;
	}
	
	// Input from right side of terminal
	SPACE(15); wprintf(L"%sDUT %d Input:%s %s", 
		COLOR_BLUE_BOLD, dut, COLOR_RESET, last_message_received);
	LINE(2);
	SPACE(15); BOX_B(W-15);
	SPACE(15); BOX_B(W-15);
	SPACE(15); BOX_B(4); fill(); LINE(1);
	SPACE(15); BOX_B(4); fill(); LINE(1);
	SPACE(15); BOX_B(4); fill(); LINE(1);
	SPACE(15); BOX_B(4); fill(); LINE(1);
	SPACE(15); BOX_B(4); fill(); LINE(1);
	SPACE(15); BOX_B(W-15);
	SPACE(15); BOX_B(W-15);
	LINE(7);
	SPACE(15); wprintf(L"%sDUT %d Sending Data:%s %s ⇉", 
		COLOR_BLUE_BOLD, dut, COLOR_RESET, last_message_sent);
	LINE(5);
}

