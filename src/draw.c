#include "draw.h"

SDL_Window* win;
SDL_Surface* win_surface;
SDL_Renderer* renderer; //apply transformations to images
SDL_Event event;
SDL_Texture* car_texture;
SDL_Texture* ambulance_texture;
vehicle_SDL_T vehicle_icons[NUM_VEHICLES_TRAFFIC];
vehicle_SDL_T emergency_vehicle_icon;
intersection_SDL_T intersection_icons[NUM_INTERSECTIONS];
SDL_Rect dispatch_point_icon;

char run_path[PATH_SIZE];
char path_assets_car[PATH_SIZE];
char path_assets_ambulance[PATH_SIZE];

/* 
init_gui: 
    Initial draw function, draws the background and intersections 
*/
void init_gui(
    vehicle_T* vehicles, 
    vehicle_T* emergency_vehicle, 
    intersection_T* intersections,
    vector_T* dispatch_point)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    win = SDL_CreateWindow(
        WINDOW_NAME, 
        100, 
        100, 
        WINDOW_X, 
        WINDOW_Y, 
        SDL_WINDOW_SHOWN);

    
    if (!win) {
        printf("Failed to create window: %s", SDL_GetError());
        exit(1);
    }

    win_surface = SDL_GetWindowSurface(win);
    
    if (!win_surface) {
        printf("Failed to create window surface: %s", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);

    if (!renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    initialize_assets();

    /* Set the background color */
    SDL_FillRect(win_surface, NULL, SDL_MapRGB(win_surface->format, COLOR_WHITE));
    for (int i = 0; i < NUM_INTERSECTIONS; i++) {
        init_draw_intersection(&(intersections[i]));
    }

    init_draw_emergency_vehicle(emergency_vehicle);

    for (int i = 0; i < NUM_VEHICLES_TRAFFIC; i++) {
        init_draw_vehicle(&(vehicles[i]));
    }

    /* Initialize the dispatch point */
    dispatch_point_icon.x = dispatch_point->x;
    dispatch_point_icon.y = dispatch_point->y;
    dispatch_point_icon.w = VEHICLE_SIZE;
    dispatch_point_icon.h = VEHICLE_SIZE;

    SDL_UpdateWindowSurface(win);
}

/* 
update_gui: 
    Updates the GUI with vehicle positions and intersection phase changes
*/
void update_gui(
    vehicle_T* vehicles, 
    vehicle_T* emergency_vehicle,
    intersection_T* intersections,
    vector_T* dispatch_point)
{
    static const double rotation_vector[4] = {0.0, 90.0, 180.0, 270.0};

    SDL_SetRenderDrawColor(renderer, COLOR_GREEN, 1);
    SDL_RenderFillRect(renderer, &(dispatch_point_icon));

    SDL_SetRenderDrawColor(renderer, COLOR_BLACK, 1);
    SDL_RenderFillRect(renderer, &(emergency_vehicle_icon.pos));
    emergency_vehicle_icon.pos.x = emergency_vehicle->location.x;
    emergency_vehicle_icon.pos.y = emergency_vehicle->location.y;
    SDL_RenderCopyEx(
        renderer, 
        emergency_vehicle_icon.texture, 
        NULL, 
        &(emergency_vehicle_icon.pos), 
        rotation_vector[emergency_vehicle->instructions.list[emergency_vehicle->instructions.current]], 
        NULL, 
        SDL_FLIP_NONE);

    for (int i = 0; i < NUM_VEHICLES_TRAFFIC; i++) {
        SDL_RenderFillRect(renderer, &(vehicle_icons[i].pos));
        vehicle_icons[i].pos.x = vehicles[i].location.x;
        vehicle_icons[i].pos.y = vehicles[i].location.y;
        SDL_RenderCopyEx(
            renderer, 
            vehicle_icons[i].texture, 
            NULL, 
            &(vehicle_icons[i].pos), 
            rotation_vector[vehicles[i].instructions.list[vehicles[i].instructions.current]], 
            NULL, 
            SDL_FLIP_NONE);
    }

    SDL_SetRenderDrawColor(renderer, COLOR_YELLOW, 1);
    for (int i = 0; i < NUM_INTERSECTIONS; i++) {
        SDL_RenderFillRects(renderer, intersection_icons[i].divs, NUM_DIRECTIONS);
        for (int j = 0; j < MAX_CONNECTIONS; j++) {
            SDL_FillRect(
                win_surface, 
                &(intersection_icons[i].stops[j]),
                intersection_icons[i].stop_colors[j]);    
        }
    }

    SDL_RenderPresent(renderer);
}

void exit_gui()
{
    SDL_DestroyWindow(win);
}

void init_draw_vehicle(vehicle_T* vehicle)
{
    vehicle_icons[vehicle->id].texture = car_texture;
    vehicle_icons[vehicle->id].pos.w   = VEHICLE_SIZE;
    vehicle_icons[vehicle->id].pos.h   = VEHICLE_SIZE;
    vehicle_icons[vehicle->id].pos.x   = vehicle->location.x;
    vehicle_icons[vehicle->id].pos.y   = vehicle->location.y;
    SDL_RenderCopyEx(renderer, vehicle_icons[vehicle->id].texture, NULL, &(vehicle_icons[vehicle->id].pos), 0.0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
}

void init_draw_emergency_vehicle(vehicle_T* vehicle)
{
    emergency_vehicle_icon.texture = ambulance_texture;
    emergency_vehicle_icon.pos.w   = VEHICLE_SIZE;
    emergency_vehicle_icon.pos.h   = VEHICLE_SIZE;
    emergency_vehicle_icon.pos.x   = vehicle->location.x;
    emergency_vehicle_icon.pos.y   = vehicle->location.y;
    SDL_RenderCopyEx(renderer, emergency_vehicle_icon.texture, NULL, &(emergency_vehicle_icon.pos), 0.0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
}

void init_draw_intersection(intersection_T* intersection)
{
    SDL_Surface* win_surface = SDL_GetWindowSurface(win);

    //intersection
    SDL_Rect center = { 
        .w = INTERSECTION_SIZE, 
        .h = INTERSECTION_SIZE,
        .x = intersection->location.x,
        .y = intersection->location.y
    };
    SDL_FillRect(win_surface, &center, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    //north
    SDL_Rect north = { 
        .w = INTERSECTION_SIZE,
        .h = intersection->lengths[NORTH],
        .x = center.x,
        .y = center.y - intersection->lengths[NORTH],
    };
    SDL_FillRect(win_surface, &north, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    intersection_icons[intersection->id].divs[NORTH].w = DIV_WIDTH;
    intersection_icons[intersection->id].divs[NORTH].h = north.h - DIV_WIDTH;
    intersection_icons[intersection->id].divs[NORTH].x = north.x + DIV_OFFSET;
    intersection_icons[intersection->id].divs[NORTH].y = north.y;
    SDL_FillRect(win_surface, &(intersection_icons[intersection->id].divs[NORTH]), SDL_MapRGB(win_surface->format, COLOR_YELLOW));
        
    //east
    SDL_Rect east = { 
        .w = intersection->lengths[EAST],
        .h = INTERSECTION_SIZE,
        .x = center.x + INTERSECTION_SIZE,
        .y = center.y
    };
    SDL_FillRect(win_surface, &east, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    intersection_icons[intersection->id].divs[EAST].w = east.w - DIV_WIDTH;
    intersection_icons[intersection->id].divs[EAST].h = DIV_WIDTH;
    intersection_icons[intersection->id].divs[EAST].x = east.x + DIV_WIDTH;
    intersection_icons[intersection->id].divs[EAST].y = east.y + DIV_OFFSET;
    SDL_FillRect(win_surface, &(intersection_icons[intersection->id].divs[EAST]), SDL_MapRGB(win_surface->format, COLOR_YELLOW));
    
    //south
    SDL_Rect south = {
        .w = INTERSECTION_SIZE,
        .h = intersection->lengths[SOUTH],
        .x = center.x,
        .y = center.y + INTERSECTION_SIZE
    };
    SDL_FillRect(win_surface, &south, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    intersection_icons[intersection->id].divs[SOUTH].w = DIV_WIDTH;
    intersection_icons[intersection->id].divs[SOUTH].h = south.h - DIV_WIDTH;
    intersection_icons[intersection->id].divs[SOUTH].x = south.x + DIV_OFFSET;
    intersection_icons[intersection->id].divs[SOUTH].y = south.y + DIV_WIDTH; 
    SDL_FillRect(win_surface, &(intersection_icons[intersection->id].divs[SOUTH]), SDL_MapRGB(win_surface->format, COLOR_YELLOW));

    //west
    SDL_Rect west = {
        .w = intersection->lengths[WEST],
        .h = INTERSECTION_SIZE,
        .x = center.x - intersection->lengths[WEST],
        .y = center.y
    };
    SDL_FillRect(win_surface, &west, SDL_MapRGB(win_surface->format, COLOR_BLACK));
    
    intersection_icons[intersection->id].divs[WEST].w = west.w - DIV_WIDTH;
    intersection_icons[intersection->id].divs[WEST].h = DIV_WIDTH;
    intersection_icons[intersection->id].divs[WEST].x = west.x;
    intersection_icons[intersection->id].divs[WEST].y = west.y + DIV_OFFSET;
    SDL_FillRect(win_surface, &(intersection_icons[intersection->id].divs[WEST]), SDL_MapRGB(win_surface->format, COLOR_YELLOW));
    
    /* Stopping points */
    intersection_icons[intersection->id].stops[NORTH].w = INTERSECTION_SIZE;
    intersection_icons[intersection->id].stops[NORTH].h = DIV_WIDTH;
    intersection_icons[intersection->id].stops[NORTH].x = center.x;
    intersection_icons[intersection->id].stops[NORTH].y = south.y;
    
    intersection_icons[intersection->id].stops[EAST].w = DIV_WIDTH;
    intersection_icons[intersection->id].stops[EAST].h = INTERSECTION_SIZE;
    intersection_icons[intersection->id].stops[EAST].x = center.x - DIV_WIDTH;
    intersection_icons[intersection->id].stops[EAST].y = center.y;

    intersection_icons[intersection->id].stops[SOUTH].w = INTERSECTION_SIZE;
    intersection_icons[intersection->id].stops[SOUTH].h = DIV_WIDTH;
    intersection_icons[intersection->id].stops[SOUTH].x = center.x;
    intersection_icons[intersection->id].stops[SOUTH].y = center.y - DIV_WIDTH;

    intersection_icons[intersection->id].stops[WEST].w = DIV_WIDTH;
    intersection_icons[intersection->id].stops[WEST].h = INTERSECTION_SIZE;
    intersection_icons[intersection->id].stops[WEST].x = east.x;
    intersection_icons[intersection->id].stops[WEST].y = center.y;
    draw_phase_change(intersection);

    SDL_UpdateWindowSurface(win);
}

/*
Draw Phase Change
    Colors the stopping rectangles depending on the current phase.
        Green: Straight and left turns
        Blue:  Left turns only
        Red:   Stop
*/
void draw_phase_change(intersection_T* intersection)
{
    switch (intersection->phase) {
        case VERTICAL_L:
            intersection_icons[intersection->id].stop_colors[NORTH] = SDL_MapRGB(win_surface->format, COLOR_BLUE);
            intersection_icons[intersection->id].stop_colors[EAST]  = SDL_MapRGB(win_surface->format, COLOR_RED);
            intersection_icons[intersection->id].stop_colors[SOUTH] = SDL_MapRGB(win_surface->format, COLOR_BLUE);
            intersection_icons[intersection->id].stop_colors[WEST]  = SDL_MapRGB(win_surface->format, COLOR_RED);
            break;
        case VERTICAL_SR:
            intersection_icons[intersection->id].stop_colors[NORTH] = SDL_MapRGB(win_surface->format, COLOR_GREEN);
            intersection_icons[intersection->id].stop_colors[EAST]  = SDL_MapRGB(win_surface->format, COLOR_RED);
            intersection_icons[intersection->id].stop_colors[SOUTH] = SDL_MapRGB(win_surface->format, COLOR_GREEN);
            intersection_icons[intersection->id].stop_colors[WEST]  = SDL_MapRGB(win_surface->format, COLOR_RED);
            break;
        case HORIZONTAL_L:
            intersection_icons[intersection->id].stop_colors[NORTH] = SDL_MapRGB(win_surface->format, COLOR_RED);
            intersection_icons[intersection->id].stop_colors[EAST]  = SDL_MapRGB(win_surface->format, COLOR_BLUE);
            intersection_icons[intersection->id].stop_colors[SOUTH] = SDL_MapRGB(win_surface->format, COLOR_RED);
            intersection_icons[intersection->id].stop_colors[WEST]  = SDL_MapRGB(win_surface->format, COLOR_BLUE);
            break;
        case HORIZONTAL_SR:
            intersection_icons[intersection->id].stop_colors[NORTH] = SDL_MapRGB(win_surface->format, COLOR_RED);
            intersection_icons[intersection->id].stop_colors[EAST]  = SDL_MapRGB(win_surface->format, COLOR_GREEN);
            intersection_icons[intersection->id].stop_colors[SOUTH] = SDL_MapRGB(win_surface->format, COLOR_RED);
            intersection_icons[intersection->id].stop_colors[WEST]  = SDL_MapRGB(win_surface->format, COLOR_GREEN);
            break;
    }
}

void poll_for_exit()
{
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
        exit(1);
    }
}

void initialize_assets()
{
    trim_run_path();
    /* Car */
    strcpy(path_assets_car, run_path);
    strcat(path_assets_car, PATH_IMG_CAR);
    car_texture = IMG_LoadTexture(renderer, path_assets_car);
    
    /* Ambulance */
    strcpy(path_assets_ambulance, run_path);
    strcat(path_assets_ambulance, PATH_IMG_AMBULANCE);
    ambulance_texture = IMG_LoadTexture(renderer, path_assets_ambulance);

    if (!car_texture) {
        printf("Failed to load vehicle icon (%s), %s\n", path_assets_car, SDL_GetError());
    }

    if (!ambulance_texture) {
        printf("Failed to load vehicle icon (%s), %s\n", path_assets_car, SDL_GetError());
    }
}

void trim_run_path()
{
    char* base_path = PATH_BASE;
    char* j = base_path;
    char* i_curr;

    for (char* i = run_path; *i != '\0'; i++) {
        i_curr = i;
        j = base_path;
        while (*i_curr == *j) {
            j++;
            i_curr++;
            if (*j == '\0') {
                *(i_curr + 1) = '\0';
                return;
            }
        }
    }

    printf("Error: Could not establish correct path\n");
    exit(1);
}