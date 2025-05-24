CC=cl
CFLAGS=/c /AL /nologo /W3
LFLAGS=/Q
OBJS=rkgen.obj rkval.obj rknewset.obj rkfgen.obj rkfval.obj rkfile.obj rkmath.obj
LFLAGSOLD=/Q /NOE /DOSSEG /PACKC /PACKD
all: rkvb10.lib rkvb10.qlb

rkvb10.lib: $(OBJS)
    lib rkvb10.lib +rkfgen+rkfile+rkfval+rkgen+rkmath+rknewset+rkval,rkvb10.lst;

rkvb10.qlb: $(OBJS)
    link $(LFLAGS) +rkfgen+rkfile+rkfval+rkgen+rkmath+rknewset+rkval,rkvb10.qlb,,n:\vbdos10\lib\vbdosqlb.lib;
    
rkgen.obj: rkgen.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkgen.c

rkval.obj: rkval.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkval.c

rknewset.obj: rknewset.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rknewset.c

rkfgen.obj: rkfgen.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkfgen.c

rkfval.obj: rkfval.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkfval.c

rkfile.obj: rkfile.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkfile.c

rkmath.obj: rkmath.c regkey.h rkintern.h
    $(CC) $(CFLAGS) rkmath.c

clean:
    del *.obj
    del *.lib
    del *.qlb
    del *.map
    del *.lst
