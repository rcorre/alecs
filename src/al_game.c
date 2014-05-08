#include "al_game.h"

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *frame_timer;
ALLEGRO_FONT *main_font;

int al_game_init() {
  srand((unsigned)time(NULL));

  if (!al_init()) {
    fprintf(stderr, "failed to init allegro\n");
    return -1;
  }
  if (!al_init_primitives_addon()) {
    fprintf(stderr, "failed to init primitives\n");
    return -1;
  }
  al_init_font_addon();
  al_init_ttf_addon();
  if (!(main_font = al_load_ttf_font(MAIN_FONT_PATH, 12, 0))) {
    fprintf(stderr, "failed to load font %s\n", MAIN_FONT_PATH);
    return -1;
  }
  if (!al_install_keyboard()) {
    fprintf(stderr, "failed to init keyboard\n");
    return -1;
  }
  if (!al_install_mouse()) {
    fprintf(stderr, "failed to init mouse\n");
    return -1;
  }
  if (!al_install_audio()) {
    fprintf(stderr, "failed to init audio\n");
    return -1;
  }
  if (!al_init_acodec_addon()) {
    fprintf(stderr, "failed to init acodec addon\n");
    return -1;
  }
  if (!al_reserve_samples(1)) { //reserve space for a single audio sample
    fprintf(stderr, "failed to init acodec addon\n");
    return -1;
  }
  if (!al_init_image_addon()) {
    fprintf(stderr, "failed to init image addon\n");
    return -1;
  }
  if (!(frame_timer = al_create_timer(1.0 / FPS))) {
    fprintf(stderr, "failed to create timer\n");
  }
  if (!(display = al_create_display(SCREEN_W,SCREEN_H))) {
    fprintf(stderr, "failed to create display\n");
    return -1;
  }
  if (!(event_queue = al_create_event_queue())) {
    fprintf(stderr, "failed to create event_queue\n");
    al_destroy_display(display);
    return -1;
  }

  al_set_target_bitmap(al_get_backbuffer(display));

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(frame_timer));
  al_register_event_source(event_queue, al_get_mouse_event_source());
  al_register_event_source(event_queue, al_get_keyboard_event_source());

  al_clear_to_color(al_map_rgb(0,0,0));
  al_flip_display();

  al_start_timer(frame_timer);

  return 0; // initialization succeeded
}

void al_game_shutdown() {
  if (frame_timer != NULL) {
    al_destroy_timer(frame_timer);
  }
  if (main_font != NULL) {
    al_destroy_font(main_font);
  }
  if (event_queue != NULL) {
    al_destroy_event_queue(event_queue);
  }
  if (display != NULL) {
    al_destroy_display(display);
  }
}
