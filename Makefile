CFLAGS=-g -Wall -Wextra -fdiagnostics-color=always -Wno-misleading-indentation

ifeq ($(OS),Windows_NT)
    RM = cmd /C del /Q
    EXT = .exe
else
    RM = rm -f
    EXT =
endif

PROGS=JOC$(EXT)

.PHONY: all clean realclean

all : $(PROGS)

main.o :main.c 4_enratlla.h Arbre.h
	gcc -c $(CFLAGS) main.c -o main.o

4_enratlla.o : 4_enratlla.c 4_enratlla.h
	gcc -c $(CFLAGS) 4_enratlla.c -o 4_enratlla.o

Arbre.o : Arbre.c Arbre.h 4_enratlla.h
	gcc -c $(CFLAGS)  Arbre.c -o  Arbre.o


JOC$(EXT) : main.o 4_enratlla.o Arbre.o
	gcc $(CFLAGS) -o $@ main.o 4_enratlla.o Arbre.o

clean :

	-$(RM) *.o

realclean : clean
	-$(RM) $(PROGS)
