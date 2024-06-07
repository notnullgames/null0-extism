TARGET=wasm32-wasi
WASI_SDK_PREFIX=/opt/wasi-sdk
CC=$(WASI_SDK_PREFIX)/bin/clang

# C compiler and linker flags
CFLAGS=-target $(TARGET) -I../../../pdk/c -I../
LDFLAGS=-Wl,--no-entry -mexec-model=reactor

# List source files
SRC=src/main.c
OBJ=$(SRC:.c=.o)

main.wasm: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

 %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

test: main.wasm
	extism call main.wasm load

clean:
	rm -f $(OBJ) main.wasm
