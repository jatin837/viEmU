CC=clang

run: exe
	./exe

exe: main.c
	$(CC) -o exe main.c

clean: 
	rm ./exe ./debug

debug:
	$(CC) -o debug -g main.c

run_debug: debug
	gdb ./debug
