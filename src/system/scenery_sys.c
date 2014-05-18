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

static void make_cloud();

void scenery_system_fn(double time) {
  cloud_timer -= time;
  if (cloud_timer <= 0) {
    make_cloud();
    cloud_timer = cloud_delay;
  }
}

/** set frequency at which clouds spawn */
void scenery_sys_set_cloud_frequency(double clouds_per_sec) {
  assert(clouds_per_sec != 0);
  cloud_delay = 1 / cloud_max_depth;
}

static void make_cloud() {
  double start_y = randd(0, SCREEN_H);
  double start_x = SCREEN_W + 200;
  int depth = randi(cloud_min_depth, cloud_max_depth);
  double speed = -randd(cloud_min_speed, cloud_max_speed);
  double alpha = randd(cloud_min_opacity, cloud_max_opacity);
  ecs_entity *cloud = ecs_entity_new((vector){.x = start_x, .y = start_y});
  sprite* s = ecs_attach_sprite(cloud, "cloud", depth);
  s->scale = (vector){
    randd(cloud_min_scale.x, cloud_max_scale.y),
    randd(cloud_min_scale.x, cloud_max_scale.y)
  };
  s->tint.a = alpha;
  Body *body = &ecs_add_component(cloud, ECS_COMPONENT_BODY)->body;
  make_constant_vel_body(body, (vector){speed, 0}, 0);
  body->destroy_on_exit = WEST;
}
