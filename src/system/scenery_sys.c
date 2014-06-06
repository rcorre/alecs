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
const static vector mountain_min_scale = {0.5, 0.5};
const static vector mountain_max_scale = {1.5, 2.5};
const static int mountain_min_depth = -4;
const static int mountain_max_depth = -2;
const static double mountain_speed = 150;
const static double mountain_density = 3;
static double mountain_timer;

static void make_cloud();
static void make_mountain();

void scenery_system_fn(double time) {
  cloud_timer -= time;
  if (cloud_timer <= 0) {
    make_cloud();
    cloud_timer = cloud_delay;
  }
  mountain_timer -= time;
  if (mountain_timer <= 0) {
    make_mountain();
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

static void make_mountain() {
  int depth = randi(mountain_min_depth, mountain_max_depth);
  ecs_entity *mountain = ecs_entity_new(ZEROVEC, ENTITY_SCENIC);
  sprite* s = ecs_attach_sprite(mountain, "mountain", depth);
  s->scale = (vector){
    randd(mountain_min_scale.x, mountain_max_scale.y),
    randd(mountain_min_scale.x, mountain_max_scale.y)
  };
  mountain->position = (vector) {
    SCREEN_W + sprite_width(s) / 2,
    SCREEN_H - sprite_height(s) / 2
  };
  Body *body = &ecs_add_component(mountain, ECS_COMPONENT_BODY)->body;
  make_constant_vel_body(body, (vector){-mountain_speed, 0});
  body->destroy_on_exit = WEST;
  mountain_timer = sprite_width(s) / mountain_speed / mountain_density;
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
