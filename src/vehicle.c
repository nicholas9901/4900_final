#include "prototypes.h"

void init_vehicle(
    vehicle_T* vehicle,
    intersection_T* intersection,
    instructions_T* instructions, 
    direction_T initial_direction,
    vehicle_priority_T priority,
    short speed)
{
    init_vector(
        &(vehicle->location), 
        initial_direction == WEST || initial_direction == EAST ? 
        intersection->road_lengths[initial_direction] : 0,
        initial_direction == NORTH || initial_direction == SOUTH ? 
        intersection->road_lengths[initial_direction] : 0);
        
    vehicle->instructions = instructions;
    vehicle->priority     = priority;
    vehicle->speed        = speed;
}
