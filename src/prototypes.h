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
    int,
    char);

void init_active_vehicles(vehicle_list_T*, vehicle_T*, vehicle_T*);
bool move_vehicle(vehicle_T*);
void enqueue_vehicle(vehicle_list_T*, vehicle_T*, int);
phase_T get_phase(direction_T, direction_T);
bool emergency_arrived(vehicle_T*);


/* intersection.c */
void init_intersection(
    intersection_T*,
    intersection_T*,
    intersection_T*,
    intersection_T*,
    intersection_T*,
    int[MAX_CONNECTIONS],
    char);

void init_intersection_construction(intersection_T*);
void transfer_vehicles(vehicle_list_T*, vehicle_list_T*);
void dequeue_vehicles(vehicle_list_T*, intersection_T*);


#if GUI
bool phase_timer(vehicle_list_T*, intersection_T*);
#endif

#endif /* PROTOTYPES_H */
