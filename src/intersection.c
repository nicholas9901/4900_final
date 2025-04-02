#include "prototypes.h"

#if GUI
#include "print.h"
#include "draw.h"
#endif

/*
Initialize Intersection
    Note that the location isn't set, this will be done when constructing
    the network judging by the order of which they are connected. Neither does
    this apply the connections--by default, all connections connect to itself.
*/
void init_intersection(
    intersection_T* intersection,
    int lengths[MAX_CONNECTIONS],
    char id)
{
    intersection->phase = HORIZONTAL_SR;    
    intersection->id    = id;
    intersection->timer = INIT_PHASE_TIMER;
    
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        intersection->connections[i]         = intersection;
        intersection->lengths[i]             = lengths[i];
        intersection->queued_vehicles[i].num = 0;
    }    
}

/*
Connect Intersection
    Given an intersection from, connects from to new, its connection varying
    by the given direction.
*/
void connect_intersection(
    intersection_T* from, 
    intersection_T* new,
    direction_T direction)
{
    /* 
    Do a traversal to determine loops--this is analogous to a ciruclar doubly 
    linked list 
    */
    direction_T opposite = determine_connection(direction);
    intersection_T* curr = from->connections[opposite];
    intersection_T* prev = curr;

    while (curr != from) {
        prev = curr;
        curr = curr->connections[opposite];
    }

    prev->connections[opposite]  = new; 
    new->connections[direction]  = prev;
    from->connections[direction] = new;
    new->connections[opposite]   = from;

    /* Apply the correct transformations to the new intersection */
    if (direction % 2 == 1 /* Horizontal directions */ ) {
        new->location.x = 
            from->location.x + (INTERSECTION_SIZE + from->lengths[direction] + 
            from->connections[direction]->lengths[opposite]) *
            direction_matrix[direction][0];

        new->location.y = from->location.y;
    }
    else /* Vertical directions */ {
        new->location.x = from->location.x;

        new->location.y = 
            from->location.y + (INTERSECTION_SIZE + from->lengths[direction] + 
            from->connections[direction]->lengths[opposite]) * 
            direction_matrix[direction][1];            
    }
    calculate_intersection_lengths(new);
}

/*
Calculate Intersection Lengths
    Calculates all the integer values associated with each road of an 
    intersection
*/
void calculate_intersection_lengths(intersection_T* intersection)
{
    /* Endpoints */
    intersection->end_points[NORTH] = intersection->location.y - 
    intersection->lengths[NORTH];

    intersection->end_points[EAST] = intersection->location.x + 
        intersection->lengths[EAST] + INTERSECTION_SIZE;

    intersection->end_points[SOUTH] = intersection->location.y + 
        intersection->lengths[SOUTH] + INTERSECTION_SIZE;

    intersection->end_points[WEST] = intersection->location.x -
        intersection->lengths[WEST];

    /* Turning points */
    intersection->turning_points_right[NORTH] = intersection->location.y + 
        LANE_OFFSET;
    intersection->turning_points_left[NORTH] = intersection->location.y;

    intersection->turning_points_right[EAST] = intersection->location.x;
    intersection->turning_points_left[EAST] = intersection->location.x + 
        LANE_OFFSET;

    intersection->turning_points_right[SOUTH] = intersection->location.y;
    intersection->turning_points_left[SOUTH] = intersection->location.y + 
        LANE_OFFSET;
        
    intersection->turning_points_right[WEST] = intersection->location.x +
        LANE_OFFSET;
    intersection->turning_points_left[WEST] = intersection->location.x;


    /* Stopping points */
    intersection->stopping_points[NORTH] = 
        intersection->location.y + INTERSECTION_SIZE + DIV_WIDTH;

    intersection->stopping_points[EAST] = 
        intersection->location.x - DIV_WIDTH - VEHICLE_SIZE;

    intersection->stopping_points[SOUTH] = 
        intersection->location.y - DIV_WIDTH - VEHICLE_SIZE;

    intersection->stopping_points[WEST] = 
        intersection->location.x + INTERSECTION_SIZE + DIV_WIDTH;
}

/*
Transfer Vehicles
    Helper function for dequeue_vehicles
*/
void transfer_vehicles(vehicle_list_T* list_to, vehicle_list_T* list_from)
{
    while (list_from->num > 0) {
        list_to->vehicles[(list_to->num)++] = list_from->vehicles[--(list_from->num)];
    }
}

