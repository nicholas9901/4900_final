#include "prototypes.h"

void init_vehicle(
    vehicle_T* vehicle,
    intersection_T* intersection,
    instructions_T* instructions, 
    direction_T initial_direction,
    vehicle_priority_T priority,
    float max_speed,
    float acceleration)
{
    init_vector(
        &(vehicle->location), 
        initial_direction == WEST || initial_direction == EAST ? 
        intersection->road_lengths[initial_direction] : 0,
        initial_direction == NORTH || initial_direction == SOUTH ? 
        intersection->road_lengths[initial_direction] : 0);
        
    vehicle->instructions = instructions;
    vehicle->priority     = priority;
    vehicle->max_speed    = max_speed;
    vehicle->acceleration = acceleration;
}

#if GUI
void print_vehicle(vehicle_T* vehicle, int id)
{
    printf("Vehicle %d:\n       (x,y): ", id);
    print_vector(&(vehicle->location));
    printf(
        "\n       speed: %f\n"
        "       accel: %f\n"
        "   max_speed: %f\n",
        0.0f, // TODO: add a function to get speed
        vehicle->acceleration,
        vehicle->max_speed);
    // TODO: need to add print instructions function
}
#endif