#
# Makefile for TINY
# 20171643 Park Junhyeok
#

CC = gcc

CFLAGS = 

OBJS = lex.yy.o tiny.tab.o main.o util.o

hw2_binary: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o hw2_binary

main.o: main.c globals.h util.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

tiny.tab.o: tiny.tab.c
	$(CC) $(CFLAGS) -c tiny.tab.c

lex.yy.c: tiny.l
	bison -d tiny.y
	flex tiny.l

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

clean:
	-rm hw2_binary
	-rm $(OBJS)
	-rm lex.yy.c
	-rm tiny.tab*