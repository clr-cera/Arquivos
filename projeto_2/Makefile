all: main lib operations file_manager funcoes_fornecidas
	gcc -I. *.o -o main.output -Wall -g
	rm -f *.o
 
main:
	gcc -c src/main.c -g

lib:
	gcc -c src/Lib/*.c -g

operations:
	gcc -c src/Operations/*.c -g

funcoes_fornecidas:
	gcc -c src/Funcoes_fornecidas/*.c -g

file_manager: file_walker index_walker b_tree
	gcc -c src/FileManager/*.c -g

file_walker: header register filter
	gcc -c src/FileManager/FileWalker/*.c -g

header:
	gcc -c src/FileManager/FileWalker/Header/*.c -g

register:
	gcc -c src/FileManager/FileWalker/Register/*.c -g

filter:
	gcc -c src/FileManager/FileWalker/Filter/*.c -g

index_walker: index index_header
	gcc -c src/FileManager/IndexWalker/*.c -g

index:
	gcc -c src/FileManager/IndexWalker/Index/*.c -g

index_header:
	gcc -c src/FileManager/IndexWalker/IndexHeader/*.c -g

b_tree: node b_header
	gcc -c src/FileManager/BTreeWalker/*.c -g

node:
	gcc -c src/FileManager/BTreeWalker/Node/*.c -g

b_header:
	gcc -c src/FileManager/BTreeWalker/BTreeHeader/*.c -g


clean:
	rm -f *.o *.output *.zip

run:
	./main.output

test:
	@ if [ -z "$(NUM)" ]; then \
		echo "Please provide a NUM value, e.g., 'make test NUM=1'"; \
	else \
		./main.output < ./arquivos/in/$(NUM).in; \
	fi
	
zip:
	zip -R arquivo_2.zip . '*.c' '*.h' 'Makefile' '*.txt'

restore:
	@cp ./arquivos/Entrada/* ./
