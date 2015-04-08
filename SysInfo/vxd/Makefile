#****************************************************************************
#
#PCI BIOS, Memory , Interrupt handle VxD
#                                         for windows 95/98
#                                       ver 1.0 Aug 20 1999
#                                        kashiwano masahiro
#
#****************************************************************************

#
# このVxDをコンパイルするにはDDKがインストールされている必要があります。
# 
# 95/98どちらでも動くVxDにするためには95用のDDKが必要です。
# 98用DDKを使用した場合、95では動作しません。
# 95用DDKでは、VC ver2.0に付属のLINKを使用する必要があります。

# 環境に合わせてPATHは修正必要
RC16 = c:\mstools\binw16\rc
ML = c:\ddk\masm\ml
CL = cl
LINK = c:\ddk\vc2link\link

CFLAGS  = -Zdp -Gs -c -DIS_32 -DDEBLEVEL=1 -DDEBUG
LFLAGS     = -machine:i386 -debug:none kernel32.lib 
AFLAGS     = -coff -DBLD_COFF -DIS_32 -W2 -Zd -c -Cx -DMASM6

all: direxist bin\pcidbg95.vxd

direxist:
        if not exist bin\nul    md bin

bin\pcidbg95.res: pcidbg95.rc
       $(RC16)  -Fo$@ -r pcidbg95.rc

bin\pcidbg95.obj: pcidbg95.c
        $(CL) $(CFLAGS) -Fo$@ %s

bin\ctrl.obj: ctrl.asm
        $(ML) $(AFLAGS) -Fo$@ %s

bin\pcidbg95.vxd: bin\pcidbg95.obj bin\ctrl.obj bin\pcidbg95.res\
                  pcidbg95.def
        echo >NUL @<<pcidbg95.crf
-MACHINE:i386 -DEBUG:NONE -PDB:NONE 
-DEF:pcidbg95.def -OUT:bin\pcidbg95.vxd -MAP:bin\pcidbg95.map
-VXD vxdwraps.clb bin\pcidbg95.obj bin\ctrl.obj 
<<
        $(LINK) @pcidbg95.crf
        cd bin
        mapsym pcidbg95
	adrc2vxd pcidbg95.vxd pcidbg95.res
        cd ..

clean:
        del bin\*.obj
        del bin\*.vxd
        del bin\*.exe
        del bin\*.exp
        del bin\*.map
        del bin\*.sym
        del bin\*.lib
        del bin\*.res
        rd  bin
        del *.pdb
        
