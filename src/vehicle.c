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
    vehicle->turning      = true;
    vehicle->speed        = speed;
}

/* 
Move Vehicle:

*/
void move(vehicle_T* vehicle)
{
    int difference = 0;
    switch (vehicle->instructions->list[vehicle->instructions->current]) {
        case NORTH:
            if (vehicle->turning) {
                switch (vehicle->instructions->list[vehicle->instructions->next]) {
                    case EAST:
                        if (vehicle->location.y - vehicle->speed <= 
                            vehicle->intersection->turning_points[NORTH])
                        {
                            vehicle->location.y = 
                                vehicle->intersection->turning_points[NORTH];

                            next_instruction(vehicle->instructions);   
                            vehicle->turning = false; 
                            return;                            
                        }
                        break;
                    case WEST:
                        if (vehicle->location.y - vehicle->speed <= 
                            vehicle->intersection->turning_points[NORTH] - 
                            LANE_OFFSET) 
                        {
                            vehicle->location.y = 
                                vehicle->intersection->turning_points[NORTH] - LANE_OFFSET;

                            next_instruction(vehicle->instructions);   
                            vehicle->turning = false; 
                            return;
                        }
                        break;
                    default:
                        vehicle->turning = 0;
                        break;
                }
            }
            else if ((difference = vehicle->location.y - vehicle->speed - 
                 vehicle->intersection->endpoints[NORTH].y) <= 0) 
            {
                vehicle->location.y = 
                    vehicle->intersection->connections[NORTH]->endpoints[determine_connection(NORTH)].y 
                    + difference;
        
                vehicle->intersection = vehicle->intersection->connections[NORTH];
                vehicle->turning = true;
            }
            /* North movement */
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
                                vehicle->intersection->turning_points[EAST] + LANE_OFFSET;
                
                            next_instruction(vehicle->instructions);   
                            vehicle->turning = false;
                            return;  
                        }
                        break;
                    case SOUTH:
                        if (vehicle->location.x + vehicle->speed >= 
                            vehicle->intersection->turning_points[EAST]) 
                        {
                            vehicle->location.x = 
                                vehicle->intersection->turning_points[EAST];   

                            vehicle->intersection = vehicle->intersection->connections[EAST];
                            next_instruction(vehicle->instructions);  
                            vehicle->turning = false;
                            return;
                        }
                        break;
                    default:
                        vehicle->turning = 0;
                        break;
                }
            }
            else if((difference = vehicle->location.x + vehicle->speed - 
                vehicle->intersection->endpoints[EAST].x) >= 0) 
            {
                vehicle->location.x = 
                    vehicle->intersection->connections[EAST]->endpoints[determine_connection(EAST)].x 
                    + difference;
        
                vehicle->intersection = vehicle->intersection->connections[EAST];
                vehicle->turning = true;
            }
            /* East movement */
            vehicle->location.x += vehicle->speed;
            break;
        case SOUTH:
            if (vehicle->turning) {
                switch (vehicle->instructions->list[vehicle->instructions->next]) {
                    case EAST:
                        if (vehicle->location.y + vehicle->speed >= 
                            vehicle->intersection->turning_points[SOUTH] + 
                            LANE_OFFSET)
                        {
                            vehicle->location.y = 
                                vehicle->intersection->turning_points[SOUTH] + LANE_OFFSET;
                
                            next_instruction(vehicle->instructions);   
                            vehicle->turning = false; 
                            return;
                        }
                        break;
                    case WEST:
                        if (vehicle->location.y + vehicle->speed >= 
                            vehicle->intersection->turning_points[SOUTH])
                        {
                            vehicle->location.y = 
                                vehicle->intersection->turning_points[SOUTH];
                
                            next_instruction(vehicle->instructions);   
                            vehicle->turning = false;
                            return;
                        }
                        break;
                    default:
                        vehicle->turning = 0;
                        break;
                }
            }
            else if ((difference = vehicle->location.y + vehicle->speed - 
                      vehicle->intersection->endpoints[SOUTH].y) >= 0) 
            {
                vehicle->location.y = 
                    vehicle->intersection->connections[SOUTH]->endpoints[determine_connection(SOUTH)].y 
                    + difference;
        
                vehicle->intersection = vehicle->intersection->connections[SOUTH];
                vehicle->turning = true;
            }
            /* South movement */
            vehicle->location.y += vehicle->speed;
            break;
        case WEST:
            if (vehicle->turning) {
                switch (vehicle->instructions->list[vehicle->instructions->next]) {
                    case NORTH:
                        if (vehicle->location.x - vehicle->speed <= 
                            vehicle->intersection->turning_points[WEST])
                        {
                            vehicle->location.x = 
                                vehicle->intersection->turning_points[WEST];
                
                            next_instruction(vehicle->instructions);   
                            vehicle->turning = false;
                            return; 
                        }
                        break;
                    case SOUTH:
                        if (vehicle->location.x - vehicle->speed <= 
                            vehicle->intersection->turning_points[WEST] - 
                            LANE_OFFSET)
                        {
                            vehicle->location.x = 
                                vehicle->intersection->turning_points[WEST] - LANE_OFFSET;

                            next_instruction(vehicle->instructions);   
                            vehicle->turning = false; 
                            return;
                        }
                        break;
                    default:
                        vehicle->turning = 0;
                        break;
                }
            }
            else if ((difference = vehicle->location.x - vehicle->speed - 
                      vehicle->intersection->endpoints[WEST].x) <= 0) 
            {
                vehicle->location.x = 
                    vehicle->intersection->connections[WEST]->endpoints[determine_connection(WEST)].x 
                    + difference;
        
                vehicle->intersection = vehicle->intersection->connections[WEST];
                vehicle->turning = true;
            }
            /* West movement */
            vehicle->location.x -= vehicle->speed;
            break;
        default:
            direction_error(vehicle->instructions->list[vehicle->instructions->current]);
    }
}
