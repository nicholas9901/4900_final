#include "prototypes.h"

void init_intersection(
    intersection_T* intersection,
    intersection_T* connection_north,
    intersection_T* connection_east,
    intersection_T* connection_south,
    intersection_T* connection_west,
    float road_lengths[MAX_CONNECTIONS])
{
    intersection->curr_phase         = HORIZONTAL_SR;
    intersection->connections[NORTH] = connection_north;
    intersection->connections[EAST]  = connection_east;
    intersection->connections[SOUTH] = connection_south;
    intersection->connections[WEST]  = connection_west;
    
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        intersection->road_lengths[i] = road_lengths[i];
    }
}