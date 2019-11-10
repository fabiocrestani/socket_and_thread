/*
	crc.h
	Author: Fabio Crestani
*/

#ifndef __CRC_H__
#define __CRC_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint16_t crc_16(const unsigned char input[], uint32_t len);

#endif // __CRC_H__
