#ifndef ECS_H
#define ECS_H

/** \file ecs.h
  * \brief \b Entity-Component-System framework for \b Allegro5
**/

#include "components.h"
#include "render.h"
#include "util/list.h"
#include "util/geometry.h"
#include "system/scenery_sys.h"
#include "system/body_sys.h"

/* struct declarations ********************************************************/
/** tag to identify the type of a \ref ecs_component.
 *  values must start at 0 and increment by 1 up to \ref NUM_COMPONENT_TYPES */
typedef enum ecs_component_type {
  ECS_COMPONENT_BODY,
  ECS_COMPONENT_COLLIDER,
  ECS_COMPONENT_PROPULSION,
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
    Body body;
    Collider collider;
    Propulsion propulsion;
    Health health;
    Timer timer;
  };
} ecs_component;

/** a game object composed of \ref ecs_components */
typedef struct ecs_entity {
  /** absolute location of entity's center */
  vector position;
  /** rotation of entity about its center (\b radians) */
  double angle;
  /** components indexed by \ref ecs_component_type */
  struct ecs_component *components[NUM_COMPONENT_TYPES];
  /** sprite determining how entity is rendered.
   *  may be NULL to if entity has no visual representation. */
  sprite *sprite;
  /** pointer to node in entity list - DO NOT MODIFY */
  list_node *_node;
} ecs_entity;
/******************************************************************************/

/* Typedefs *******************************************************************/
/** a system is just a function called every frame which updates the state of
 *  some components based on elapsed time */
typedef void (*ecs_system)(double time);
/******************************************************************************/

/* global entity-component data stores ****************************************/
/** active component lists indexed by \ref ecs_component_type */
extern list *ecs_component_store[NUM_COMPONENT_TYPES];
/** list of every \ref ecs_system update function.
 *  the handlers are called every frame in order from the list head to tail */
extern list *ecs_systems;
/** list of every active \ref ecs_entity. */
extern list *ecs_entities;
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
ecs_component* ecs_add_component(ecs_entity *entity, ecs_component_type type);

/** remove the component of a given type from an entity
  * \param entity entity from which to remove component
  * \param type type of component to remove. If entity has no such component,
  * \ref ecs_remove_component does nothing.
**/
void ecs_remove_component(ecs_entity *entity, ecs_component_type type);

/** attach a sprite to an entity so it can be rendered
  * \param entity entity to which sprite should be attached
  * \param name key to identify sprite
  * \param depth layer at which sprite should be drawn
  * \return the newly created and attached sprite
**/
sprite* ecs_attach_sprite(ecs_entity *entity, const char *name, int depth);

/** remove and free sprite attached to an entity
  * \param entity entity from which to remove sprite
**/
void ecs_remove_sprite(ecs_entity *entity);

/** call every \ref ecs_system function in \ref ecs_systems.
 *  \param time time elapsed since last update call (seconds) */
void ecs_update_systems(double time);

/** free every active \ref ecs_entity and every attached \ref ecs_component */
void ecs_free_all_entities();

/** clean up all resources allocated by the ECS framework. */
void ecs_shutdown();
/******************************************************************************/
#endif /* end of include guard: ECS_H */
