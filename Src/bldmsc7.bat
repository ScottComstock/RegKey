del *.lst
del *.obj
del *.lib
del *.ASM
del *.QLB
rem del p:\doors\libs\vbdos\rkvb10.lib
cl /c /AL /Gt8 /Ge /FaRKMATH.ASM /DNOT_C_TARGET /DVB_TARGET rkmath.c
cl /c /AL /Gt8 /Ge /FaRKNEWSET.ASM /DNOT_C_TARGET /DVB_TARGET rknewset.c
cl /c /AL /Gt8 /Ge /FaRKGEN.ASM /DNOT_C_TARGET /DVB_TARGET rkgen.c
cl /c /AL /Gt8 /Ge /FaRKVAL.ASM /DNOT_C_TARGET /DVB_TARGET rkval.c
cl /c /AL /Gt8 /Ge /FaRKFGEN.ASM /DNOT_C_TARGET /DVB_TARGET rkfgen.c
cl /c /AL /Gt8 /Ge /FaRKFVAL.ASM /DNOT_C_TARGET /DVB_TARGET rkfval.c
cl /c /AL /Gt8 /Ge /FaRKFILE.ASM /DNOT_C_TARGET /DVB_TARGET rkfile.c
lib rkvb10.lib +rkfgen+rkfile+rkfval+rkgen+rkmath+rknewset+rkval,rkvb10.lst;
link /nod /q /packc /packd /packf rkfgen+rkfile+rkfval+rkgen+rkmath+rknewset+rkval,rkvb10.qlb,,n:\vbdos10\lib\vbdosqlb.lib;
rem copy rkvb10.lib p:\doors\libs\vbdos
