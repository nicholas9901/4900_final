#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "definitions.h"

/* vector.c */
void init_vector(vector_T*, float, float);

#if GUI
void print_vector(vector_T*);
#endif

/* instructions.c */

void init_instructions
(
    instructions_T*,
    direction_T*,
    int
);

/* vehicle.c */
void init_vehicle 
(
    vehicle_T*,
    intersection_T*,
    instructions_T*, 
    direction_T,
    vehicle_priority_T,
    float,
    float
);

#if GUI
void print_vehicle(vehicle_T*, int);
#endif

/* intersection.c */
void init_intersection
(
    intersection_T*,
    intersection_T*,
    intersection_T*,
    intersection_T*,
    intersection_T*,
    float[MAX_CONNECTIONS]
);

#if GUI
void print_intersection(intersection_T*);
#endif

/* draw.c */
#if GUI
void draw_gui();
void update_gui(vehicle_T*, intersection_T*);
void exit_gui();
void draw_vehicle(vehicle_T*);
void draw_intersection(intersection_T*);

#endif

#endif /* PROTOTYPES_H */
