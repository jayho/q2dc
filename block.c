/*
 * Copyright 2005 Jens Hoffmann
 * 
 * This file is part of q2dc.
 * 
 * q2dc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * 
 * Foobar is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with Foobar. If not, see
 * http://www.gnu.org/licenses/.
 */

/*
 * block.c
 *
 * See block.h for a short description
 */

/*#include <stdio.h>*/
#include "common.h"
#include "block.h"

void initBlock(BLOCK_T *b) {
	b->p = b->data;
	b->size = 0;
}

void shiftBlockToLeft(BLOCK_T *b, unsigned long count) {
	unsigned long i;

	for (i=count; i < b->size; i++)
		b->data[i-count] = b->data[i];
	b->p -= count;
	b->size -= count;
}

void copyBytes(BLOCK_T *b_to, BLOCK_T *b_from, unsigned long n) {
	while (n-- > 0) {
		*b_to->p++ = *b_from->p++;
		b_to->size++;
	}
}

BYTE copyByte(BLOCK_T *b_to, BLOCK_T *b_from) {
	BYTE b = readByte(b_from);
	writeByte(b_to, b);
	return b;
}

short copyShort(BLOCK_T *b_to, BLOCK_T *b_from) {
	short s = readShort(b_from);
	writeShort(b_to, s);
	return s;
}

long copyLong(BLOCK_T *b_to, BLOCK_T *b_from) {
	long l = readLong(b_from);
	writeLong(b_to, l);
	return l;
}

void copyString(BLOCK_T *b_to, BLOCK_T *b_from) {
	for(; *b_from->p; *b_to->p++ = *b_from->p++, b_to->size++);
	b_from->p++;
	*b_to->p = '\0';
	b_to->p++;
	b_to->size++;
		
}

void skipBytes(BLOCK_T *b, unsigned long n) {
	b->p += n;
}

void skipByte(BLOCK_T *b) {
	skipBytes(b, 1);
}

void skipShort(BLOCK_T *b) {
	skipBytes(b, 2);
}

void skipLong(BLOCK_T *b) {
	skipBytes(b, 4);
}

int moreBlockData(BLOCK_T *b) {
	return (b->p < &b->data[b->size]);
}

BYTE readByte(BLOCK_T *b) {
	return *b->p++;
}

void writeByte(BLOCK_T *b, BYTE byte) {
	*b->p++ = byte;
	b->size++;
}

short readShort(BLOCK_T *b) {
	return ((short)(readByte(b)))
		+((short)(readByte(b))<<8);
}

void writeShort(BLOCK_T *b, short s) {
	writeByte(b, (BYTE)((s    )&0xFF));
	writeByte(b, (BYTE)((s>> 8)&0xFF));
}

long readLong(BLOCK_T *b) {
	return ((long)(readByte(b)))
		+((long)(readByte(b))<<8)
		+((long)(readByte(b))<<16)
		+((long)(readByte(b))<<24);
}

void writeLong(BLOCK_T *b, long l) {
	writeByte(b, (BYTE)((l    )&0xFF));
	writeByte(b, (BYTE)((l>> 8)&0xFF));
	writeByte(b, (BYTE)((l>>16)&0xFF));
	writeByte(b, (BYTE)((l>>24)&0xFF));
}

/* EOF */

