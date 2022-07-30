all: lib

lib: libsaucy.a

libsaucy.a : saucy.o
	ar cr $@ $^
	ranlib $@

saucy.o: saucy.c
	g++ -c -ansi -pedantic -Wall -O3 $< -o $@


clean:
	rm -f saucy.o libsaucy.a
