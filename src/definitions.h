#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* struct constants */
#define NUM_INTERSECTIONS 1
#define NUM_INSTRUCTIONS  2
#define NUM_VEHICLES      1

/* intersection constants */
#define MAX_CONNECTIONS   4
#define INTERSECTION_SIZE 50
#define DEFAULT_LENGTH    50
#define LANE_OFFSET       (INTERSECTION_SIZE / 2)

/* vehicle constants */
#define DEFAULT_SPEED 2
#define CAR_SIZE      (INTERSECTION_SIZE / 2)

/* location constants */
#define MAX_X 1200
#define MAX_Y 1200
#define INIT_INTERSECTION_START_X ((MAX_X / 2) - INTERSECTION_SIZE)
#define INIT_INTERSECTION_START_Y ((MAX_Y / 2) - INTERSECTION_SIZE)

#define SLEEP_INTERVAL 30000

/* enumerated types */
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
    int x;
    int y;
} vector_T;

/* vehicle_instructions */
typedef struct {
    /* 
    list: 
        List of directions the car will travel upon reaching each 
        intersection
     */
    direction_T* list; 
    char num;     /* Number of instructions */
    char current; /* Index of the current instruction */
    char next;    /* Index of the next instruction */
} instructions_T;

/* 
intersection 
    Data in `connections` and `lengths` stored in the same order as the
    `direction` enum, so `connections[0]` is `NORTH`, `connections[1]` is
    `EAST`, etc.
*/
typedef struct intersection {
    vector_T location;
    vector_T endpoints[MAX_CONNECTIONS];
    phase_T curr_phase;
    struct intersection* connections[MAX_CONNECTIONS]; 
    int lengths[MAX_CONNECTIONS];
    int turning_points[MAX_CONNECTIONS];
} intersection_T;

/* vehicle */
typedef struct {
    vector_T location;
    intersection_T* intersection;
    instructions_T* instructions;
    vehicle_priority_T priority;
    int speed;
    char turning;
} vehicle_T;