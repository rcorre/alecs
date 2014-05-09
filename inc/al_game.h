#ifndef AL_GAME_H
#define AL_GAME_H

#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#define SCREEN_W 1280
#define SCREEN_H 720
#define FPS 60
#define MEDIA_DIR "media"
#define FONT_DIR MEDIA_DIR "/fonts"
#define MAIN_FONT_PATH FONT_DIR "/LiberationMono-Regular.ttf"
#define DATA_DIR "data"
#define COMPONENT_DATA_DIR DATA_DIR "/components"

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *frame_timer;
extern ALLEGRO_FONT *main_font;

// set up allegro and subsystems
int al_game_init();

// perform teardown before termination
void al_game_shutdown();

#endif /* end of include guard: AL_GAME_H */
