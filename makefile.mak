hello:
	tcc -Ilib hello.cpp lib\vga.cpp lib\font.cpp

square:
	tcc -Ilib square.cpp lib\vga.cpp lib\draw.cpp lib\draw3d.cpp

clean:
	del *.obj
	del *.exe