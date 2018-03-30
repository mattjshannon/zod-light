what:
	@echo "Use  make solaris   to build solaris version"
	@echo "Use  make linux     to build linux version"
	@echo "Use  make mac       to build mac version"
solaris:
	cd libBG; gmake all; cd ..
	cd libSFD; gmake; cd ..
	cd libjbcvtc; gmake; cd ..
	cc -c   BackgroundModel.c 
	cc -c   background_elat.c
	cc     BackgroundModel.o \
	background_elat.o \
	cspice.a \
        libBG/libjbBG.a libSFD/libjbSFD.a \
        libjbcvtc/libjbcvtc.a -lm -o BackgroundModel

linux:
	cd libBG; gmake all; cd ..
	cd libSFD; gmake; cd ..
	cd libjbcvtc; gmake -f Makefile.linux; cd ..
	cc -c   BackgroundModel.c 
	cc -c   background_elat.c
	cc     BackgroundModel.o \
	background_elat.o \
	cspice.a \
        libBG/libjbBG.a libSFD/libjbSFD.a \
        libjbcvtc/libjbcvtc.a -lm -o BackgroundModel

mac:
	cd libBG; make all; cd ..
	cd libSFD; make; cd ..
	cd libjbcvtc; make -f Makefile.mac; cd ..
	cc -c   BackgroundModel.c 
	cc -c   background_elat.c
	cc     BackgroundModel.o \
	background_elat.o \
	cspice.a \
        libBG/libjbBG.a libSFD/libjbSFD.a \
        libjbcvtc/libjbcvtc.a -lm -o BackgroundModel	
clean:
	rm -f *.o
	cd libBG; make clean; cd ..
	cd libSFD; make clean; cd ..
	cd libjbcvtc; make clean; cd ..

