I found some of my old code from the late 90's. I remember having a lot of fun
writing low(ish) level code for dealing with graphics, mouse, extended memory, etc.

This goes back to the very start of my journey in software development.

I thought it would be fun to run some of this code, but found that it was either
incomplete or broken in various ways.

My best guess is that at some point I decided to replace all the various
graphics functions with a class hierarchy. None of the other code uses the
classes, and unfortunately I don't have any of my original graphics functions.

It looked pretty straight forward to update some of the demos to use the classes,
but the class hierarchy didn't make much sense.

So this repo contains a minor re-imagining of what my early self was up to.
Most of the logic was left untouched, but code was re-structured and formatted
to follow a style that makes me less want to gouge out my eyeballs.

## Building & Running

Use Borland Turbo C++ 3.0 compiler.

In an attempt to simplify building, I added a simple makefile but I don't know
how compliant TC 3.0 is with standard makefile syntax so I didn't get too fancy.

There are make targets for each executable, and another to clean compiled files.

## Links

Interrupt lists

https://www.ctyme.com/intr/int.htm
http://employees.oneonta.edu/higgindm/assembly/DOS_AND_ROM_BIOS_INTS.htm
http://www.techhelpmanual.com/2-main_menu.html

About fonts in VGA ROM

https://www.alexandrugroza.ro/microelectronics/essays-research/vga-rom-fonts/index.html
