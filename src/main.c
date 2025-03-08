#include "prototypes.h"

int main() {

    intersection_T* intersections = malloc(sizeof(intersection_T) * NUM_INSTRUCTIONS);
    vehicle_T* vehicles = malloc(sizeof(vehicle_T) * NUM_VEHICLES);

    direction_T* list_instructions = malloc(sizeof(direction_T) * NUM_INSTRUCTIONS);
    list_instructions[0] = EAST;
    list_instructions[1] = SOUTH;

    instructions_T vi1;
    init_instructions(&vi1, list_instructions, NUM_INSTRUCTIONS);
    
    float rl1[MAX_CONNECTIONS] = {DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH};
    init_intersection(&(intersections[0]), &(intersections[0]), &(intersections[0]), &(intersections[0]), &(intersections[0]), rl1);

    init_vehicle(
        &(vehicles[0]),
        &(intersections[0]),
        &vi1,        
        WEST,
        PRIORITY_LOW,
        DEFAULT_MAX_SPEED,
        DEFAULT_ACCELERATION);

    #if GUI
    draw_gui(vehicles, intersections);
    print_vehicle(&(vehicles[0]), 1);
    exit_gui();
    #endif

    free(list_instructions);
    free(vehicles);
    free(intersections);
}