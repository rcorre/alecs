#include "system/sprite_sys.h"

static void draw_sprite(void *component) {
  ecs_component *c = (ecs_component*)component;
  ecs_entity *e = c->owner_entity;
  sprite s = c->sprite;
  al_draw_tinted_scaled_rotated_bitmap(
      s.bitmap,                     // sprite bitmap
      s.tint,                       // sprite color
      s.center.x, s.center.y,       // center of bitmap
      e->position.x, e->position.y, // location to draw center to
      s.scale, s.scale,             // x and y scaling
      e->angle,                     // rotation of entity
      0                             // draw flags (flip horizontal/vertical)
  );
}

void sprite_system_draw(void) {
  list *sprite_list = ecs_component_store[COMP_SPRITE];
  list_each(sprite_list, draw_sprite);
}
