#include <stdio.h>
#include <stdlib.h>

/* temporary constants */
#define NUM_INTERSECTIONS 1
#define NUM_INSTRUCTIONS  2
#define NUM_VEHICLES      1

/* intersection constants */
#define MAX_CONNECTIONS   4
#define INTERSECTION_SIZE 50
#define DEFAULT_LENGTH    50

/* vehicle constants */
#define DEFAULT_SPEED 2

typedef enum {NORTH, EAST, SOUTH, WEST} direction_T;
typedef enum {PRIORITY_HIGH, PRIORITY_MEDIUM, PRIORITY_LOW} vehicle_priority_T;

/*
Types of phases
    VERTICAL_*   : Traffic flow in the north and south directions
    HORIZONTAL_* : Traffic flow in the east and west directions
    *_L          : Left turns
    *_SR         : Straight ahead and right turns
*/
typedef enum {
    VERTICAL_L, 
    VERTICAL_SR, 
    HORIZONTAL_L, 
    HORIZONTAL_SR
} phase_T;

/* vector */
typedef struct {
    short x;
    short y;
} vector_T;

/* 
intersection 
    Data in `connections` and `road_lengths` stored in the same order as the
    `direction` enum, so `connections[0]` is `NORTH`, `connections[1]` is
    `EAST`, etc.
*/
typedef struct intersection {
    vector_T location;
    phase_T curr_phase;
    struct intersection* connections[MAX_CONNECTIONS]; 
    short road_lengths[MAX_CONNECTIONS];
} intersection_T;

/* vehicle_instructions */
typedef struct {
    /* 
    list: 
        List of directions the car will travel upon reaching each 
        intersection
     */
    direction_T* list; 
    char num;     /* Number of instructions */
    char current; /* Current instruction */
} instructions_T;

/* vehicle */
typedef struct {
    vector_T location;
    instructions_T* instructions;
    vehicle_priority_T priority;
    short speed;
} vehicle_T;
