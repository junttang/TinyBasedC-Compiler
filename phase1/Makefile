#
# Makefile for TINY
# 20171643 Park Junhyeok
#

CC = gcc

CFLAGS = 

OBJS = main.o util.o lex.yy.o

hw1_binary: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o hw1_binary

main.o: main.c globals.h util.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

lex.yy.c: tiny.l
	flex tiny.l

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

clean:
	-rm hw1_binary
	-rm $(OBJS)
	-rm lex.yy.c
