/*
	crc.c
	Author: Fabio Crestani
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint16_t crc_16(const unsigned char input[], uint32_t len)
{
    uint16_t crc = 0xFFFF;
    uint8_t i;

    while (len--)
    {
        crc ^= *input++;

        for (i = 0; i < 8; i++)
        {
            if (!(crc & 1))
			{
                crc >>= 1;
			}
            else
            {
                crc >>= 1;
                crc ^= 0x8408;
            }
        }
    }

    return ~crc;
}
