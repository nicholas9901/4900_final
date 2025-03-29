#include "print.h"

void print_vector(vector_T* vector)
{
    printf("(%d,%d)", vector->x, vector->y);
}

void print_direction(direction_T direction)
{
    char str[6]; 
    switch (direction) {
        case NORTH:
            strcpy(str, "NORTH");
            break;
        case EAST:
            strcpy(str, "EAST");
            break;
        case SOUTH:
            strcpy(str, "SOUTH");
            break;
        case WEST:
            strcpy(str, "WEST");
            break;
        default:
            direction_error(direction);
    }
    printf("%s", str);
}

void print_instructions(instructions_T* instructions)
{
    printf("Current Instruction: ");
    print_direction(instructions->list[instructions->current]);
    printf(" [%d]\n", instructions->current);
    printf("All instructions:\n");
    for (int i = 0; i < instructions->num; i++) {
        printf("    ");
        print_direction(instructions->list[i]);
        printf(" [%d]\n", i);
    }
}

void print_vehicle(vehicle_T* vehicle, int id)
{
    printf("Vehicle %d:\n   (x,y): ", id);
    print_vector(&(vehicle->location));
    printf("\n  Speed: %d\n", vehicle->speed);
    printf("  Turning: %d\n", vehicle->turning);
    printf(" Stopping: %d\n", vehicle->stopping);
    print_instructions(vehicle->instructions);
}

void print_intersection(intersection_T* intersection, int id)
{
    printf("Intersection %d:\n  (x,y): ", id);
    print_vector(&(intersection->location));
    
    /* Endpoints */
    printf("\n  Endpoints:\n    North: ");
    print_vector(&(intersection->endpoints[NORTH]));
    printf("\n    East: ");
    print_vector(&(intersection->endpoints[EAST]));
    printf("\n    South: ");
    print_vector(&(intersection->endpoints[SOUTH]));
    printf("\n    West: ");
    print_vector(&(intersection->endpoints[WEST]));

    /* Turning Points */
    printf(
        "\n  Turning Points:\n    North: %d\n    East: %d\n    South: %d\n    West: %d\n",
        intersection->turning_points[NORTH],
        intersection->turning_points[EAST],
        intersection->turning_points[SOUTH],
        intersection->turning_points[WEST]
    );
}