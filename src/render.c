#include "render.h"

// each layer stores a list of sprites to be drawn at that layer
list *sprite_layers[SPRITE_LAYER_LIMIT * 2 + 1];

static double elapsed_time; // time for current update (draw) call
static void draw_sprite(sprite *s);
static list* get_sprite_layer(int layernum);
static ALLEGRO_FONT *debug_font;

void sprite_init() {
  for (int i = 0; i < SPRITE_LAYER_COUNT; i++) {
    assert(sprite_layers[i] == NULL); // assert not already initialized
    sprite_layers[i] = list_new();    // create a new list for each depth layer
  }

#ifndef NDEBUG
  debug_font = al_game_get_font("LiberationMono-Regular");
#endif
}

void sprite_shutdown() {
  for (int i = 0; i < SPRITE_LAYER_LIMIT * 2 + 1; i++) {
    list_free(sprite_layers[i], (list_lambda)sprite_free);
  }
}

sprite* sprite_new(const char *name, vector *ref_position, double *ref_angle, 
    int depth) {
  sprite *s = calloc(1, sizeof(sprite));
  ALLEGRO_BITMAP *bmp = al_game_get_bitmap(name);
  assert(bmp != NULL);
  s->bitmap = bmp;
  s->center = (vector) {
    .x = al_get_bitmap_width(bmp) / 2,
    .y = al_get_bitmap_height(bmp) / 2
  };
  s->scale = (vector){1, 1};
  s->tint = al_map_rgb(255,255,255);
  s->position_ptr = ref_position;
  s->angle_ptr = ref_angle;
  // give sprite back-reference to its node so it may be removed when freed
  s->_node = list_push(get_sprite_layer(depth), s);
  s->_depth = depth;
  // default to using full sprite
  s->frame_width = al_get_bitmap_width(bmp);
  s->frame_height = al_get_bitmap_height(bmp);
  // TODO: set animation timer
  return s;
}

void sprite_free(sprite *sprite) {
  list_remove(get_sprite_layer(sprite->_depth), sprite->_node, free);
}

void sprite_set_depth(sprite *sprite, int depth) {
  // remove sprite from current layer but do not free it
  list_remove(get_sprite_layer(sprite->_depth), sprite->_node, NULL);
  sprite->_depth = depth;
  // push sprite onto new layer and set node
  sprite->_node = list_push(get_sprite_layer(depth), sprite);
}

void render_all_sprites(double time) {
  elapsed_time = time;
  for (int layer = 0; layer < SPRITE_LAYER_COUNT; layer++) {
    list *sprites = sprite_layers[layer];
    list_each(sprites, (list_lambda)draw_sprite);
  }
}

static void draw_sprite(sprite *s) {
  if (s->animation_type != ANIMATE_OFF) {
    s->_animation_timer -= elapsed_time;
    if (s->_animation_timer < 0) {
      s->_animation_timer = 1 / s->animation_rate;
      // move to next frame, check if it is past the last frame
      if (++(s->current_frame) == sprite_num_frames(s)) {
        switch(s->animation_type) {
          case ANIMATE_ONCE:
            s->animation_type = ANIMATE_OFF;
            break;
          case ANIMATE_LOOP:
            s->current_frame = 0;
            break;
          default:
            break;
        }
      }
    }
  }

  int sx = s->current_frame * s->frame_width; // section x coordinate
  
  al_draw_tinted_scaled_rotated_bitmap_region(
      s->bitmap,                              // sprite bitmap
      sx, 0, s->frame_width, s->frame_height, // section
      s->tint,                                // sprite color
      s->center.x, s->center.y,               // center of bitmap
      s->position_ptr->x, s->position_ptr->y, // location to draw center to
      s->scale.x, s->scale.y,                 // x and y scaling
      *(s->angle_ptr),                        // rotation of entity
      0                                       // horiz/vert flip
  );
#ifndef NDEBUG
  al_draw_textf(debug_font, al_map_rgb(255,0,0), s->position_ptr->x, 
      s->position_ptr->y, 0, "angle: %3.3f", *s->angle_ptr);
#endif
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

int sprite_width(sprite *s) {
  return al_get_bitmap_width(s->bitmap) * s->scale.x;
}

int sprite_height(sprite *s) {
  return al_get_bitmap_height(s->bitmap) * s->scale.y;
}

int sprite_num_frames(sprite *s) {
  return al_get_bitmap_width(s->bitmap) / s->frame_width;
}
