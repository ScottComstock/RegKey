del *.lst
del *.obj
del *.lib
rem del p:\doors\libs\vbdos\rkvb10.lib
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET rkmath.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET rknewset.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET rkgen.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET rkval.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET rkfgen.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET rkfval.c
cl /c /Gq /o /DBUILDING_REGKEY /DNOT_C_TARGET /DVB_TARGET rkfile.c
lib rkvb10.lib /noe +rkfgen+rkfile+rkfval+rkgen+rkmath+rknewset+rkval,rkvb10.lst;
rem copy rkvb10.lib p:\doors\libs\vbdos
