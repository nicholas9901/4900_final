#include "print.h"

void print_vector(vector_T* vector)
{
    printf("(%d,%d)", vector->x, vector->y);
}

void print_vehicle(vehicle_T* vehicle, int id)
{
    printf("Vehicle %d:\n       (x,y): ", id);
    print_vector(&(vehicle->location));
    printf(
        "speed: %d\n",
        vehicle->speed);
    // TODO: need to add print instructions function
}
