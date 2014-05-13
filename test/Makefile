CC = gcc
# Wall: warn all
# std: use C11 standard
CFLAGS = -Wall -std=c11 -D_GNU_SOURCE
# flags for debug build
# ggdb: use expressive names for gdb debugging
# O0: avoid optimizations to make debugging easier
# pg: generate profiling info for gprof
DBG_FLAGS = $(CFLAGS) -ggdb -O0 -pg

INC_DIR = ../inc

# allegro addons to link
ADDONS = 	allegro_acodec-5.0 \
					allegro_audio-5.0 \
					allegro_font-5.0 \
					allegro_ttf-5.0 \
					allegro_image-5.0 \
					allegro_color-5.0 \
					allegro_primitives-5.0

LIBS = `pkg-config --libs allegro-5.0 $(ADDONS)` -lm

all: test-stringmap

test-stringmap: test_stringmap.c
	$(CC) $(DBG_FLAGS) -o test_stringmap test_stringmap.c -I $(INC_DIR) $(LIBS)

clean:
	rm test_stringmap
