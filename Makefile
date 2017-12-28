CC=g++
CFLAGS=-g
DEPS =  
OBJ = httpd.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

httpd: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ) httpd
