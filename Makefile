# Copyright 2005 Jens Hoffmann
# 
# This file is part of q2dc.
# 
# q2dc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
# 
# Foobar is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with Foobar. If not, see
# http://www.gnu.org/licenses/.

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
