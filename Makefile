CC=gcc
CFLAGS=-I. -Wall -Wextra -lpthread
OBJS = udp.o can.o can_visual_send_receive.o logger.o crc.o gui.o

RM := rm -rf

all: can_visual_send_receive

can_visual_send_receive: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	#$(RM) *.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

run: can_visual_send_receive
	./can_visual_send_receive $(ARGS) &

clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) can_visual_send_receive
	-$(RM) *.o
