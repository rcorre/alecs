#include <stdio.h>
#include <stdlib.h>

#include "al_game.h"
#include "ecs.h"
#include "particle_effects.h"
#include "system/keyboard_sys.h"
#include "system/mouse_sys.h"
#include "scene/scene.h"
#include "scene/level.h"

static double last_frame_time; // when the last update occured

// compute frame time and update all systems. return current fps
static bool main_update();
// render current frame and flip buffer to display
static void main_draw();

int main(int argc, char *argv[]) {
  if (al_game_init() != 0) { // set up allegro framework
    fprintf(stderr, "allegro setup failed\n");
    al_game_shutdown();
    exit(-1);
  }

  particle_init(al_get_backbuffer(display));
  ecs_init();                  // set up entity-component-system framework
  register_scene(level_new()); // set initial scene

  bool run = true;         // false when game should exit
  bool frame_tick = false; // true when frame time has ticked (time to update)
  while(run) {             // update-draw loop
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);
    switch(ev.type) {
      case ALLEGRO_EVENT_TIMER:
        if (ev.timer.source == frame_timer) {
          frame_tick = true;
        }
        break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        run = false;       // stop running if user asks to close window
      default:
        ecs_handle_keypress(ev);
        ecs_handle_mouse(ev);
        scene_handle_input(ev);
        break;
    }
    if (frame_tick && al_is_event_queue_empty(event_queue)) {
      frame_tick = false;
      run = main_update();
      main_draw();
    }
  }

  particle_shutdown();
  ecs_shutdown();
  al_game_shutdown();
  return 0;
}

static bool main_update() {
  double cur_time = al_get_time();
  double delta = cur_time - last_frame_time; // time elapsed since last frame
  last_frame_time = cur_time;
  ecs_update_systems(delta);      // update every system
  update_particles(delta);
  bool run = scene_update(delta); // run scene's update function
  return run;
}

static void main_draw() {
  al_clear_to_color(scene_bg_color);
  draw_particles();
  render_all_sprites();
  weapon_system_draw();
  scene_draw(); // the scene may draw something in addition to sprites (UI)
  al_flip_display();
}
