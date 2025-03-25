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
void init_vector(vector_T*, short, short);

/* instructions.c */
void init_instructions(
    instructions_T*,
    direction_T*,
    int);

/* vehicle.c */
void init_vehicle(
    vehicle_T*,
    intersection_T*,
    instructions_T*, 
    direction_T,
    vehicle_priority_T,
    short);

/* intersection.c */
void init_intersection(
    intersection_T*,
    intersection_T*,
    intersection_T*,
    intersection_T*,
    intersection_T*,
    short[MAX_CONNECTIONS]);

#endif /* PROTOTYPES_H */
