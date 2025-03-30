#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/* Struct constants */
#define NUM_INTERSECTIONS    2
#define NUM_VEHICLES_TRAFFIC 1
#define NUM_VEHICLES_TOTAL   (NUM_VEHICLES_TRAFFIC + 1)
#define NUM_DIRECTIONS       4

/* Intersection constants */
#define MAX_CONNECTIONS   4
#define INTERSECTION_SIZE 50
#define DEFAULT_LENGTH    60
#define LANE_OFFSET       (INTERSECTION_SIZE / 2)
#define DIV_WIDTH         4 /* Lane division and stopping point widths */
#define DIV_OFFSET        ((INTERSECTION_SIZE / 2) - DIV_WIDTH + 1)
#define PHASE_TIMER       50
#define NUM_PHASES        4

/* Vehicle constants */
#define DEFAULT_SPEED   2
#define EMERGENCY_SPEED 3
#define VEHICLE_SIZE        (INTERSECTION_SIZE / 2)

/* Location constants */
#define MAX_X 1200
#define MAX_Y 1200
#define INIT_INTERSECTION_START_X ((MAX_X / 2) - INTERSECTION_SIZE)
#define INIT_INTERSECTION_START_Y ((MAX_Y / 2) - INTERSECTION_SIZE)

#define SLEEP_INTERVAL 30000

/* Enumerated types */
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

/* Struct forward declarations */
typedef struct vehicle vehicle_T;
typedef struct vehicle_list vehicle_list_T;
typedef struct intersection intersection_T;

/* Structs */

/* Vector */
typedef struct {
    int x;
    int y;
} vector_T;

/* Vehicle Instructions */
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

/* Vehicle */
typedef struct vehicle {
    vector_T location;
    intersection_T* intersection;
    instructions_T instructions;
    vehicle_priority_T priority;
    bool turning;
    bool stopping;
    int speed;
    char id;
} vehicle_T;

/* Vehicle List */
typedef struct vehicle_list {
    vehicle_T* vehicles[NUM_VEHICLES_TOTAL];
    int num;
} vehicle_list_T;

/* 
Intersection 
    Data in `connections` and `lengths` stored in the same order as the
    `direction` enum, so `connections[0]` is `NORTH`, `connections[1]` is
    `EAST`, etc.
*/
typedef struct intersection {
    vector_T location;
    vector_T endpoints[MAX_CONNECTIONS];
    phase_T phase;
    struct intersection* connections[MAX_CONNECTIONS]; 
    vehicle_list_T queued_vehicles[MAX_CONNECTIONS];
    int lengths[MAX_CONNECTIONS];
    int turning_points[MAX_CONNECTIONS];
    int stopping_points[MAX_CONNECTIONS];
    bool constructed;
    int timer;
    char id;
} intersection_T;