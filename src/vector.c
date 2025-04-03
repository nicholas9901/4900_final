#include "prototypes.h"

void init_vector(vector_T* vector, int x, int y)
{
    vector->x = x;
    vector->y = y;
}

bool emergency_arrived(vector_T* dispatch_point, vehicle_T* emergency_vehicle) 
{
    int xd = emergency_vehicle->location.x - dispatch_point->x;
    int yd = emergency_vehicle->location.y - dispatch_point->y;
    if ((xd * xd) + (yd * yd) <= SUCCESS_DISTANCE_THRESHOLD) {
        return true;
    }
    return false; 
}