BIN = makebin
OBJ = makebin.o

all: $(BIN)

clean:
	rm -f $(BIN) $(OBJ) *~

install: all
	install --strip $(BIN) /usr/bin