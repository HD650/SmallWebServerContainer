CC=g++
CFLAGS= -std=c++11 -g -I. -lpthread
DEPS = Server.h util.h
OBJ = main.o Server.o util.o

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $< 

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) main
