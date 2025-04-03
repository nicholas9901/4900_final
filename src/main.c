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
    intersection_T  intersections[NUM_INTERSECTIONS];
    vehicle_T       vehicles[NUM_VEHICLES_TRAFFIC];
    //vehicle_T       vehicles[2];
    vehicle_T       emergency_vehicle;
    vehicle_list_T  active_vehicles;
    //direction_T     list_instructions[NUM_INST_1];
    vector_T        dispatch_point;
    avg_time_T      execution_time;
    int             ticks = 0; /* Used to determine response time */
    struct timespec next_wakeup, start_time, end_time; /* Used to assess performance */


    //Scenario 1 (high traffic density): set NUM_VEHICLES_TRAFFIC = 14, NUM_INTERSECTIONS = 7
    direction_T list_instructions1[NUM_INST_1] = { EAST, EAST, SOUTH, WEST, NORTH };
    direction_T list_instructions2[NUM_INST_1] = { EAST, EAST, SOUTH, WEST, SOUTH };
    direction_T list_instructions3[NUM_INST_1] = { WEST, WEST, SOUTH, EAST, SOUTH };
    direction_T list_instructions4[NUM_INST_1] = { WEST, WEST, NORTH, EAST, NORTH };
    direction_T list_instructions5[NUM_INST_1] = { SOUTH, EAST, NORTH, EAST, SOUTH };
    direction_T list_instructions6[NUM_INST_1] = { EAST, EAST, EAST, NORTH, WEST };
    direction_T list_instructions7[NUM_INST_1] = { NORTH, WEST, SOUTH, WEST, NORTH };
    direction_T list_instructions8[NUM_INST_1] = { WEST, SOUTH, WEST, NORTH, WEST };
    direction_T list_instructions9[NUM_INST_1] = { NORTH, EAST, NORTH, WEST, WEST };
    direction_T list_instructions10[NUM_INST_1] = { SOUTH, EAST, NORTH, EAST, NORTH };
    direction_T list_instructions11[NUM_INST_1] = { WEST, WEST, SOUTH, EAST, EAST };
    direction_T list_instructions12[NUM_INST_1] = { WEST, NORTH, WEST, WEST, WEST };
    direction_T list_instructions13[NUM_INST_1] = { SOUTH, EAST, SOUTH, EAST, SOUTH };
    direction_T list_instructions14[NUM_INST_1] = { NORTH, NORTH, WEST, SOUTH, WEST };



    instructions_T instructions_1, instructions_2, instructions_3, instructions_4, instructions_5, 
        instructions_6, instructions_7, instructions_8, instructions_9, instructions_10, instructions_11,
        instructions_12, instructions_13, instructions_14;

    init_instructions(&instructions_1, list_instructions1, NUM_INST_1);
    init_instructions(&instructions_2, list_instructions2, NUM_INST_1);
    init_instructions(&instructions_3, list_instructions3, NUM_INST_1);
    init_instructions(&instructions_4, list_instructions4, NUM_INST_1);
    init_instructions(&instructions_5, list_instructions5, NUM_INST_1);
    init_instructions(&instructions_6, list_instructions6, NUM_INST_1);
    init_instructions(&instructions_7, list_instructions7, NUM_INST_1);
    init_instructions(&instructions_8, list_instructions8, NUM_INST_1);
    init_instructions(&instructions_9, list_instructions9, NUM_INST_1);
    init_instructions(&instructions_10, list_instructions10, NUM_INST_1);
    init_instructions(&instructions_11, list_instructions11, NUM_INST_1);
    init_instructions(&instructions_12, list_instructions12, NUM_INST_1);
    init_instructions(&instructions_13, list_instructions13, NUM_INST_1);
    init_instructions(&instructions_14, list_instructions14, NUM_INST_1);


   
    int road_lengths_1[MAX_CONNECTIONS] = {DEFAULT_LENGTH+30, DEFAULT_LENGTH+30, DEFAULT_LENGTH, DEFAULT_LENGTH};

    /* Initialize the intersections */
    init_intersection(&(intersections[0]), road_lengths_1, 0);
    init_intersection(&(intersections[1]), road_lengths_1, 1);
    init_intersection(&(intersections[2]), road_lengths_1, 2);
    init_intersection(&(intersections[3]), road_lengths_1, 3);
    init_intersection(&(intersections[4]), road_lengths_1, 4);
    //extra intersections (scenario 1)
    init_intersection(&(intersections[5]), road_lengths_1, 5);
    init_intersection(&(intersections[6]), road_lengths_1, 6);


    
    /* Determine position and lengths for the first intersection */
    init_vector(
        &(intersections[0].location),
        INIT_INTERSECTION_START_X, 
        INIT_INTERSECTION_START_Y);
    calculate_intersection_lengths(&(intersections[0]));

    connect_intersection(&(intersections[0]), &(intersections[1]), EAST);
    connect_intersection(&(intersections[0]), &(intersections[2]), WEST);
    connect_intersection(&(intersections[0]), &(intersections[3]), NORTH);
    connect_intersection(&(intersections[1]), &(intersections[4]), NORTH);
    connect_intersection(&(intersections[3]), &(intersections[4]), EAST);
    //extra intersections (scenario 1)
    connect_intersection(&(intersections[2]), &(intersections[5]), NORTH);
    connect_intersection(&(intersections[3]), &(intersections[5]), WEST);
    connect_intersection(&(intersections[5]), &(intersections[6]), WEST);

    
    /* Set the dispatch point */
    init_vector(
        &dispatch_point,
        intersections[4].end_points[EAST],
        intersections[4].location.y + LANE_OFFSET);

    /* Emergency vehicle */
    init_vehicle(
        &emergency_vehicle,
        &(intersections[3]),
        &instructions_1,        
        PRIORITY_HIGH,
        EMERGENCY_SPEED,
        -1);

    /* Traffic vehicles */

    //Scenario 1 vehicles
    init_vehicle(&(vehicles[0]), &(intersections[0]), &instructions_1, PRIORITY_LOW, DEFAULT_SPEED, 0);
    init_vehicle(&(vehicles[1]), &(intersections[2]), &instructions_2, PRIORITY_LOW, DEFAULT_SPEED, 1);
    init_vehicle(&(vehicles[2]), &(intersections[4]), &instructions_3, PRIORITY_LOW, DEFAULT_SPEED, 2);
    init_vehicle(&(vehicles[3]), &(intersections[2]), &instructions_4, PRIORITY_LOW, DEFAULT_SPEED, 3);
    init_vehicle(&(vehicles[4]), &(intersections[3]), &instructions_5, PRIORITY_LOW, DEFAULT_SPEED, 4);
    init_vehicle(&(vehicles[5]), &(intersections[0]), &instructions_6, PRIORITY_LOW, DEFAULT_SPEED, 5);
    init_vehicle(&(vehicles[6]), &(intersections[3]), &instructions_7, PRIORITY_LOW, DEFAULT_SPEED, 6);
    init_vehicle(&(vehicles[7]), &(intersections[4]), &instructions_8, PRIORITY_LOW, DEFAULT_SPEED, 7);
    init_vehicle(&(vehicles[8]), &(intersections[1]), &instructions_9, PRIORITY_LOW, DEFAULT_SPEED, 8);
    init_vehicle(&(vehicles[9]), &(intersections[5]), &instructions_10, PRIORITY_LOW, DEFAULT_SPEED, 9);
    init_vehicle(&(vehicles[10]), &(intersections[6]), &instructions_11, PRIORITY_LOW, DEFAULT_SPEED, 10);
    init_vehicle(&(vehicles[11]), &(intersections[1]), &instructions_12, PRIORITY_LOW, DEFAULT_SPEED, 11);
    init_vehicle(&(vehicles[12]), &(intersections[6]), &instructions_13, PRIORITY_LOW, DEFAULT_SPEED, 12);
    init_vehicle(&(vehicles[13]), &(intersections[0]), &instructions_14, PRIORITY_LOW, DEFAULT_SPEED, 13);

    /* 
    Initially all vehicles are in the active state, put them all in. 
    Whether a vehicle is active or not determines if it is moving or waiting
    in queue at a red light.
    */
    init_active_vehicles(&active_vehicles, &emergency_vehicle, vehicles);

    #if GUI
    init_gui(vehicles, &emergency_vehicle, intersections, &dispatch_point);
    #endif
    
    clock_gettime(CLOCK_MONOTONIC, &next_wakeup);

    /* Main program loop */
    while (!emergency_arrived(&dispatch_point, &emergency_vehicle)) {

        clock_gettime(CLOCK_MONOTONIC, &start_time); /* Record start time */

        for (int i = 0; i < active_vehicles.num; i++) {
            if (!move_vehicle(active_vehicles.vehicles[i])) {
                enqueue_vehicle(
                    &active_vehicles, 
                    active_vehicles.vehicles[i], 
                    i);
            }
        }

        for (int i = 0; i < NUM_INTERSECTIONS; i++) {
            #if GUI
            record_queue_times(&(intersections[i]));
            #endif
            //if (tlc_baseline(&active_vehicles, &(intersections[i]))) {}
            if (tlc_emergency(&active_vehicles, &emergency_vehicle, &(intersections[i]))) {}
        }
        #if GUI
        poll_for_exit(); /* Listen for an exit event before updating */
        update_gui(vehicles, &emergency_vehicle, intersections, &dispatch_point);
        #endif

        clock_gettime(CLOCK_MONOTONIC, &end_time); /* Record end time */
        ticks++;
        next_wakeup.tv_nsec += SLEEP_INTERVAL;

        /* Correct nanosecond overflow */
        if (next_wakeup.tv_nsec >= NANOSECOND) {
            next_wakeup.tv_sec += 1;
            next_wakeup.tv_nsec -= NANOSECOND;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_wakeup, NULL);
        execution_time.total += (end_time.tv_nsec - start_time.tv_nsec) / 1e6;
    }
    execution_time.num = ticks;

    /* Summarize the data */
    #if GUI
    printf("Emergency vehicle arrived in %d ticks\n", ticks);
    printf("Average Execution Time (ms): %f\n", execution_time.total / execution_time.num);
    print_avg_queue_times(vehicles, &emergency_vehicle);
    while(1) {
        poll_for_exit();
    }
    #else
    printf("%f", execution_time.total / execution_time.num);
    #endif
}