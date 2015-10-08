/**
 * Various utility functions for use by device drivers
 * 
 * Copyright (C) 2015 Tommy Vestermark
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "util.h"
#include <stdio.h>

uint8_t reverse8(uint8_t x) {
    x = (x & 0xF0) >> 4 | (x & 0x0F) << 4;
    x = (x & 0xCC) >> 2 | (x & 0x33) << 2;
    x = (x & 0xAA) >> 1 | (x & 0x55) << 1;
    return x;
}


uint8_t crc8(uint8_t const message[], unsigned nBytes, uint8_t polynomial, uint8_t init) {
    uint8_t remainder = init;	
    unsigned byte, bit;

    for (byte = 0; byte < nBytes; ++byte) {
        remainder ^= message[byte];
        for (bit = 0; bit < 8; ++bit) {
            if (remainder & 0x80) {
                remainder = (remainder << 1) ^ polynomial;
            }
            else {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder;
}


uint8_t crc8le(uint8_t const message[], unsigned nBytes, uint8_t polynomial, uint8_t init) {
    uint8_t crc = init, i;	
    unsigned byte;
    uint8_t bit;


    for (byte = 0; byte < nBytes; ++byte) {
	for (i = 0x01; i & 0xff; i <<= 1) {
	    bit = (crc & 0x80) == 0x80;	    
	    if (message[byte] & i) {
		bit = !bit;
	    }
	    crc <<= 1;
	    if (bit) {
		crc ^= polynomial;
	    }
	}
	crc &= 0xff;
    }

    return reverse8(crc);
}

void local_time_str(time_t time_secs, char *buf) {
	time_t etime;
	struct tm *tm_info;

	if (time_secs == 0) {
		time(&etime);
	} else {
		etime = time_secs;
	}

	tm_info = localtime(&etime);

	strftime(buf, LOCAL_TIME_BUFLEN, "%Y-%m-%d %H:%M:%S", tm_info);
}

float celsius2fahrenheit(float celsius)
{
  return celsius * 9 / 5 + 32;
}


// Test code
// gcc -I include/ -std=gnu99 -D _TEST src/util.c
#ifdef _TEST
int main(int argc, char **argv) {
	fprintf(stderr, "util:: test\n");

	uint8_t msg[] = {0x08, 0x0a, 0xe8, 0x80};

	fprintf(stderr, "util::crc8(): odd parity:  %02X\n", crc8(msg, 3, 0x80));
	fprintf(stderr, "util::crc8(): even parity: %02X\n", crc8(msg, 4, 0x80));

	return 0;
}
#endif /* _TEST */
