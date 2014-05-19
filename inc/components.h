#ifndef COMPONENTS_H
#define COMPONENTS_H

/** \file components.h
  * \brief all component data structures that can fill an \ecs_component
  * A component should not contain any data that must be freed upon its
  * construction
**/

#include <stdbool.h>
#include "util/geometry.h"
#include "ecs.h"
#include "particle_effects.h"

/** tag to identify the type of a \ref ecs_component.
 *  values must start at 0 and increment by 1 up to \ref NUM_COMPONENT_TYPES */
typedef enum ecs_component_type {
  ECS_COMPONENT_BODY,
  ECS_COMPONENT_COLLIDER,
  ECS_COMPONENT_PROPULSION,
  ECS_COMPONENT_HEALTH,
  ECS_COMPONENT_KEYBOARD_LISTENER,
  ECS_COMPONENT_MOUSE_LISTENER,
  NUM_COMPONENT_TYPES
} ecs_component_type;

/** forward declaration of \ref ecs_entity, defined in \ref ecs.h */
struct ecs_entity;
typedef void (*ecs_entity_trigger)(struct ecs_entity *ent);
typedef void (*ecs_mouse_handler)(struct ecs_entity *ent, bool lmb, bool rmb);
/** action to be taken on keypress
 *  \param ent pointer to the entity that holds the keyboard handler
 *  \param keycode code of pressed key
 *  \param down true if key was pressed, false if released */
typedef void (*ecs_keyboard_handler)(struct ecs_entity *entity, int keycode, 
    bool down);

/** component giving physical properties to an \ref ecs_entity */
typedef struct Body {
  /** affects collision response */
  double mass;
  /** rate of change in x and y coordinates (px/sec) */
  vector velocity;
  /** rate of change in angle (radians) */
  double angular_velocity;
  /** maximum linear velocity in px/sec */
  double max_linear_velocity;
  /** maximum rotational velocity in radians/sec */
  double max_angular_velocity;
  /** the entity will be destroyed if it leaves the screen from any of the
   * provided directions */
  Direction destroy_on_exit;
} Body;

/** component allowing an \ref ecs_entity to collide */
typedef struct Collider {
  /** hit detection rectangle */
  rectangle rect;
  /** if true, owner will bounce when colliding with level bounds */
  bool keep_inside_level;
} Collider;

/** component allowing an \ref ecs_entity to move */
typedef struct Propulsion {
  /** acceleration applied for linear movement (pixels/sec) */
  double linear_accel;
  /** acceleration applied for rotation (radians/sec) */
  double angular_accel;
  /** factor by which to apply horizontal and vertical acceleration.
   *  +/-1.0 is full thrust in +/- x/y direction, zero vector means off */
  vector linear_throttle;
  /** factor by which to apply angular acceleration
   *  +/-1.0 is full angular_accel clockwise/counterclockwise, 0 is off */
  double angular_throttle;
  particle_generator particle_effect;
} Propulsion;

/** comonent indicating that an \ref ecs_entity can take damage */
typedef struct Health {
  /** maximum health point count */
  double max_hp;
  /** health points remaining - initialized to \ref max_hp */
  double hp;
} Health;

/** comonent that triggers a change in an \ref ecs_entity after some time */
typedef struct Timer {
  /** maximum health point count */
  double time_left;
  /** action to perform on owner entity when \ref time_left == 0 */
  ecs_entity_trigger timer_trigger;
} Timer;

typedef struct KeyboardListener {
  ecs_keyboard_handler handler;
} KeyboardListener;

typedef struct MouseListener {
  /** area in which to register clicks */
  rectangle click_rect;
  /** action to take on mouse entering \ref click_rect */
  ecs_entity_trigger on_enter;
  /** action to take on mouse leaving \ref click_rect */
  ecs_entity_trigger on_leave;
  /** action to take when mouse button pressed inside \ref click_rect */
  ecs_mouse_handler on_press;
  /** action to take when mouse button released inside \ref click_rect */
  ecs_mouse_handler on_release;
} MouseListener;

#endif /* end of include guard: COMPONENTS_H */
