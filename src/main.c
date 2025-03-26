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
    direction_T    list_instructions[4];
    list_instructions[0] = NORTH;
    list_instructions[1] = EAST;
    list_instructions[2] = SOUTH;
    list_instructions[3] = WEST;

    instructions_T vi1;
    init_instructions(&vi1, list_instructions, 4);
    
    int rl1[MAX_CONNECTIONS] = {DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH};
    init_intersection(
        &(intersections[0]), 
        &(intersections[0]), 
        &(intersections[0]), 
        &(intersections[0]), 
        &(intersections[0]), 
        rl1);

    init_intersection_locations(&(intersections[0]));

    init_vehicle(
        &(vehicles[0]),
        &(intersections[0]),
        &vi1,        
        PRIORITY_LOW,
        DEFAULT_SPEED);

    #if GUI
    print_vehicle(&(vehicles[0]), 1);
    print_intersection(&(intersections[0]), 1);
    init_gui(vehicles, intersections);
    #endif
    /* Main program loop */
    while(1) {
        for (int idx_vehicle = 0; idx_vehicle < NUM_VEHICLES; idx_vehicle++) {
            move(&vehicles[idx_vehicle]);
        }

        // for (int idx_intersection; 
        //     idx_intersection < NUM_INTERSECTIONS; 
        //     idx_intersection++) 
        // {

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