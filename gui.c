/*
	gui.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <locale.h>
#include <wchar.h>

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

// max x is 56
void fill(int x)
{
	wprintf(L"%s", COLOR_YELLOW);

	for (int i = 0; i < x; i++)
		wprintf(L"┃");

	wprintf(L"%s", COLOR_RESET);
}

void gui_init(uint8_t left)
{
	int x = 10;

	// Left side of terminal
	if (left)
	{
		LINE(6);
		SPACE(15); wprintf(L"⇇"); SPACE(26); wprintf(L"%sDUT 2 Sending Data%s", 
			COLOR_RED_BOLD, COLOR_RESET);
		LINE(6);
		SPACE(41); wprintf(L"%s DUT 2 Input Buffer%s\n\n", COLOR_RED_BOLD, COLOR_RESET);
		BOX_R(W-20); LINE(1);
		BOX_R(W-20); LINE(1);
		SPACE(W-x - 24); fill(x); BOX_R(4); LINE(1);
		SPACE(W-x - 24); fill(x); BOX_R(4); LINE(1);
		SPACE(W-x - 24); fill(x); BOX_R(4); LINE(1);
		SPACE(W-x - 24); fill(x); BOX_R(4); LINE(1);
		SPACE(W-x - 24); fill(x); BOX_R(4); LINE(1);
		BOX_R(W-20); LINE(1);
		BOX_R(W-20); LINE(1);
		return;
	}
	
	// Right side of terminal
	SPACE(20); wprintf(L"%sDUT 1 Input Buffer%s", COLOR_BLUE_BOLD, COLOR_RESET);
	LINE(2);
	SPACE(20); BOX_B(W-20);
	SPACE(20); BOX_B(W-20);
	SPACE(20); BOX_B(4); fill(x); LINE(1);
	SPACE(20); BOX_B(4); fill(x); LINE(1);
	SPACE(20); BOX_B(4); fill(x); LINE(1);
	SPACE(20); BOX_B(4); fill(x); LINE(1);
	SPACE(20); BOX_B(4); fill(x); LINE(1);
	SPACE(20); BOX_B(W-20);
	SPACE(20); BOX_B(W-20);
	LINE(8); 
	SPACE(20); wprintf(L"%sDUT 1 Sending Data                       %s⇉", 
		COLOR_BLUE_BOLD, COLOR_RESET);
	LINE(4);

}

void gui_update(void)
{



}