/*
Dequeue Vehicles
    Given an intersection, and its phase takes all the vehicles in the 
    corresponding direction queues and puts them back into the list of active 
    vehicles. Also resets the intersection stopping point for directions 
    affected.
*/
void dequeue_vehicles(
    vehicle_list_T* active_vehicles, 
    intersection_T* intersection)
{
        transfer_vehicles(
            active_vehicles,
            &(intersection->queued_vehicles[NORTH]));

        transfer_vehicles(
            active_vehicles,
            &(intersection->queued_vehicles[SOUTH]));

        transfer_vehicles(
            active_vehicles,
            &(intersection->queued_vehicles[EAST]));

        transfer_vehicles(
            active_vehicles,
            &(intersection->queued_vehicles[WEST]));
}

/*
Total In Queue
    Helper function for the TLC algorithms.
    Return the total number of vehicles in the directional queues of an 
    intersection.
*/
int total_in_queue(intersection_T* intersection) {
    int sum = 0;
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        sum += intersection->queued_vehicles[i].num;
    }
    return sum;
}

/*
Total Horizontal
    Helper function for the TLC algorithms.
    Returns the number of vehicles in the east and west queues.
*/
int total_horizontal(intersection_T* intersection)
{
    return intersection->queued_vehicles[EAST].num + 
        intersection->queued_vehicles[WEST].num;
}

/*
Total Vertical
    Helper function for the TLC algorithms.
    Returns the number of vehicles in the north and south queues.
*/
int total_vertical(intersection_T* intersection)
{
    return intersection->queued_vehicles[NORTH].num + 
        intersection->queued_vehicles[SOUTH].num;
}

/*
Max Flow
    Helper function for the TLC algorithms.
    Returns the direction corresponding to the maximum queue between two 
    opposing traffic flows given the current phase.
*/
int max_flow(intersection_T * intersection)
{
    /* Consider the vertical phases */
    if (intersection->phase < HORIZONTAL_L) {
        /* 
        If the NORTH queue is greater than SOUTH, return NORTH. 
        Else, return SOUTH. 
        */
        return intersection->queued_vehicles[NORTH].num > 
            intersection->queued_vehicles[SOUTH].num ? 
            intersection->queued_vehicles[NORTH].num :
            intersection->queued_vehicles[SOUTH].num;
    }
    /* Consider the horizontal phases */
    else {
        /* 
        If the EAST queue is greater than WEST, return EAST. 
        Else, return WEST. 
        */
       return intersection->queued_vehicles[EAST].num > 
            intersection->queued_vehicles[WEST].num ? 
            intersection->queued_vehicles[EAST].num :
            intersection->queued_vehicles[WEST].num;
    }
}

/*
Traffic Light Control Algorithm: 
An Intelligent Traffic Light scheduling algorithm through VANETs, by Younes &
Boukereche, from 
    The baseline algorithm with no priority for emergency vehicles. The time of
    a phase is proportional to the queue density in the corresponding 
    directions.
*/
bool tlc_baseline(vehicle_list_T* active_vehicles, intersection_T* intersection)
{
    /* Do nothing if there is no traffic */
    if (total_in_queue(intersection) <= 0 ) { return false; }

    if (intersection->timer <= 0) {
        int cycles = NUM_PHASES;
        while (intersection->timer <= 0) {
            intersection->phase = (intersection->phase + 1) % NUM_PHASES;
            intersection->timer = max_flow(intersection) * TIME_TO_CROSS;
            cycles--;
            if (cycles <= 0) { return false; }
        }
        dequeue_vehicles(active_vehicles, intersection);
        draw_phase_change(intersection);    
        return true;
    }
    intersection->timer--;
    return false;
}

/*
Traffic Light Control Algorithm: 
Based off two papers: 
  - Traffic Management for Emergency Vehicle Priority Based on Visual Sensing by
    Nellore and Hancke
  - A Smart Priority-Based Traffic Control System for Emergency Vehicles by 
    Karmakar et al.

    The emergency algorithm which factors in queue densities as well as 
    vehicle priorities.

*/
bool tlc_emergency(
    vehicle_list_T* active_vehicles, 
    vehicle_T* emergency_vehicle, 
    intersection_T* intersection)
{
    return true;
}

/*
Phase Timer
    Sample phase changing algorithm for testing
*/
#if GUI
bool phase_timer(vehicle_list_T* active_vehicles, intersection_T* intersection)
{
    if (intersection->timer < 0 ) {
        intersection->phase = (intersection->phase + 1) % NUM_PHASES;
        intersection->timer = TIME_TO_CROSS;
        print_intersection(intersection);
        dequeue_vehicles(active_vehicles, intersection);
        draw_phase_change(intersection);
        return true;
    }
    else { 
        intersection->timer--; 
        return false;
    }
}
#endif
