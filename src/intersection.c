#include "prototypes.h"

/*
Initialize Intersection
    Note that the location isn't set, this will be done when constructing
    the network judging by the order of which they are connected.
*/
void init_intersection(
    intersection_T* intersection,
    intersection_T* connection_north,
    intersection_T* connection_east,
    intersection_T* connection_south,
    intersection_T* connection_west,
    int lengths[MAX_CONNECTIONS])
{
    intersection->curr_phase         = HORIZONTAL_SR;
    intersection->connections[NORTH] = connection_north;
    intersection->connections[EAST]  = connection_east;
    intersection->connections[SOUTH] = connection_south;
    intersection->connections[WEST]  = connection_west;
    
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        intersection->lengths[i] = lengths[i];
    }    
}

/*
Initialize Intersection Locations
*/
void init_intersection_locations(intersection_T* intersection)
{
    /* 
    The first intersection should be in the middle of the simulation canvas,
    and all following intersections will then recursively follow from the 
    previous by iterating through its connections.
    */
    init_vector(
        &(intersection[0].location),
        INIT_INTERSECTION_START_X, 
        INIT_INTERSECTION_START_Y);

    // TODO: refactor all this into a recursive function
    /* Endpoints */
    intersection[0].endpoints[NORTH].x = intersection[0].location.x;
    intersection[0].endpoints[NORTH].y = intersection[0].location.y - 
        intersection[0].lengths[NORTH];

    intersection[0].endpoints[EAST].x = intersection[0].location.x + 
        intersection[0].lengths[EAST] + INTERSECTION_SIZE;
    intersection[0].endpoints[EAST].y = intersection[0].location.y;

    intersection[0].endpoints[SOUTH].x = intersection[0].location.x + 
        LANE_OFFSET;
    intersection[0].endpoints[SOUTH].y = intersection[0].location.y + 
        intersection[0].lengths[SOUTH] + INTERSECTION_SIZE;
    

    intersection[0].endpoints[WEST].x = intersection[0].location.x -
        intersection[0].lengths[WEST];
    intersection[0].endpoints[WEST].y = intersection[0].location.y + 
        LANE_OFFSET;

    /* Turning points */
    intersection[0].turning_points[NORTH] = intersection[0].location.y + 
        LANE_OFFSET;

    intersection[0].turning_points[EAST] = intersection[0].location.x;

    intersection[0].turning_points[SOUTH] = intersection[0].location.y;

    intersection[0].turning_points[WEST] = intersection[0].location.x +
        LANE_OFFSET;
}
