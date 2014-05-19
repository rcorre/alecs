#ifndef PARTICLE_EFFECTS_H
#define PARTICLE_EFFECTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "al_game.h"
#include "util/geometry.h"
#include "util/al_helper.h"

/**
 * \file particle_generator.h
 * Interface for the particle engine
 */

/**
 * \struct generator_data
 */
typedef struct generator_data {
  char *name;                           /*!< unique id */
  double spawn_rate;                    /*!< particles/sec */
  double spawn_velocity;                /*!< velocity of particles px/sec */
  double max_velocity;                  /*!< maximum velocity of a particle */
  double deceleration;                  /*!< proportion of speed loss / second */
  double min_duration, max_duration;    /*!< lifetime of particles (sec) */
  double start_radius;                  /*!< radius(px) at start of life */
  double end_radius;                    /*!< radius(px) at end of life */
  double spawn_arc;                     /*!< arc (radians) of spawn direction */
  ALLEGRO_COLOR start_color, end_color; /*!< color at start and end of life */
} generator_data;

typedef struct particle_generator {
  generator_data *data; /*!< data used to determine generator properties */
  vector position;      /*!< location of generator */
  double angle;         /*!< angle of generator (radians) */
  //! accumulate "partial" spawns (where spawn_density * time is not an integer)
  double _spawn_counter;
} particle_generator;

typedef struct particle {
  vector position;            /*!< location of particle in world */
  vector velocity;            /*!< change in position (px/sec) */
  double time_alive;          /*!< seconds particle has spent alive */
  double time_to_live;        /*!< total number of seconds for which to exist */
  double radius;              /*!< radius of particle (pixels) */
  ALLEGRO_COLOR color;        /*!< color of particle. changes over time */
  const generator_data* data; /*!< reference to data used to create particle */
} particle;

/*! \fn void particle_init(ALLEGRO_BITMAP *display)
    \brief set up particle engine
    \param display pointer to the main display
 */
void particle_init(ALLEGRO_BITMAP *display);
// load all generator datas, return list of all of generator names
list* load_all_generator_data();
// retrieve a generator with which to spawn particles
particle_generator* get_particle_generator(char *name);
// free memory of a particle generator, unregister it from generator list
void free_particle_generator(particle_generator* gen);
// create particles at a specified position using a generator
// density multiplies the generator's default spawn rate. Use 1.0 for default
void spawn_particles(particle_generator *gen, double time, double density);
// call once for each update
void update_particles(double time);
// call once during each draw
void draw_particles();
// remove all particles
void clear_particles();
// remove all particles and generator datas.
void particle_shutdown();
// return number of active particles
int get_particle_count();

#endif /* end of include guard: PARTICLE_EFFECTS_H */
