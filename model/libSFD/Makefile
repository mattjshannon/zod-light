# 
# $Id: Makefile,v 1.1 2000/08/11 21:47:49 sscadm1 Exp $
#
# Makefile
#
# Description:
#  build file for SFD subroutines (Schlegel routines as modified by J.Bennett)
#
#
CC = cc
CFLAGS = -O
LIBNAME = libjbSFD.a

INSTALLDIRLIB = ../../../lib

SRCS=   dust_getval_sub.c subs_lambert_serv.c subs_fits.c subs_memory.c \
   subs_inoutput.c

OBJS=   dust_getval_sub.o subs_lambert_serv.o subs_fits.o subs_memory.o \
   subs_inoutput.o

all: ${LIBNAME}

${LIBNAME}: ${OBJS}
	rm -f $@
	ar cq $@ ${OBJS}
	rm -f ${OBJS}

install: 
	cp -p   ${LIBNAME} ${INSTALLDIRLIB}

clean:
	rm -f *.o core a.out ${LIBNAME}
