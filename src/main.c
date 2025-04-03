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
    vehicle_T       emergency_vehicle;
    vehicle_list_T  active_vehicles;
    direction_T     list_instructions[NUM_INST_1];
    vector_T        dispatch_point;
    avg_time_T      execution_time;
    int             ticks = 0; /* Used to determine response time */
    struct timespec next_wakeup, start_time, end_time; /* Used to assess performance */

    list_instructions[0] = EAST;
    list_instructions[1] = EAST;
    list_instructions[2] = SOUTH;
    list_instructions[3] = WEST;
    list_instructions[4] = NORTH;

    instructions_T instructions_1;
    init_instructions(&instructions_1, list_instructions, NUM_INST_1);
    
    int road_lengths_1[MAX_CONNECTIONS] = {DEFAULT_LENGTH+30, DEFAULT_LENGTH+30, DEFAULT_LENGTH, DEFAULT_LENGTH};

    /* Initialize the intersections */
    init_intersection(&(intersections[0]), road_lengths_1, 0);
    init_intersection(&(intersections[1]), road_lengths_1, 1);
    init_intersection(&(intersections[2]), road_lengths_1, 2);
    init_intersection(&(intersections[3]), road_lengths_1, 3);
    init_intersection(&(intersections[4]), road_lengths_1, 4);
    
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
            // if (tlc_baseline(&active_vehicles, &(intersections[i]))) {}
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