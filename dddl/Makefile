CFLAGS = -g -Wall -Werror
YFLAGS = -d -v

all:
	cd .. ; make

install:
	cd .. ; make install

dddl: parser.o lexer.o strings.o
	$(CC) -o dddl parser.o lexer.o strings.o -lfl

parser.c: parser.y
	bison $(YFLAGS) parser.y -o parser.c

lexer.c: lexer.l
	flex -olexer.c lexer.l

clean:
	rm -f *~ *.o y.output y.tab.h parser.c lexer.c dddl parser.output parser.h strings.o

*.o: builtin.h parser.h strings.h
