CC = gcc
CFLAGS = -m32 -Wall -g -O2 -W
AS = nasm
ASMFLAGS = -g -f elf32

all: integral

integral: main.o functions.o roots.o integral.o test.o
	$(CC) $(CFLAGS) $^ -lm -o $@
main.o: main.c functions.h roots.h integral.h test.h
	$(CC) $(CFLAGS) -c $< -o $@
functions.o: functions.asm
	$(AS) $(ASMFLAGS) functions.asm -o functions.o 
%.o: %.c functions.h
	$(CC) $(CFLAGS) -c $< -o $@

test:
	./integral --test-root 2:3:-1.05:0.33:0.0001:-1
	./integral --test-root 1:3:1:3.2:0.0001:2
	./integral --test-root 1:2:0:2.1:0.0001:1 

	./integral --test-integral 1:0:2:0.00001:25.6
	./integral --test-integral 2:-1:1:0.00001:28.4
	./integral --test-integral 3:-5:0:0.00001:112.5
	
clean:
	-rm *.o
