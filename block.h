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
 * block.h
 *
 * Provides a structure and operations on DM2-'blocks' that get read from and 
 * written to the demo files by the main loop 
 */ 

#ifndef __BLOCK_H
#define __BLOCK_H

#define MAX_BSIZE 65536

typedef struct {
	BYTE data[MAX_BSIZE];
	unsigned long size;
	BYTE *p;
} BLOCK_T;

void initBlock(BLOCK_T *b);
void shiftBlockToLeft(BLOCK_T *b, unsigned long count);
int moreBlockData(BLOCK_T *b);
BYTE readByte(BLOCK_T *b);
void writeByte(BLOCK_T *b, BYTE byte);
short readShort(BLOCK_T *b);
void writeShort(BLOCK_T *b, short s);
long readLong(BLOCK_T *b);
void writeLong(BLOCK_T *b, long l);
void copyBytes(BLOCK_T *b_to, BLOCK_T *b_from, unsigned long n);
BYTE copyByte(BLOCK_T *b_to, BLOCK_T *b_from);
short copyShort(BLOCK_T *b_to, BLOCK_T *b_from);
long copyLong(BLOCK_T *b_to, BLOCK_T *b_from);
void copyString(BLOCK_T *b_to, BLOCK_T *b_from);
void skipBytes(BLOCK_T *b, unsigned long n);
void skipByte(BLOCK_T *b);
void skipShort(BLOCK_T *b);
void skipLong(BLOCK_T *b);

#endif

/* EOF */

