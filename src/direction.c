#include "prototypes.h"

void direction_error(direction_T direction)
{
    printf("Error: Direction enum out of bounds (%d)\n", direction);
    exit(1);
}

/*
Determine Connection
    Given a direction, returns the logical connection
*/
direction_T determine_connection(direction_T direction)
{
    switch (direction) {
        case NORTH:
            return SOUTH;
        case EAST:
            return WEST;
        case SOUTH:
            return NORTH;
        case WEST:
            return EAST;
        default:
            direction_error(direction);
    }
}