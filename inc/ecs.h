#ifndef ECS_H
#define ECS_H

#include "component/sprite.h"
#include "util/list.h"
#include "util/geometry.h"

/* struct declarations ********************************************************/
/** tag to identify the type of a \ref ecs_component */
typedef enum ecs_component_type {
  COMP_SPRITE,
  COMP_BODY,
  COMP_COLLIDER,
  COMP_PROPULSION,
  NUM_COMPONENT_TYPES
} ecs_component_type;

/** tag to identify the type of a \ref ecs_system */
typedef enum ecs_system_type {
  SYS_SPRITE,
  SYS_BODY,
  SYS_COLLIDER,
  SYS_PROPULSION,
  NUM_SYSTEM_TYPES
} ecs_system_type;

/** component which can be attached to an \ref entity. Plain Old Data. */
typedef struct ecs_component {
  ecs_component_type type;
  struct ecs_entity *owner_entity;
  list_node *node;
  union {
    struct sprite *sprite;
  };
} component;

/** component which can be attached to an \ref ecs_entity. 
 * <em>Plain Old Data.</em> 
 **/
typedef struct ecs_entity {
  vector position;
  double angle;
  struct ecs_component components[NUM_COMPONENT_TYPES];
} ecs_entity;

/** contains functions to create or destroy a certain kind of \ref
 *  ecs_component */
typedef struct ecs_component_manager {
  struct ecs_component *(*new)(struct ecs_component *template);
  void (*free)(struct ecs_component *comp);
  void (*load_templates)(void);
  void (*free_templates)(void);
} ecs_component_manager;

/** performs an update and possibly draw routine every frame, modifying the
 *  state of components in the game world */
typedef struct ecs_system {
  /** system update function. Called every frame. 
   *  takes time elapsed since last update as only argument */
  void (*update)(double elapsed_time);
  /** system rendering function. Called every frame after update cycle. 
   *  may be \c NULL to indicate that the system does not render anything. */
  void (*draw)(void);
} ecs_system;
/******************************************************************************/

/* global entity-component data stores ****************************************/
/** component managers indexed by \ref ecs_component_type */
extern ecs_component_manager ecs_component_managers[NUM_COMPONENT_TYPES];
/** active component lists indexed by \ref ecs_component_type */
extern list *ecs_component_store[NUM_COMPONENT_TYPES];
/** systems indexed by \ref ecs_system_type */
extern ecs_system ecs_systems[NUM_SYSTEM_TYPES];
/******************************************************************************/

/* functions ******************************************************************/
/** initialize entity-component-system framework */
void ecs_init();
/** create new entity with no attached components.
  * \return a new \ref ecs_entity. free with \ref ecs_destroy_entity.
**/
ecs_entity *ecs_create_entity();
/** free an entity and every \ref ecs_component attached to it
  * \param entity entity to be destroyed
**/
void ecs_destroy_entity(ecs_entity *entity);
/** attach a new component to an entity
  * \param entity \ref ecs_entity to which the component will be attached
  * \param template template acquired via \ref ecs_get_component_template which
  * will be used to construct the new component
**/
void ecs_attach_component(ecs_entity *entity, struct ecs_component *template);
/** acquire a component template needed to create components of a certain type
  * \param type \ref ecs_component_type of component
  * \return template component used to construct compoonents
**/
struct ecs_component ecs_get_component_template(ecs_component_type type, 
    const char *name);
/******************************************************************************/
#endif /* end of include guard: ECS_H */
