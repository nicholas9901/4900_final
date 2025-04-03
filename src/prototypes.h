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
bool emergency_arrived(vector_T*, vehicle_T*);

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

/* intersection.c */
void init_intersection(intersection_T*, int[MAX_CONNECTIONS], char);
void connect_intersection(intersection_T*, intersection_T*, direction_T);    
void calculate_intersection_lengths(intersection_T*);
void transfer_vehicles(vehicle_list_T*, vehicle_list_T*);
void dequeue_vehicles(vehicle_list_T*, intersection_T*);
int total_in_queue(intersection_T*);
int total_horizontal(intersection_T*);
int total_vertical(intersection_T*);
direction_T max_queue(intersection_T*);
bool tlc_baseline(vehicle_list_T*, intersection_T*);
bool tlc_emergency(vehicle_list_T*, vehicle_T*, intersection_T*);

#if GUI
bool phase_timer(vehicle_list_T*, intersection_T*);
#endif

#endif /* PROTOTYPES_H */
