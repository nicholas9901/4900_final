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
    print_instructions(&(vehicle->instructions));
}

void print_intersection(intersection_T* intersection)
{
    printf("Intersection %d:\n  (x,y): ", intersection->id);
    print_vector(&(intersection->location));
    
    /* Queued Vehicles */
    printf("  Queued Vehicles:\n");
    for (int direction = 0; direction < NUM_DIRECTIONS; direction++) {
        printf("    ");
        print_direction(direction);
        printf("[%d]: ", intersection->queued_vehicles[direction].num);
        for (int i = 0; i < intersection->queued_vehicles[direction].num; i++) {
            printf("%d, ", intersection->queued_vehicles[direction].vehicles[i]->id);
        }
        printf("\n");
    }
}


/*
Print Average Queue Times
    Prints the average queue times of each intersection measured in the number
    of simulation ticks.
*/
void print_avg_queue_times(vehicle_T* vehicles, vehicle_T* emergency_vehicle)
{
    double total_all = 0;
    for (int i = 0; i < NUM_VEHICLES_TRAFFIC; i++) {
        double avg = vehicles[i].queue_time.total / vehicles[i].queue_time.num;
        printf(
            "Average queue time for vehicle [%d]: %f\n",
            vehicles[i].id,
            avg);
        total_all += avg;
    }
    printf(
        "Average queue time of all vehicles: %f\n", 
        total_all / NUM_VEHICLES_TRAFFIC);
    printf(
        "Average queue time for the emergency vehicle: %f\n",
        emergency_vehicle->queue_time.total / emergency_vehicle->queue_time.num);
}
