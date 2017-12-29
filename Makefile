CC=g++
CFLAGS=-g -I.
DEPS = Server.h util.h
OBJ = main.o Server.o util.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) main
