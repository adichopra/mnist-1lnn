all: main

main: 
	gcc -o bin/mnist-1lnn -Iutil main.c 1lnn.c util/mnist-utils.c

