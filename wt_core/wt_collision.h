#ifndef _WT_COLLISION_H_
#define _WT_COLLISION_H_
#include "wt_core.h"

void wt_collision_detect_table_version(wt_world *w);
void wt_collision_detect(wt_world *w);
void wt_collision_contact_clear(wt_world *w);
void wt_contact_before_solve(wt_contact *contact, wt_r32 dt);
void wt_contact_solve(wt_contact *contact, wt_r32 dt);
void wt_collision_before_solve(wt_world *w, wt_r32 dt);
void wt_collision_solve(wt_world *w, wt_r32 dt);

#endif