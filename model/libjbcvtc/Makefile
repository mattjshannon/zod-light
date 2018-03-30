#
# $Id: Makefile,v 1.4 2006/08/29 17:36:02 tatianag Exp $
#
# Makefile
#
# Description:
#  build file for C version of J. Bennett's coordinate conversion (ccon) 
#  library 
#
#
RANLIB = :
CFLAGS = -xc99=none -g -I.
LIBNAME = libjbcvtc.a

INSTALLDIRLIB  = ../../lib
INSTALLDIRMAN3 = ../../man/man3

SRCS=   ecleqx.c fk5prc.c galeqd.c gtjpa.c gtjul2.c \
	jbdist.c jbposa.c jcnvc2.c jcnvp2.c jgtfkc.c \
	nwcprc.c
OBJS=   ecleqx.o fk5prc.o galeqd.o gtjpa.o gtjul2.o \
	jbdist.o jbposa.o jcnvc2.o jcnvp2.o jgtfkc.o \
	nwcprc.o


all: ${LIBNAME}


${LIBNAME}: ${OBJS}
	rm -f ${LIBNAME}
	ar q ${LIBNAME} ${OBJS}
	$(RANLIB) $(LIBNAME)

install: ${INSTALLDIRLIB}/${LIBNAME}

${INSTALLDIRLIB}/${LIBNAME}: ${LIBNAME}
	cp -p ${LIBNAME} ${INSTALLDIRLIB}
	$(RANLIB)  ${INSTALLDIRLIB}/${LIBNAME}
#	cp fk5prc.3c ${INSTALLDIRMAN3}
#	cp gtjpa.3c  ${INSTALLDIRMAN3}
#	cp gtjul2.3c ${INSTALLDIRMAN3}
#	cp jbdist.3c ${INSTALLDIRMAN3}
#	cp jbelpt.3c ${INSTALLDIRMAN3}
#	cp jbposa.3c ${INSTALLDIRMAN3}
#	cp jcnvc2.3c ${INSTALLDIRMAN3}
#	cp jcnvp2.3c ${INSTALLDIRMAN3}
#	cp nwcprc.3c ${INSTALLDIRMAN3}
#	cp unjpa.3c  ${INSTALLDIRMAN3}
#	cp unjul2.3c ${INSTALLDIRMAN3}

clean:
	rm -f *.o *.a *.i a.out core
