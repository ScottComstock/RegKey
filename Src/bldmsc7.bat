del *.lst
del *.obj
del *.lib
del p:\doors\libs\vbdos\rkvb10.lib
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET /Fcrkmath.out rkmath.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET /Fcrknewset.out rknewset.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET /Fcrkgen.out rkgen.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET /Fcrkval.out rkval.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET /Fcrkfgen.out rkfgen.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET /Fcrkfval.out rkfval.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET /Fcrkfile.out rkfile.c
copy n:\c700\lib\mlibce.lib .
lib rkvb10.lib /noe +rkfgen+rkfile+rkfval+rkgen+rkmath+rknewset+rkval,rkvb10.lst;
rem link /m /q ,,,rkvb10.lib;
copy rkvb10.lib p:\doors\libs\vbdos
