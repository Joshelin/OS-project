
CC = arm-none-eabi-gcc
LD =  arm-none-eabi-ld
SRCDIR = src/
PATHUARM = /usr/include/uarm/
OBJECT = $(PATHUARM)crtso.o $(PATHUARM)libuarm.o main.o Tree.o Queue.o Alloc.o Semaphore.o 
ELF32 = /usr/include/uarm/ldscripts/elf32ltsarm.h.uarmcore.x    
OPTIONS = -mcpu=arm7tdmi 

all: main pulitore

main: $(OBJECT)
	$(LD) -T $(ELF32) -o main $(OBJECT)
main.o: main.c
	$(CC) $(OPTIONS) -c main.c
Tree.o: $(SRCDIR)Tree.c
	$(CC) $(OPTIONS) -c $(SRCDIR)Tree.c 
Queue.o: $(SRCDIR)Queue.c
	$(CC) $(OPTIONS) -c $(SRCDIR)Queue.c
Alloc.o: $(SRCDIR)Alloc.c
	$(CC) $(OPTIONS) -c $(SRCDIR)Alloc.c
Semaphore.o: $(SRCDIR)Semaphore.c
	$(CC) $(OPTIONS) -c $(SRCDIR)Semaphore.c

pulitore: $(OBJECT)
	make clean

clean:
	rm -f *.o 

