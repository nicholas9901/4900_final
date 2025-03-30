#include "prototypes.h"

#if GUI
#include "draw.h"
#include "print.h"
#endif

int main(int argc, char** argv) {
    
    /* 
    Get the path of the program to load the assets if the GUI build is running 
    */
    #if GUI
    realpath(argv[0], run_path);
    #endif

    /* Initializing the structures */
    intersection_T intersections[NUM_INTERSECTIONS];
    vehicle_T      vehicles[NUM_VEHICLES_TRAFFIC];
    vehicle_T      emergency_vehicle;
    vehicle_list_T active_vehicles;
    direction_T    list_instructions[2];
    list_instructions[0] = SOUTH;
    list_instructions[1] = WEST;

    instructions_T instructions_1;
    init_instructions(&instructions_1, list_instructions, 2);
    
    int road_lengths_1[MAX_CONNECTIONS] = {DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH, DEFAULT_LENGTH};

    init_intersection(
        &(intersections[0]), 
        &(intersections[0]), 
        &(intersections[1]), 
        &(intersections[0]), 
        &(intersections[1]),
        road_lengths_1,
        0);

    init_intersection(
        &(intersections[1]), 
        &(intersections[1]), 
        &(intersections[0]), 
        &(intersections[1]), 
        &(intersections[0]), 
        road_lengths_1,
        1);

    /* Intersection coordinate generation */
    init_vector(
        &(intersections[0].location),
        INIT_INTERSECTION_START_X, 
        INIT_INTERSECTION_START_Y);

    init_intersection_construction(&(intersections[0]));
    
    /* Emergency vehicle */
    init_vehicle(
        &emergency_vehicle,
        &(intersections[0]),
        &instructions_1,        
        PRIORITY_HIGH,
        EMERGENCY_SPEED,
        -1);

    /* Traffic vehicles */
    init_vehicle(
        &(vehicles[0]),
        &(intersections[0]),
        &instructions_1,        
        PRIORITY_LOW,
        DEFAULT_SPEED,
        0);
    
    /* 
    Initially all vehicles are in the active state, put them all in. 
    Whether a vehicle is active or not determines if it is moving or waiting
    in queue at a red light.
    */
    init_active_vehicles(&active_vehicles, &emergency_vehicle, vehicles);

    #if GUI
    // print_vehicle(&(vehicles[0]), 1);
    // print_intersection(&(intersections[0]), 1);
    // print_intersection(&(intersections[1]), 2);
    init_gui(vehicles, &emergency_vehicle, intersections);
    #endif
    
    /* Main program loop */
    while(emergency_arrived(&emergency_vehicle)) {
        for (int i = 0; i < active_vehicles.num; i++) {
            if (!move_vehicle(active_vehicles.vehicles[i])) {
                enqueue_vehicle(
                    &active_vehicles, 
                    active_vehicles.vehicles[i], 
                    i);
            }
        }

        for (int i = 0; i < NUM_INTERSECTIONS; i++) {
            if (phase_timer(&active_vehicles, &(intersections[i]))) {
                #if GUI
                print_intersection(&(intersections[i]), i);
                #endif
            }
        }
        #if GUI
        poll_for_exit(); /* Listen for an exit event before updating */
        update_gui(vehicles, &emergency_vehicle, intersections);
        #endif
        usleep(SLEEP_INTERVAL);
    }

    #if GUI
    while(1) {
        poll_for_exit();
    }
    #endif

    printf("exit\n");
}