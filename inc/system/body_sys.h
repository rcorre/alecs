#ifndef BODY_SYS_H
#define BODY_SYS_H

/** \file body_sys.h
  * \brief system that handles movement of physical bodies.
  * operates on \ref Body components
**/

#include "ecs.h"

/** system function to update movement of physical bodies */
void body_system_fn(double time);

/** set up a body component to have constant velocity
  * \param b Body component to modify
  * \param vel constant linear velocity of body (px/sec)
  * \param angular_vel constant angular velocity of body (radians/sec) 
**/
void make_constant_vel_body(Body *b, vector vel);

#endif /* end of include guard: BODY_SYS_H */
