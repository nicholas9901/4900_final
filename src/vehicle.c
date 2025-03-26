#include "prototypes.h"

/*
Initialize Vehicle:
    The location of the vehicle is determined by the direction--for example,
    if the starting direction is north, the vehicle will start in the southmost
    part of the intersection.
*/
void init_vehicle(
    vehicle_T* vehicle,
    intersection_T* intersection,
    instructions_T* instructions, 
    vehicle_priority_T priority,
    int speed)
{
    direction_T corresponding_direction = determine_connection(
        instructions->list[instructions->current]);

    init_vector(
        &(vehicle->location), 
        intersection->endpoints[corresponding_direction].x,
        intersection->endpoints[corresponding_direction].y);
    
    vehicle->intersection = intersection;
    vehicle->instructions = instructions;
    vehicle->priority     = priority;
    vehicle->speed        = speed;
    vehicle->turning      = 1;
}

/* 
Move Vehicle:

*/
void move(vehicle_T* vehicle)
{
    int difference = 0;
    switch (vehicle->instructions->list[vehicle->instructions->current]) {
        case NORTH:
            vehicle->location.y -= vehicle->speed;
            break;
        case EAST:
            if (vehicle->turning) {
                switch (vehicle->instructions->list[vehicle->instructions->next]) {
                    case NORTH:
                        if (vehicle->location.x + vehicle->speed >= 
                            vehicle->intersection->turning_points[EAST] + 
                            LANE_OFFSET)
                        {
                            vehicle->location.x = 
                                vehicle->intersection->turning_points[EAST] + 
                                LANE_OFFSET;
                            
                            next_instruction(vehicle->instructions);  
                            vehicle->turning = 0;
                            return;  
                        }
                        break;
                    case SOUTH:
                        if (vehicle->location.x + vehicle->speed >= 
                            vehicle->intersection->turning_points[EAST])
                        {
                            vehicle->location.x = 
                                vehicle->intersection->turning_points[EAST];
                            next_instruction(vehicle->instructions);   
                            vehicle->turning = 0; 
                            return;
                        }
                        break;
                    default:
                        vehicle->turning = 0;
                        break;
                }
            }
            vehicle->location.x += vehicle->speed;
            break;
        case SOUTH:
            if ((difference = vehicle->location.y + vehicle->speed - 
                vehicle->intersection->endpoints[SOUTH].y) >= 0)
            {
                vehicle->location.y = transition_y(vehicle, SOUTH, difference);
            }
            else { vehicle->location.y += vehicle->speed; }
            break;
        case WEST:
            vehicle->location.x -= vehicle->speed;
            break;
        default:
            direction_error(vehicle->instructions->list[vehicle->instructions->current]);
    }
}

/*
Intersection Transition
    
*/
int transition_x(vehicle_T* vehicle, direction_T direction, int difference)
{
    int x = vehicle->intersection->connections[direction]->endpoints[determine_connection(direction)].x + difference;
    vehicle->intersection = vehicle->intersection->connections[direction];
    return x;
}

int transition_y(vehicle_T* vehicle, direction_T direction, int difference)
{
    int y = vehicle->intersection->connections[direction]->endpoints[determine_connection(direction)].y + difference;
    vehicle->intersection = vehicle->intersection->connections[direction];
    return y;
}