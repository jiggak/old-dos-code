hello:
	tcc -Ilib hello.cpp lib\vga.cpp lib\font.cpp

clean:
	del *.obj
	del *.exe