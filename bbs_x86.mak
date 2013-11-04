CC = gcc
AR = ar

CFLAGS = -Wall
AFLAGS =

PROG = libbbs.a

HDRS = socket.h telnet.h terminal.h thread.h
SRCS = socket.c telnet.c terminal.c thread.c
OBJS = socket.o telnet.o terminal.o thread.o

$(PROG): $(OBJS)
	 $(AR) rc $@ $(OBJS)

.c.o:
	$(CC) -c -o $@ $<

clean:
	rm -f $(PROG) $(OBJS)

.PHONY:
	clean
