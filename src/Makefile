CC = emcc
src = $(wildcard *.c)
obj = $(src:.c=.o)
name = wasm-fun

STANDARD_CFLAGS = -Wall -std=c18 -Os
ifeq ($(CC),g++)
	SDL_CFLAGS := $(shell sdl2-config --cflags)
	SDL_LDFLAGS := $(shell sdl2-config --libs)
	LDFLAGS = $(SDL_LDFLAGS) 
	CFLAGS = $(STANDARD_CFLAGS) $(SDL_CFLAGS) $(LDFLAGS) -o $(name)
else
	LDFLAGS = -s USE_SDL=2 -s WASM=1  
	CFLAGS = $(STANDARD_CFLAGS) $(LDFLAGS) --embed-file assets -o $(name).html
endif


myprog: $(obj)
	$(CC) $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(name).html $(name).js $(name).wasm $(name)
