EXEC=raytracer.exe
SOURCES=json.c color.c bmp.c scene3D.c raytracing.c menu.c args.c main.c -lm
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall
 
.PHONY: default clean
 
default: $(EXEC)
 
json.o:json.c json.h
color.o:color.c color.h json.c json.h
bmp.o:bmp.c bmp.h color.c color.h
scene3D.o:scene3D.c scene3D.h json.c json.h
raytracing.o:raytracing.c raytracing.h
menu.o:menu.c menu.h
args.o:args.c args.h
main.o:main.c json.h color.h bmp.h scene3D.h raytracing.h menu.h args.h

 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~
