CC = gcc # compiler
CFLAGS = -Wall -g # compile flags
LIBS = -lpthread -lrt # libs

all: seq par_t par_p

seq: seq.o
	$(CC) -o seq seq.o $(LIBS)

par_t: par_t.o
	$(CC) -o par_t par_t.o $(LIBS)

par_p: par_p.o
	$(CC) -o par_p par_p.o $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $*.c

%.o: %.c # generates the object files
	$(CC) $(CFLAGS) -c $*.c

clean: # cleans stuff
	rm -f seq par_t par_p *.o *~
