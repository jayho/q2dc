.PHONY: clean

CFLAGS = -Wall -ansi -pedantic

CC = gcc
LIBS =   
INCLUDES =
OBJS = dm2convert.o dm2.o block.o tools.o
SRCS = dm2convert.c dm2.c block.c tools.c
HDRS = dm2convert.h dm2.h block.h tools.h

all: q2dc

# The variable $@ has the value of the target. $+ are all prerequisites, space separated
q2dc: ${OBJS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ $+

%.o:%.c
	${CC} ${CFLAGS} ${INCLUDES} -c $+

clean:
	rm *.o q2dc*

# DO NOT DELETE
