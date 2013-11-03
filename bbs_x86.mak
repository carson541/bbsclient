CC = gcc
AR = ar

CFLAGS = -Wall
AFLAGS =

PROG = libbbs.a

HDRS = socket.h
SRCS = socket.c
OBJS = socket.o

$(PROG): $(OBJS)
	 $(AR) rc $@ $(OBJS)

.c.o:
	$(CC) -c -o $@ $<

clean:
	rm -f $(PROG) $(OBJS)

.PHONY:
	clean
