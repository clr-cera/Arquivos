all: main
	gcc -I. *.o -o main.out -std=c99 -Wall -lmd
	rm -f *.o
 
main:
	gcc -c main.c -o main.o

example:
	gcc -c example.c -o example.o

clean:
	rm -f *.o *.out *.zip

run:
	./main.out

test:
	./main.out < ./tests/caso1.in
	
zip:
	zip -R arquivo_intro.zip . '*.c' '*.h' 'Makefile' '*.in' '*.txt'
