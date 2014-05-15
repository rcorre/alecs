#include "render.h"

// each layer stores a list of sprites to be drawn at that layer
list *sprite_layers[SPRITE_LAYER_LIMIT * 2 + 1];

static void draw_sprite(sprite *s);
static list* get_sprite_layer(int layernum);

void sprite_init() {
  for (int i = 0; i < SPRITE_LAYER_COUNT; i++) {
    assert(sprite_layers[i] == NULL); // assert not already initialized
    sprite_layers[i] = list_new();    // create a new list for each depth layer
  }
}

void sprite_shutdown() {
  for (int i = 0; i < SPRITE_LAYER_LIMIT * 2 + 1; i++) {
    list_free(sprite_layers[i], (list_lambda)sprite_free);
  }
}

sprite* sprite_new(const char *name, vector *ref_position, double *ref_angle, 
    int depth) {
  sprite *s = malloc(sizeof(sprite));
  ALLEGRO_BITMAP *bmp = al_game_get_bitmap(name);
  assert(bmp != NULL);
  s->bitmap = bmp;
  s->width = al_get_bitmap_width(bmp);
  s->height = al_get_bitmap_height(bmp);
  s->center = (vector) {
    .x = al_get_bitmap_width(bmp) / 2,
    .y = al_get_bitmap_height(bmp) / 2
  };
  s->scale = 1;
  s->tint = al_map_rgb(255,255,255);
  s->position_ptr = ref_position;
  s->angle_ptr = ref_angle;
  // give sprite back-reference to its node so it may be removed when freed
  s->_node = list_push(get_sprite_layer(depth), s);
  s->_depth = depth;
  return s;
}

void sprite_free(sprite *sprite) {
  list_remove(sprite_layers[sprite->_depth], sprite->_node, free);
}

void sprite_set_depth(sprite *sprite, int depth) {
  // remove sprite from current layer but do not free it
  list_remove(sprite_layers[sprite->_depth], sprite->_node, NULL);
  sprite->_depth = depth;
  // push sprite onto new layer and set node
  sprite->_node = list_push(sprite_layers[depth], sprite);
}

void render_all_sprites() {
  for (int layer = 0; layer < SPRITE_LAYER_COUNT; layer++) {
    list *sprites = sprite_layers[layer];
    list_each(sprites, (list_lambda)draw_sprite);
  }
}

static void draw_sprite(sprite *s) {
  al_draw_tinted_scaled_rotated_bitmap(
      s->bitmap,                              // sprite bitmap
      s->tint,                                // sprite color
      s->center.x, s->center.y,               // center of bitmap
      s->position_ptr->x, s->position_ptr->y, // location to draw center to
      s->scale, s->scale,                     // x and y scaling
      *(s->angle_ptr),                        // rotation of entity
      0                                       // horiz/vert flip
  );
}

static list* get_sprite_layer(int layernum) {
  int layer = layernum + SPRITE_LAYER_LIMIT; // adjust to non-negative index
  if (layer >= SPRITE_LAYER_COUNT || layer < 0) {
    fprintf(stderr, "sprite layer %d is out of range [%d,%d]",
        layernum, -SPRITE_LAYER_LIMIT, SPRITE_LAYER_LIMIT);
    exit(-1);
  }
  return sprite_layers[layer];
}
