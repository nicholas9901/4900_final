#include "prototypes.h"
#include "draw.h"
#include "print.h"

int main(int argc, char** argv) {
    #if GUI
    realpath(argv[0], run_path);
    #endif

    intersection_T intersections[NUM_INSTRUCTIONS];
    vehicle_T      vehicles[NUM_VEHICLES];
    direction_T    list_instructions[NUM_INSTRUCTIONS];
    list_instructions[0] = EAST;
    list_instructions[1] = SOUTH;

    instructions_T vi1;
    init_instructions(&vi1, list_instructions, NUM_INSTRUCTIONS);
    
    short rl1[MAX_CONNECTIONS] = {DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH};
    init_intersection(
        &(intersections[0]), 
        &(intersections[0]), 
        &(intersections[0]), 
        &(intersections[0]), 
        &(intersections[0]), 
        rl1);

    init_vehicle(
        &(vehicles[0]),
        &(intersections[0]),
        &vi1,        
        WEST,
        PRIORITY_LOW,
        DEFAULT_SPEED);

    #if GUI
    draw_gui(vehicles, intersections);
    print_vehicle(&(vehicles[0]), 1);
    exit_gui();
    #endif
}