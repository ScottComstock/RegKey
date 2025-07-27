CC=cl
CFLAGS=/c /AL /nologo /W3 /DMSDOS /DRKASM
LFLAGS=/Q /nod
OBJS=rkgen.obj rkval.obj rknewset.obj rkfgen.obj rkfval.obj rkfile.obj rkmath.obj rkr.obj

all: regkey.lib

regkey.lib: $(OBJS)
    lib regkey.lib +rkmath+rknewset+rkgen+rkval+rkfile+rkfgen+rkfval,regkey.lst;
    
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

rkr.obj: rkr.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkr.c

clean:
    del *.obj
    del *.lib
    del *.qlb
    del *.map
    del *.lst
    del *.bak
