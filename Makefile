#crear un Makefile para un proyecto de c y DJGPP y MSDOS
#añadir la carpeta c:/allegro4/include para buscar los archivos de cabecera
#añadir la libreria de allegro -lalleg
#Hay dos ficheros uno es main.c y el otro el lib.c
#el ejecutable se llama game.exe

CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lalleg
INCLUDES = -Ic:/allegro4/include
OBJS = main.o lib.o

game.exe: $(OBJS)
	$(CC) $(OBJS) -o game.exe $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c main.c

lib.o: lib.c
	$(CC) $(CFLAGS) $(INCLUDES) -c lib.c

clean:
	del *.o

