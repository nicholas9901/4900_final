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
    
    intersection->constructed = false;
    
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        intersection->lengths[i] = lengths[i];
    }    
}

/*
Initialize Intersection Locations
*/
void init_intersection_construction(intersection_T* intersection)
{
    /* 
    The first intersection should be in the middle of the simulation canvas,
    and all following intersections will then recursively follow from the 
    previous by iterating through its connections.
    */

    /* Endpoints */
    intersection->endpoints[NORTH].x = intersection->location.x;
    intersection->endpoints[NORTH].y = intersection->location.y - 
        intersection->lengths[NORTH];

    intersection->endpoints[EAST].x = intersection->location.x + 
        intersection->lengths[EAST] + INTERSECTION_SIZE;
    intersection->endpoints[EAST].y = intersection->location.y;

    intersection->endpoints[SOUTH].x = intersection->location.x + 
        LANE_OFFSET;
    intersection->endpoints[SOUTH].y = intersection->location.y + 
        intersection->lengths[SOUTH] + INTERSECTION_SIZE;
    

    intersection->endpoints[WEST].x = intersection->location.x -
        intersection->lengths[WEST];
    intersection->endpoints[WEST].y = intersection->location.y + 
        LANE_OFFSET;

    /* Turning points */
    intersection->turning_points[NORTH] = intersection->location.y + 
        LANE_OFFSET;

    intersection->turning_points[EAST] = intersection->location.x;

    intersection->turning_points[SOUTH] = intersection->location.y;

    intersection->turning_points[WEST] = intersection->location.x +
        LANE_OFFSET;
    
    intersection->constructed = true;

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (!intersection->connections[i]->constructed) {
            direction_T opposite = determine_connection(i);

            if (i == NORTH || i == SOUTH) {
                intersection->connections[i]->location.x = 
                    intersection->location.x;

                intersection->connections[i]->location.y = 
                    intersection->location.y + INTERSECTION_SIZE +
                    intersection->lengths[i] + 
                    intersection->connections[i]->lengths[opposite];
            }
            else if (i == EAST || i == WEST) {
                intersection->connections[i]->location.x = 
                    intersection->location.x + INTERSECTION_SIZE +
                    intersection->lengths[i] + 
                    intersection->connections[i]->lengths[opposite];

                intersection->connections[i]->location.y = 
                    intersection->location.y;
            }
            init_intersection_construction(intersection->connections[i]);
        }
    }
}