#include <assert.h>

#include "system/scenery_sys.h"
#include "util/al_helper.h"

const static vector cloud_min_scale = {1, 0.5};
const static vector cloud_max_scale = {45, 15};
const static double cloud_min_speed = 50;
const static double cloud_max_speed = 800;
const static int cloud_min_depth = -3;
const static int cloud_max_depth = 3;
const static double cloud_min_opacity = 0.1;
const static double cloud_max_opacity = 0.8;

// spawn a cloud every cloud_delay seconds
static double cloud_delay = 0.5;
static double cloud_timer;

// mountain settings
enum { NUM_MOUNTAIN_SPAWNERS = 2 };

struct mountain_spawner {
  const vector min_scale, max_scale;
  const int depth;
  const double speed;
  const double density;
  const char *sprite_name;
  double timer;
};

static struct mountain_spawner mountain_spawners[NUM_MOUNTAIN_SPAWNERS] = {
  {
    .min_scale = { 0.5, 0.8 },
    .max_scale = { 2.0, 3.0 },
    .depth = -4,
    .speed = 50,
    .density = 2.5,
    .sprite_name = "mountain"
  },
  {
    .min_scale = { 0.5, 0.7 },
    .max_scale = { 1.8, 2.4 },
    .depth = -3,
    .speed = 60,
    .density = 3,
    .sprite_name = "mountain-fg"
  }
};

static void make_cloud();
static ecs_entity* make_mountain(struct mountain_spawner *spawner);

void scenery_system_fn(double time) {
  cloud_timer -= time;
  if (cloud_timer <= 0) {
    make_cloud();
    cloud_timer = cloud_delay;
  }
  for (int i = 0; i < NUM_MOUNTAIN_SPAWNERS; i++) {
    if ((mountain_spawners[i].timer -= time) < 0) {
      make_mountain(&mountain_spawners[i]);
    }
  }
}

void scenery_pre_populate() {
  for (int i = 0; i < NUM_MOUNTAIN_SPAWNERS; i++) {
    struct mountain_spawner *spawner = &mountain_spawners[i];
    int x = 0;
    while (x < SCREEN_W) {
      ecs_entity *mountain = make_mountain(spawner);
      mountain->position.x = x;
      x += sprite_width(mountain->sprite) / 2 / spawner->density;
      spawner->timer = 0;
    }
  }
}

/** set frequency at which clouds spawn */
void scenery_sys_set_cloud_frequency(double clouds_per_sec) {
  assert(clouds_per_sec != 0);
  cloud_delay = 1 / cloud_max_depth;
}

void scenery_add_background(const char *name, int depth, double speed, int
    offset)
{
  ecs_entity *bg = ecs_entity_new((vector){SCREEN_W / 2 + offset, SCREEN_H / 2},
      ENTITY_SCENIC);
  ecs_attach_sprite(bg, name, depth);
  Body *body = &ecs_add_component(bg, ECS_COMPONENT_BODY)->body;
  make_constant_vel_body(body, (vector){-speed, 0});
}

static void make_cloud() {
  double start_y = randd(0, SCREEN_H);
  double start_x = SCREEN_W + 200;
  int depth = randi(cloud_min_depth, cloud_max_depth);
  double speed = -randd(cloud_min_speed, cloud_max_speed);
  double alpha = randd(cloud_min_opacity, cloud_max_opacity);
  ecs_entity *cloud = ecs_entity_new((vector){.x = start_x, .y = start_y},
      ENTITY_SCENIC);
  sprite* s = ecs_attach_sprite(cloud, "cloud", depth);
  s->scale = (vector){
    randd(cloud_min_scale.x, cloud_max_scale.y),
    randd(cloud_min_scale.x, cloud_max_scale.y)
  };
  s->tint.a = alpha;
  Body *body = &ecs_add_component(cloud, ECS_COMPONENT_BODY)->body;
  make_constant_vel_body(body, (vector){speed, 0});
  body->destroy_on_exit = WEST;
}

static ecs_entity* make_mountain(struct mountain_spawner *spawner) {
  ecs_entity *mountain = ecs_entity_new(ZEROVEC, ENTITY_SCENIC);
  sprite* s = ecs_attach_sprite(mountain, spawner->sprite_name, spawner->depth);
  s->scale = (vector){
    randd(spawner->min_scale.x, spawner->max_scale.x),
    randd(spawner->min_scale.y, spawner->max_scale.y)
  };
  mountain->position = (vector) {
    SCREEN_W + sprite_width(s) / 2,
    SCREEN_H - sprite_height(s) / 2
  };
  Body *body = &ecs_add_component(mountain, ECS_COMPONENT_BODY)->body;
  make_constant_vel_body(body, (vector){-spawner->speed, 0});
  body->destroy_on_exit = WEST;
  spawner->timer = sprite_width(s) / spawner->speed / spawner->density;
  return mountain;
}

void scenery_make_explosion(vector pos, vector size, double anim_rate,
    ALLEGRO_COLOR tint, const char *sound_name)
{
  ecs_entity *boom = ecs_entity_new(pos, ENTITY_EXPLOSION);
  sprite *anim = ecs_attach_animation(boom, "explosion", 1, 32, 32,
      anim_rate, ANIMATE_ONCE);
  anim->scale = size;
  anim->tint = tint;
  al_game_play_sound(sound_name, false); // false: dont loop
}
