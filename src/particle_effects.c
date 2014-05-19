#include "particle_effects.h"

/* Static Variables ----------------------------------------------------------*/
static const int BMP_SIZE = 32;
static const char* DATA_PATH = "data/particle_effects.cfg";
static ALLEGRO_BITMAP *particle_bitmap;
// threading
static double time_elapsed; // time elapsed for current update cycle
/* ---------------------------------------------------------------------------*/

/* Internal Structs ----------------------------------------------------------*/

// node in a doubly linked list of particles
static list *particle_list;  // list of particles
static list *data_list;      // list of generator datas
/* ---------------------------------------------------------------------------*/

/* Helpers -------------------------------------------------------------------*/
static ALLEGRO_COLOR string_to_color(const char *colorstr) {
  unsigned char vals[4] = {0};  // rgba color values
  char str[20];                 // create copy of string to tokenize
  strcpy(str, colorstr);        // copy input string into str
  char *tok = strtok(str, ","); // get first token
  for (int i = 0; i < 4; ++i) {
    if (tok == NULL) {
      fprintf(stderr, "error parsing color %s. missing rgba val at index %d\n",
          colorstr, i);
    }
    else {
      vals[i] = (unsigned char)atoi(tok); // convert token to one of rgba
      tok = strtok(NULL, ",");            // get next value
    }
  }
  return al_map_rgba(vals[0],vals[1],vals[2],vals[3]);
}

static generator_data* load_generator_data(char const *name,
    ALLEGRO_CONFIG *cfg) {
  generator_data *data = malloc(sizeof(generator_data));
  data->name = malloc(strlen(name) + 1);
  strcpy(data->name, name);
  data->spawn_rate = atof(al_get_config_value(cfg, name, "spawn_rate"));
  data->spawn_velocity = atof(al_get_config_value(cfg, name, "spawn_velocity"));
  data->max_velocity = atof(al_get_config_value(cfg, name, "max_velocity"));
  data->deceleration = atof(al_get_config_value(cfg, name, "deceleration"));
  data->min_duration = atof(al_get_config_value(cfg, name, "min_duration"));
  data->max_duration = atof(al_get_config_value(cfg, name, "max_duration"));
  data->start_radius = atof(al_get_config_value(cfg, name, "start_radius"));
  data->end_radius = atof(al_get_config_value(cfg, name, "end_radius"));
  data->spawn_arc = DEGTORAD(atof(al_get_config_value(cfg, name, "spawn_arc")));
  data->start_color = string_to_color(
      al_get_config_value(cfg, name, "start_color"));
  data->end_color = string_to_color(
      al_get_config_value(cfg, name, "end_color"));
  return data;
}

static void data_free_fn(void *data) {
  free(((generator_data *)data)->name);
  free(data);
}

// helper to set up particle attributes
static particle* make_particle(generator_data *data, vector pos, double angle) {
  double angle1 = angle - data->spawn_arc / 2;
  double angle2 = angle + data->spawn_arc / 2;
  particle *p = malloc(sizeof(particle));
  p->position = pos;
  p->velocity = rand_vec(angle1, angle2, 0, data->spawn_velocity);
  p->time_alive = 0;
  p->time_to_live = randd(data->min_duration, data->max_duration);
  p->radius = data->start_radius;
  p->color = data->start_color;
  p->data = data;
  return p;
}

static void update_particle(particle *p) {
  const generator_data *data = p->data;
  double factor = p->time_alive / p->time_to_live;
  double time = time_elapsed;
  p->velocity = vector_scale(p->velocity, 1 - data->deceleration * time);
  p->velocity = vector_clamp(p->velocity, data->max_velocity);
  p->position = vector_add(p->position, vector_scale(p->velocity, time));
  p->time_alive += time;
  p->radius = data->start_radius + factor * data->end_radius;
  p->color = lerp_color(data->start_color, data->end_color, factor);
}
/* ---------------------------------------------------------------------------*/

