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
  ECS_COMPONENT_TIMER,
  ECS_COMPONENT_BEHAVIOR,
  ECS_COMPONENT_KEYBOARD_LISTENER,
  ECS_COMPONENT_MOUSE_LISTENER,
  NUM_COMPONENT_TYPES
} ecs_component_type;

typedef enum BehaviorType {
  BEHAVIOR_IDLE,   ///< hover around a bit
  BEHAVIOR_FOLLOW, ///< move towards another entity
  BEHAVIOR_MOVE    ///< move towards a location
} BehaviorType;

/** forward declaration of \ref ecs_entity, defined in \ref ecs.h */
struct ecs_entity;
/** function pointer attached to a \ref ecs_component.
 *  when called, it is passed the owner entity*/
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
  /** maximum linear velocity in px/sec */
  double max_linear_velocity;
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
  /** action to take when a collision is detected. both entities involved in
   * the collision are passed as arguments */
  void (*on_collision)(struct ecs_entity *ent1, struct ecs_entity *ent2);
} Collider;

/** component allowing an \ref ecs_entity to move */
typedef struct Propulsion {
  /** if true, propulsion applies in direction of unit angle */
  bool directed;
  /** acceleration applied for linear movement (pixels/sec) */
  double linear_accel;
  /** angular velocity achieved when turning */
  double turn_rate;
  /** factor by which to apply horizontal and vertical acceleration.
   *  +/-1.0 is full thrust in +/- x/y direction, zero vector means off */
  vector linear_throttle;
  /** factor by which to apply turn rate to body's angle
   *  +/-1.0 is full turn_rate clockwise/counterclockwise, 0 is off */
  double angular_throttle;
  particle_generator particle_effect;
} Propulsion;

/** comonent indicating that an \ref ecs_entity can take damage */
typedef struct Health {
  /** health points remaining - initialized to \ref max_hp */
  double hp;
  /** delegate to run when health hits or passes 0 */
  ecs_entity_trigger on_disable;
} Health;

/** comonent that triggers a change in an \ref ecs_entity after some time */
typedef struct Timer {
  /** maximum health point count */
  double time_left;
  /** action to perform on owner entity when \ref time_left == 0 */
  ecs_entity_trigger timer_action;
} Timer;

/** comonent that causes an entity to act autonomously */
typedef struct Behavior {
  BehaviorType type;
  struct ecs_entity *target;  ///< target of interest
  vector location; ///< location of interest
} Behavior;

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
