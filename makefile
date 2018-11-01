all: dirinfo.o
	gcc dirinfo.o

dirinfo.o: dirinfo.c
	gcc -c dirinfo.c

clean:
	rm *.o
	rm a.out

run:
	./a.out
