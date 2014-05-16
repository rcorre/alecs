#ifndef SCENE_H
#define SCENE_H

/** \file scene.h
  * \brief defines handlers that hook into the main game loop to implement a
  * "scene" or "game state".
  * scenes can be easily swapped out to change the game state by calling \ref
  * register_scene
**/

#include <allegro5/allegro.h>

/** function to process a mouse event */
typedef void (*mouse_handler)(ALLEGRO_MOUSE_EVENT);
/** function to process a keyboard event */
typedef void (*keyboard_handler)(ALLEGRO_KEYBOARD_EVENT);
/** function to process an update, taking the time elapsed as an argument */
typedef bool (*update_handler)(double);
/** function to draw to the display once per frame */
typedef void (*draw_handler)(void);
/** function to handle a shutdown request */
typedef void (*shutdown_handler)(void);

/** defines input handlers, update/draw behaviour, and other properties of a
 * particular game scene. Any of the handlers may be NULL to indicate that the
 * scene does not respond to that event. */
typedef struct scene {
  mouse_handler mouse;       ///< called for each mouse event
  keyboard_handler keyboard; ///< called for each keyboard event
  update_handler update;     ///< called once every 1 / \ref FPS
  draw_handler draw;         ///< called between clearing and before flipping
  shutdown_handler shutdown; ///< called before game exit or scene change
  ALLEGRO_COLOR bg_color;    ///< color to clear background to
} scene;

/** requested background color of current scene */
extern ALLEGRO_COLOR scene_bg_color;

// set the current scene
void register_scene(scene s);
// ask current scene to handle mouse or keyboard input
void scene_handle_input(ALLEGRO_EVENT ev);
// update current scene with elapsed time. Returns false to request termination
bool scene_update(double time);
// draw current scene to display
void scene_draw();
// call current scene's shutdown function and set all handlers to NULL
void scene_shutdown();

#endif /* end of include guard: SCENE_H */
