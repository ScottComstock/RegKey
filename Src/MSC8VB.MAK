CC=cl
BC=bc
CFLAGS=/c /AL /nologo /W3 /DNOT_C_TARGET /DVBDOS_TARGET /DMSDOS /DRKASM
BFLAGS=/d /t /c:512
LFLAGS=/Q /nod
OBJS=rkgen.obj rkval.obj rknewset.obj rkfgen.obj rkfval.obj rkfile.obj rkmath.obj rkvb.obj
VBQLBLIB=n:\vbdos10\lib\vbdosqlb.lib

all: regkeyvb.lib regkeyvb.qlb

regkeyvb.lib: $(OBJS)
    lib regkeyvb.lib +rkmath+rknewset+rkgen+rkval+rkfile+rkfgen+rkfval+rkvb,regkeyvb.lst;

regkeyvb.qlb: $(OBJS)   
    link $(LFLAGS) regkeyvb.lib,regkeyvb.qlb,,$(VBQLBLIB);
    
rkgen.obj: rkgen.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkgen.c

rkval.obj: rkval.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkval.c

rknewset.obj: rknewset.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rknewset.c

rkfgen.obj: rkfgen.c rkgen.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkfgen.c

rkfval.obj: rkfval.c rkval.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkfval.c

rkfile.obj: rkfile.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkfile.c

rkmath.obj: rkmath.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkmath.c

rkvb.obj: rkvb.bas
    $(BC) $(BFLAGS) rkvb.bas rkvb.obj;

clean:
    del *.obj
    del *.lib
    del *.qlb
    del *.map
    del *.lst
    del *.bak
