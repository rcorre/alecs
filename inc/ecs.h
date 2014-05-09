#ifndef ECS_H
#define ECS_H

/** \file ecs.h
  * \brief \b Entity-Component-System framework for \b Allegro5
**/

#include "component/sprite.h"
#include "util/list.h"
#include "util/geometry.h"

/* struct declarations ********************************************************/
/** tag to identify the type of a \ref ecs_component.
 *  values must start at 0 and increment by 1 up to \ref NUM_COMPONENT_TYPES */
typedef enum ecs_component_type {
  COMP_SPRITE = 0,
  COMP_BODY,
  COMP_COLLIDER,
  COMP_PROPULSION,
  NUM_COMPONENT_TYPES
} ecs_component_type;

/** component which can be attached to an \ref entity - Plain Old Data. 
 *  components should contain no data that needs to be freed - Any such
 *  data (e.g. a bitmap) should be allocated and freed independently of
 *  component creation/destruction. Components should simply reference such
 *  data from another data store
 */
typedef struct ecs_component {
  /** component type identifier */
  ecs_component_type type;
  /** reference to entity that owns this component.
   *  can be used to locate sibling components */
  struct ecs_entity *owner_entity;
  /** reference to location in \ref ecs_component_store.
   *  used to remove from \ref ecs_component_store upon freeing component. */
  list_node *node;
  /** data for a given component type */
  union {
    struct sprite *sprite;
  };
} component;

/** a game object composed of \ref ecs_components */
typedef struct ecs_entity {
  /** absolute location of entity's center */
  vector position;
  /** rotation of entity about its center (\b radians) */
  double angle;
  /** components indexed by \ref ecs_component_type */
  struct ecs_component *components[NUM_COMPONENT_TYPES];
} ecs_entity;

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
/** active component lists indexed by \ref ecs_component_type */
extern list *ecs_component_store[NUM_COMPONENT_TYPES];
/** list of \ref ecs_systems that will be run during gameplay */
extern list *ecs_system_list;
/******************************************************************************/

/* functions ******************************************************************/
/** initialize entity-component-system framework */
void ecs_init();
/** create new entity with no attached components.
  * \param position initial location of the center point of the new entity
  * \return a new \ref ecs_entity. free with \ref ecs_entity_free.
**/
ecs_entity *ecs_entity_new(vector position);
/** free an entity and every \ref ecs_component attached to it
  * \param entity entity to be destroyed
**/
void ecs_entity_free(ecs_entity *entity);
/** attach a new component to an entity
  * \param entity \ref ecs_entity to which the component will be attached
  * \param type type of component to attach
  * \return an \b UNINITIALIZED component.  The general component fields (\ref
  * owner_entity, \ref type, \ref node) will be set, but the type-specific
  * fields must be initialized by the caller.
**/
struct ecs_component* ecs_add_component(ecs_entity *entity, 
    ecs_component_type type);
/** remove the component of a given type from an entity
  * \param entity entity from which to remove component
  * \param type type of component to remove. If entity has no such component,
  * \ref ecs_remove_component does nothing.
**/
void ecs_remove_component(ecs_entity *entity, ecs_component_type type);
/******************************************************************************/
#endif /* end of include guard: ECS_H */
