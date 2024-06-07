# C compiler and target
CC=clang
TARGET=wasm32-unknown-unknown

# C compiler and linker flags
CFLAGS=-target $(TARGET) -nostdlib
LDFLAGS=-Wl,--no-entry -mexec-model=reactor

# List source files
SRC=src/plugin.c
OBJ=$(SRC:.c=.o)

build: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o plugin.wasm -Wl,--export=hello

test: build
	extism call plugin.wasm hello

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ)
