#include "prototypes.h"
#include "draw.h"
#include "print.h"

int main(int argc, char** argv) {
    
    /* 
    Get the path of the program to load the assets if the GUI build is running 
    */
    #if GUI
    realpath(argv[0], run_path);
    #endif

    /* Initializing the structures */
    intersection_T intersections[NUM_INTERSECTIONS];
    vehicle_T      vehicles[NUM_VEHICLES];
    direction_T    list_instructions[2];
    list_instructions[0] = WEST;
    list_instructions[1] = SOUTH;

    instructions_T vi1;
    init_instructions(&vi1, list_instructions, 2);
    
    int rl1[MAX_CONNECTIONS] = {DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH};

    init_intersection(
        &(intersections[0]), 
        &(intersections[0]), 
        &(intersections[1]), 
        &(intersections[0]), 
        &(intersections[1]), 
        rl1);

    init_intersection(
        &(intersections[1]), 
        &(intersections[1]), 
        &(intersections[0]), 
        &(intersections[1]), 
        &(intersections[0]), 
        rl1);

    /* Intersection coordinate generation */
    init_vector(
        &(intersections[0].location),
        INIT_INTERSECTION_START_X, 
        INIT_INTERSECTION_START_Y);

    init_intersection_construction(&(intersections[0]));

    init_vehicle(
        &(vehicles[0]),
        &(intersections[0]),
        &vi1,        
        PRIORITY_LOW,
        DEFAULT_SPEED);

    #if GUI
    print_vehicle(&(vehicles[0]), 1);
    print_intersection(&(intersections[0]), 1);
    print_intersection(&(intersections[1]), 2);
    init_gui(vehicles, intersections);
    #endif
    /* Main program loop */
    while(1) {
        for (int i = 0; i < NUM_VEHICLES; i++) {
            move(&(vehicles[i]));
        }

        // for (int i; i < NUM_INTERSECTIONS; i++) {

        // }
        #if GUI
        poll_for_exit(); /* Listen for an exit event before updating */
        update_gui(vehicles, intersections);
        #endif
        usleep(SLEEP_INTERVAL);
    }

    #if GUI
    while(1) {
        poll_for_exit();
    }
    #endif

    printf("exit\n");
}