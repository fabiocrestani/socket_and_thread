CC=gcc
CFLAGS=-I. -Wall -Wextra -lpthread
OBJS = udp.o can.o can_visual_ping_pong.o logger.o crc.o gui.o

RM := rm -rf

all: can_visual_ping_pong

can_visual_ping_pong: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	#$(RM) *.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

run: can_visual_ping_pong
	./can_visual_ping_pong $(ARGS) &

clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) can_visual_ping_pong
	-$(RM) *.o
