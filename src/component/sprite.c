#include "component/sprite.h"

/*
static struct ecs_component* new_sprite(component *template) {
  struct ecs_component *c = calloc(1, sizeof(struct ecs_component));
  memcpy(c, template, sizeof(struct ecs_component));
  c->type = COMP_SPRITE;
  c->node = list_push(ecs_component_store[COMP_SPRITE], c);
  return c;
}

static void free_sprite(component *comp) {
  // sprite holds no allocated resources
}

static void load_sprite_templates() {
  sprite_templates = list_new();
  ALLEGRO_CONFIG *cfg = al_load_config_file(COMPONENT_DATA_DIR "/sprites.cfg");
  ALLEGRO_CONFIG_SECTION *sec;
  //char const *key = al_get_first_config_section(cfg, &sec);
}

static void free_sprite_template(struct ecs_component *template) {
  //free bitmap
}

static void free_sprite_templates() {
  list_free(sprite_templates, free);
}
*/
