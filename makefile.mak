hello:
	tcc -Ilib hello.cpp lib\vga.cpp lib\font.cpp

hellopcx:
	tcc -Ilib hellopcx.cpp lib\vga.cpp lib\pcx.cpp lib\fontpcx.cpp

square:
	tcc -Ilib square.cpp lib\vga.cpp lib\draw.cpp lib\draw3d.cpp

imgview:
	tcc -Ilib imgview.cpp lib\vga.cpp lib\draw.cpp lib\pcx.cpp

clean:
	del *.obj
	del *.exe