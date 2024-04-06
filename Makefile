all: main lib operations file_manager
	gcc -I. *.o -o main.output -std=c99 -Wall -lmd
	rm -f *.o
 
main:
	gcc -c src/main.c

lib:
	gcc -c src/Lib/*.c 

operations:
	gcc -c src/Operations/*.c

funcoes_fornecidas:
	gcc -c src/Funcoes_fornecidas/*.c

file_manager: file_walker
	gcc -c src/FileManager/*.c

file_walker: header register
	gcc -c src/FileManager/FileWalker/*.c

header:
	gcc -c src/FileManager/FileWalker/Header/*.c

register:
	gcc -c src/FileManager/FileWalker/Register/*.c

clean:
	rm -f *.o *.output *.zip

run:
	./main.output

test:
	./main.output < ./tests/caso1.in
	
zip:
	zip -R arquivo_intro.zip . '*.c' '*.h' 'Makefile' '*.in' '*.out' '*.txt' '*.bin' '*.csv'
