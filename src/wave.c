#include "wave.h"
#include "entity/enemies.h"

static list *waves;
static ecs_entity *player_entity;
static void wave_spawn_enemies(EnemyWave *wave);

//TODO: load from cfg file
static EnemyWave wave1 = {
  .spawn_fn = spawn_enemy,
  .quantity = 4,
  .spawn_side = NORTH,
  .exit_side = WEST,
  .start_time = 3,
  .duration = 10
};

void start_enemy_waves(struct ecs_entity *player) {
  player_entity = player;
  waves = list_new();
  EnemyWave *wave = malloc(sizeof(EnemyWave));
  memcpy(wave, &wave1, sizeof(EnemyWave));
  list_push(waves, wave);
}

bool update_enemy_waves(double time) {
  list_node *node = waves->head; 
  if (!node) { return false; } // all waves complete
  while(node) {
    EnemyWave *wave = node->value;
    if (wave->start_time < 0) {
      wave_spawn_enemies(wave);
      node = list_remove(waves, node, free);
    }
    else {
      wave->start_time -= time; // count down to start
      node = node->next;
    }
  }
  return true;
}

void stop_enemy_waves() {
  list_free(waves, free);
}

static void wave_spawn_enemies(EnemyWave *wave) {
  if (wave->spawn_side & (NORTH | SOUTH)) {
    for (int i = 0; i < wave->quantity; i++) {
      // start coordinates
      int sx = (i + 1) * SCREEN_W / (wave->quantity + 1);
      int sy = -(i + 1) * 40;
      // destination coordinates
      int dx = sx;
      int dy = 60;
      EnemySpawnData data = {
        .start = (vector){sx, sy},
        .target = (vector){dx, dy},
        .exit = (vector){sx, sy},
        .duration = wave->duration,
        .player = player_entity
      };
      wave->spawn_fn(data);
    }
  }
}
