#ifndef COMPONENTS_H
#define COMPONENTS_H

/** \file components.h
  * \brief all component data structures that can fill an \ecs_component
**/

#include "util/geometry.h"
#include "ecs.h"

/** forward declaration of \ref ecs_entity, defined in \ref ecs.h */
struct ecs_entity;
typedef void (*ecs_entity_trigger)(struct ecs_entity *ent);

/** component giving physical properties to an \ref ecs_entity */
typedef struct Body {
  /** affects collision response */
  double mass;
  /** maximum linear velocity in px/sec */
  double max_linear_velocity;
  /** maximum rotational velocity in radians/sec */
  double max_angular_velocity;
} Body;

/** component allowing an \ref ecs_entity to collide */
typedef struct Collider {
  /** hit detection rectangle */
  rectangle rect;
} Collider;

/** component allowing an \ref ecs_entity to move */
typedef struct Propulsion {
  /** acceleration applied for linear movement (pixels/sec) */
  double linear_accel;
  /** acceleration applied for rotation (radians/sec) */
  double angular_accel;
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

#endif /* end of include guard: COMPONENTS_H */
