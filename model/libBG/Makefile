# 
# $Id: Makefile,v 1.2 2002/10/12 02:04:41 booth Exp $
#
# Makefile
#
# Description:
#  build library of non-SFD background estimation subroutines
#
#
CC = cc
CFLAGS = -O
LIBNAME = libjbBG.a

INSTALLDIRLIB = ../../../lib

SRCS=   get_bg.c bg_init.c earthsun.c sirtfsun.c \
   get_cspi.c get_zmod.c \
   zkernel.c scattfunc.c thermfunc.c modspec.c fiteval.c planck.c interpol.c

HDRS=   ztrig_s.h zdata_s.h zfuncs_s.h sirtfd_s.h

OBJS=   get_bg.o bg_init.o earthsun.o sirtfsun.o \
   get_cspi.o get_zmod.o \
   zkernel.o scattfunc.o thermfunc.o modspec.o fiteval.o planck.o interpol.o


get_bg.o:  ${HDRS}
bg_init.o: ${HDRS}
earthsun.o:  ${HDRS}
sirtfsun.o:  ${HDRS}
get_zmod.o:  ${HDRS}
zkernel.o:   ${HDRS}
scattfunc.o: ${HDRS}
thermfunc.o: ${HDRS}

all: ${LIBNAME}

${LIBNAME}: ${OBJS}
	rm -f $@
	ar cq $@ ${OBJS}
	rm -f ${OBJS}

install: 
	cp -p   ${LIBNAME} ${INSTALLDIRLIB}

clean:
	rm -f *.o core a.out ${LIBNAME}
