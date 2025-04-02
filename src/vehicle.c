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
    int speed,
    char id)
{
    direction_T corresponding_direction = determine_connection(
        instructions->list[instructions->current]);

    switch (corresponding_direction) {
        case NORTH:
            init_vector(
                &(vehicle->location), 
                intersection->location.x,
                intersection->end_points[corresponding_direction]);    
            break;
        case EAST:
            init_vector(
                &(vehicle->location), 
                intersection->end_points[corresponding_direction],    
                intersection->location.y);
            break;
        case SOUTH:
            init_vector(
                &(vehicle->location), 
                intersection->location.x + LANE_OFFSET,
                intersection->end_points[corresponding_direction]);
            break;
        case WEST:
            init_vector(
                &(vehicle->location), 
                intersection->end_points[corresponding_direction],    
                intersection->location.y + LANE_OFFSET);
            break;            
        default:
            direction_error(corresponding_direction);
    }
    
    vehicle->intersection = intersection;
    vehicle->instructions = *instructions;
    vehicle->priority     = priority;
    vehicle->turning      = true;
    vehicle->stopping     = true;
    vehicle->speed        = speed;
    vehicle->id           = id;
}

/*
Initialize Active Vehicles
    Puts all vehicles, traffic and emergency included into a list which
    will be used to determine which vehicles moving or stopped at a traffic
    light.
*/
void init_active_vehicles(
    vehicle_list_T* active_vehicles, 
    vehicle_T* emergency_vehicle, 
    vehicle_T* traffic_vehicles)
{
    active_vehicles->num         = NUM_VEHICLES_TOTAL;
    active_vehicles->vehicles[0] = emergency_vehicle;
    for (int i = 1; i < active_vehicles->num; i++) {
        active_vehicles->vehicles[i] = &(traffic_vehicles[i - 1]);
    }
}

/*
Remove Active Vehicle
    Remove the vehicle in the active vehicles list and put it into the 
    intersection queue. 
    If a vehicle is of high priority, update the intersection's priority
    service variable.
*/
void enqueue_vehicle(
    vehicle_list_T* active_vehicles, 
    vehicle_T* current_vehicle,
    int index)
{
    direction_T current_direction = current_vehicle->instructions.list[current_vehicle->instructions.current]; 
    vehicle_list_T* current_list = &(current_vehicle->intersection->queued_vehicles[current_direction]);
    active_vehicles->num--;
    for (int i = index; i < active_vehicles->num; i++) {
        active_vehicles->vehicles[i] = active_vehicles->vehicles[i + 1];
    }
    current_list->vehicles[(current_list->num)++] = current_vehicle;
    if (current_vehicle->priority >= PRIORITY_HIGH) {
        current_vehicle->intersection->priority_service = true;
    }
}

/*
Get Phase
    Helper function for move vehicle, gets the corresponding phase given a
    current and next direction.
*/
phase_T get_phase(direction_T current, direction_T next)
{
    switch (current) {
        case NORTH:
            if (next == WEST) return VERTICAL_L;
            return VERTICAL_SR;
        case EAST:
            if (next == NORTH) return HORIZONTAL_L;
            return HORIZONTAL_SR; 
        case SOUTH:
            if (next == EAST) return VERTICAL_L;
            return VERTICAL_SR;
        case WEST:
            if (next == SOUTH) return HORIZONTAL_L;
            return HORIZONTAL_SR;
    }
    exit(1);
}

/* 
Move Vehicle:

*/
bool move_vehicle(vehicle_T* vehicle)
{
    int direction      = vehicle->instructions.list[vehicle->instructions.current];
    int next_direction = vehicle->instructions.list[vehicle->instructions.next];
    int movement_x     = direction_matrix[direction][0] * vehicle->speed;
    int movement_y     = direction_matrix[direction][1] * vehicle->speed;
    char orientation   = direction % 2 == 1; /* True for horizontal, false for vertical */

    if (vehicle->turning) {
        int stopping_point = vehicle->intersection->stopping_points[direction];
        int difference     = orientation ? 
            (vehicle->location.x - stopping_point) * direction_matrix[direction][0] :
            (vehicle->location.y - stopping_point) * direction_matrix[direction][1];

        phase_T correct_phase = get_phase(direction, next_direction);

        if (vehicle->stopping) { /* Determine if the vehicle should stop */
            if (difference > 0 ) {
                vehicle->stopping = false;
            }
            else if (vehicle->intersection->phase   != correct_phase
                     && difference + vehicle->speed >= 0) 
            {
                if (orientation) {
                    vehicle->location.x = vehicle->intersection->stopping_points[direction];
                }
                else {
                    vehicle->location.y = vehicle->intersection->stopping_points[direction];
                }
                return false;
            }
        }
        else { /* Determine if the vehicle should turn and change directions */ 
            int turning_point = correct_phase % 2 == 0 ?
                vehicle->intersection->turning_points_left[direction] : /* Left turns */
                vehicle->intersection->turning_points_right[direction]; /* Right turns */
            int difference = orientation ? 
                (vehicle->location.x - turning_point) * direction_matrix[direction][0] :
                (vehicle->location.y - turning_point) * direction_matrix[direction][1];
            if (difference >= 0) {
                if (direction % 2 == 1 /* Horizontal */) {
                    vehicle->location.x = turning_point;
                }
                else /* Vertical */ {
                    vehicle->location.y = turning_point;
                }
                next_instruction(&(vehicle->instructions));   
                movement_x = direction_matrix[next_direction][0] * vehicle->speed;
                movement_y = direction_matrix[next_direction][1] * vehicle->speed;
                vehicle->turning = false; 
            }
        } 
    }
    /* Determine if the vehicle should transition to the next intersection */
    int end_point  = vehicle->intersection->end_points[direction];
    int difference = orientation ? 
        (vehicle->location.x - end_point) * direction_matrix[direction][0] :
        (vehicle->location.y - end_point) * direction_matrix[direction][1];

    if (difference >= 0) {
        direction_T opposite = determine_connection(direction);
        if (direction % 2 == 1 /* Horizontal directions */) {
            vehicle->location.x =
                vehicle->intersection->connections[direction]->end_points[opposite];                    
        }
        else /* Vertical directions */ {
            vehicle->location.y = 
                vehicle->intersection->connections[direction]->end_points[opposite];
        }
        vehicle->intersection = vehicle->intersection->connections[direction];
        vehicle->turning  = true;
        vehicle->stopping = true;
    }

    vehicle->location.x += movement_x;
    vehicle->location.y += movement_y;
    return true;
}

bool emergency_arrived(vehicle_T* vehicle)
{
    return true;
}