/* Public Interface ----------------------------------------------------------*/
void particle_init(ALLEGRO_BITMAP *display) {
  // create the bitmap to be used for particles
  particle_bitmap = al_create_bitmap(BMP_SIZE, BMP_SIZE);
  al_set_target_bitmap(particle_bitmap);  // draw a circle onto the bitmap
  al_draw_filled_circle(BMP_SIZE/2, BMP_SIZE/2, BMP_SIZE/2,
      al_map_rgba(255,255,255,255));
  al_set_target_bitmap(display); // set target back to main display
  data_list = list_new();        // create list to store data
  particle_list = list_new();    // create list to store particles
}

list* load_all_generator_data() {
  list *names = list_new();
  ALLEGRO_CONFIG *cfg = al_load_config_file(DATA_PATH); // open config file
  ALLEGRO_CONFIG_SECTION *section = NULL;
  al_get_first_config_section(cfg, &section);
  char const *name = al_get_next_config_section(&section);
  while (name != NULL) {
    load_generator_data(name, cfg);
    list_push(names, strdup(name));
    name = al_get_next_config_section(&section);
  }
  al_destroy_config(cfg);
  return names;
}

particle_generator get_particle_generator(char *name) {
  list_node *p = data_list->head;
  generator_data *dat = NULL;
  while (p != NULL && strcmp(((generator_data*)p->value)->name, name) != 0) {
    p = p->next; // search for data matching the given name
  }
  if (p == NULL) {                                        // did not find data
    ALLEGRO_CONFIG *cfg = al_load_config_file(DATA_PATH); // open config file
    dat = load_generator_data(name, cfg);                 // load data from file
    list_push(data_list, dat);                            // cache data in list
    al_destroy_config(cfg);
  }
  else { // found data
    dat = (generator_data*)p->value;
  }
  return (particle_generator) {
    .data = dat, .position = {0,0}, .angle = 0, ._spawn_counter = 0
  };
}

void spawn_particles(particle_generator *gen, double time, double density) {
  generator_data *data = gen->data;
  double spawn_count = gen->_spawn_counter + data->spawn_rate * density * time;
  // place excess in spawn counter
  gen->_spawn_counter = spawn_count - (int)spawn_count;
  // spawn a whole number of particles
  for (int i = 0; i < (int)spawn_count; i++) {
    // create particle and copy data to node's storage
    particle *p = make_particle(gen->data, gen->position, gen->angle);
    list_push(particle_list, p);
  }
}

void update_particles(double time) {
  time_elapsed = time;
  list_node *pnode = particle_list->head;
  while (pnode != NULL) {
    particle *p = (particle*)(pnode->value); // particle struct in node
    if (p->time_alive > p->time_to_live) {   // has particle expired?
      pnode = list_remove(particle_list, pnode, free); // remove
    }
    else {                 // not expired - perform update
      update_particle(p);  // update particle attributes
      pnode = pnode->next; // move to next particle
    }
  }
}

void draw_particles() {
  list_node *node = particle_list->head;
  al_hold_bitmap_drawing(true);  // better performance for repeated draws
  while (node != NULL) {
    particle *p = (particle*)(node->value);
    al_draw_tinted_scaled_bitmap(
        particle_bitmap,              // bitmap
        p->color,                     // tint
        0, 0, BMP_SIZE, BMP_SIZE,     // source coordinates
        p->position.x, p->position.y, // destination coordinates
        p->radius, p->radius,         // scale
        0                             // flags
        );
    node = node->next;
  }
  al_hold_bitmap_drawing(false);
}

// remove all particles
void clear_particles() {
  list_free(particle_list, free);
  particle_list = list_new();
}

void particle_shutdown() {
  if (particle_bitmap != NULL) {
    al_destroy_bitmap(particle_bitmap);
  }
  // remove all force sources, generator datas, and particles
  list_free(particle_list, free);
  list_free(data_list, data_free_fn);
}

int get_particle_count() {
  return particle_list->length;
}
/* ---------------------------------------------------------------------------*/
