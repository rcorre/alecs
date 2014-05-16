#include <assert.h>

#include "system/scenery_sys.h"
#include "util/al_helper.h"

const static double cloud_min_scale = 0.5;
const static double cloud_max_scale = 2.0;
const static double cloud_min_vel = 50;
const static double cloud_max_vel = 200;
const static int cloud_min_depth = -5;
const static int cloud_max_depth = 5;
const static int cloud_min_alpha = 20;
const static int cloud_max_alpha = 150;

// spawn a cloud every cloud_delay seconds
static double cloud_delay = 1.0;
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
  int depth = randi(cloud_min_depth, cloud_max_depth);
  double scale = randd(cloud_min_scale, cloud_max_scale);
  double vel = randd(cloud_min_vel, cloud_max_vel);
  double alpha = randi(cloud_min_alpha, cloud_max_alpha);
  ecs_entity *cloud = ecs_entity_new((vector){.x = SCREEN_W, .y = start_y});
  sprite* s = ecs_attach_sprite(cloud, "cloud", depth);
  s->scale = scale;
  s->tint.a = alpha;
}
