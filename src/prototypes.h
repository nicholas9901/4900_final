/*
prototypes.h:
    Includes all function definitions for all C files aside from print.c
    and draw.c--those two files have their own definitions since they aren't
    a part of the NOGUI build.
*/

#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "definitions.h"

/* vector.c */
void init_vector(vector_T*, int, int);

/* direction.c */
void direction_error(direction_T);
direction_T determine_connection(direction_T);

/* instructions.c */
void init_instructions(
    instructions_T*,
    direction_T*,
    int);
void next_instruction(instructions_T*);


/* vehicle.c */
void init_vehicle(
    vehicle_T*,
    intersection_T*,
    instructions_T*, 
    vehicle_priority_T,
    int);

void move(vehicle_T*);
bool determine_turn_x(vehicle_T*, direction_T, bool);
bool determine_turn_y(vehicle_T*, direction_T, bool);
bool determine_transition_x(vehicle_T*, direction_T);
bool determine_transition_y(vehicle_T*, direction_T);

/* intersection.c */
void init_intersection(
    intersection_T*,
    intersection_T*,
    intersection_T*,
    intersection_T*,
    intersection_T*,
    int[MAX_CONNECTIONS]);

void init_intersection_locations(intersection_T*);

#endif /* PROTOTYPES_H */
