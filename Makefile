CC=gcc
CFLAGS=-I. -Wall -Wextra -lpthread -g
OBJS = udp.o main.o logger.o crc.o circular_buffer.o

RM := rm -rf

all: socket_and_thread

socket_and_thread: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	#$(RM) *.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

run: socket_and_thread
	./socket_and_thread $(ARGS)

clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) socket_and_thread
	-$(RM) *.o
