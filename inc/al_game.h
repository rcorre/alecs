#ifndef AL_GAME_H
#define AL_GAME_H

/** \file al_game.h
  * \brief functions for managing common needs in an Allegro game
**/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#include "util/stringmap.h"
#include "util/list.h"

#define SCREEN_W 1280
#define SCREEN_H 720
#define FPS 60
#define RESOURCE_DIR "media"
#define FONT_DIR RESOURCE_DIR "/fonts"
#define BITMAP_DIR RESOURCE_DIR "/images"
#define SOUND_DIR RESOURCE_DIR "/sounds"
#define DATA_DIR "data"
#define COMPONENT_DATA_DIR DATA_DIR "/components"
#define MAIN_FONT_NAME "LiberationMono-Regular"

/** pointer to the main game display of size \ref SCREEN_W x \ref SCREEN_H*/
extern ALLEGRO_DISPLAY *display;
/** pointer to the main game event queue */
extern ALLEGRO_EVENT_QUEUE *event_queue;
/** timer that ticks at a rate of \ref FPS */
extern ALLEGRO_TIMER *frame_timer;
/** font for general purpose use */
ALLEGRO_FONT *main_font;

/** set up allegro and subsystems, and load all resources */
int al_game_init();

/** perform teardown before termination */
void al_game_shutdown();

/** get a pointer to a preloaded bitmap resource
  * \param name source image filename without extension
  * \return a pointer to the bitmap preloaded from the source image file
**/
ALLEGRO_BITMAP* al_game_get_bitmap(const char *name);

/** get a pointer to a preloaded font resource
  * \param name source font filename without extension
  * \return a pointer to the font preloaded from the source font file
**/
ALLEGRO_FONT* al_game_get_font(const char *name);

/** get a pointer to a preloaded sound resource
  * \param name source sound filename without extension
  * \return a pointer to the sound preloaded from the sound font file
**/
ALLEGRO_SAMPLE* al_game_get_sound(const char *name);

/** play a sound directly by its name.
  * \param name source sound filename without extension
  * \loop if true, loop; otherwise play once
  * \return id of played sample (can be passed to al_stop_sample)
**/
ALLEGRO_SAMPLE_ID al_game_play_sound(const char *name, bool loop);

#endif /* end of include guard: AL_GAME_H */
