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
    return (direction + 2) % MAX_CONNECTIONS;
}