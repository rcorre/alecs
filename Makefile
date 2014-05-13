CC = gcc
# Wall: warn all
# std: use C11 standard
CFLAGS = -Wall -std=c11 -D_GNU_SOURCE
# flags for debug build
# ggdb: use expressive names for gdb debugging
# O0: avoid optimizations to make debugging easier
# pg: generate profiling info for gprof
DBG_FLAGS = $(CFLAGS) -ggdb -O0 -pg
# flags for release build. use O3 for max optimization
REL_FLAGS = $(CFLAGS) -O3

MODULES := component system util scene 
SRC_DIR := src $(addprefix src/,$(MODULES))
INC_DIR = inc

SRC_FILES := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
TEST_SRC := $(filter-out src/main%.c, $(SRC_FILES))

EXECUTABLE = bin/run
# allegro addons to link
ADDONS = 	allegro_acodec-5.0 \
					allegro_audio-5.0 \
					allegro_font-5.0 \
					allegro_ttf-5.0 \
					allegro_image-5.0 \
					allegro_color-5.0 \
					allegro_primitives-5.0

LIBS = `pkg-config --libs allegro-5.0 $(ADDONS)` -lm

all: debug

debug: $(SOURCE_FILES)
	$(CC) $(DBG_FLAGS) -o $(EXECUTABLE) $(SRC_FILES) -I $(INC_DIR) $(LIBS)

release: $(SOURCE_FILES)
	$(CC) $(REL_FLAGS) -o $(EXECUTABLE) $(SRC_FILES) -I $(INC_DIR) $(LIBS)

test: test-stringmap

test-stringmap: $(TEST_SRC) test/test_stringmap.c
	$(CC) $(DBG_FLAGS) -o bin/test_stringmap test/test_stringmap.c $(TEST_SRC) \
		-I $(INC_DIR) $(LIBS)

clean:
	rm -r bin
