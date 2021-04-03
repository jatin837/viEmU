CC=clang

run: exe
	./exe

exe: main.c
	$(CC) -o exe main.c

clean: 
	rm ./exe
