/*
 * Provides a structure for and operations on DM2-'blocks', which are used to 
 * parse and write demo files.
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

#endif /* __BLOCK_H */
