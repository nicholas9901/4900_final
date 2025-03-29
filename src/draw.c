#include "draw.h"

SDL_Window* win;
SDL_Surface* win_surface;
SDL_Renderer* renderer; //apply transformations to images
SDL_Event event;
SDL_Texture* car_texture;
SDL_Rect curr_pos;
intersection_rects_T intersection_rects[NUM_INTERSECTIONS];

char run_path[PATH_SIZE];
char path_assets_car[PATH_SIZE];

/* 
init_gui: 
    Initial draw function, draws the background and intersections 
*/
void init_gui(vehicle_T* vehicles, intersection_T* intersections)
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
        draw_intersection(&(intersections[i]));
    }

    for (int i = 0; i < NUM_VEHICLES; i++) {
        draw_vehicle(&(vehicles[i]));
    }

    SDL_UpdateWindowSurface(win);
}

/* 
update_gui: 
    Updates the GUI with vehicle positions and intersection phase changes
*/
void update_gui(vehicle_T* vehicles, intersection_T* intersections)
{
    for (int i = 0; i < NUM_VEHICLES; i++) {
        SDL_RenderFillRect(renderer, &curr_pos);
        curr_pos.x = vehicles[0].location.x;
        curr_pos.y = vehicles[0].location.y;
        // SDL_FillRect(win_surface, &curr_pos, SDL_MapRGB(win_surface->format, COLOR_YELLOW));
        SDL_RenderCopyEx(renderer, car_texture, NULL, &curr_pos, 0.0, NULL, SDL_FLIP_NONE);
    }
    SDL_RenderPresent(renderer);
}

void exit_gui()
{
    SDL_DestroyWindow(win);
}

//moves vehicle in the east-west directions
void move_horizontal(SDL_Texture* vehicle, SDL_Rect pos) {
    //east-west road lengths: 295
    //east-west yellow line: y = 215
    
    if (pos.y > 215) {//change to use vehicle direction?
        //west --> east
        while (pos.x < WINDOW_X) {
            SDL_Rect prev_pos = { pos.x, pos.y + 5, pos.w, pos.h - 12 };
            pos.x += 2;
            SDL_RenderFillRect(renderer, &prev_pos); //set previous position background color to black
            SDL_RenderCopyEx(renderer, vehicle, NULL, &pos, 90.0, NULL, SDL_FLIP_NONE);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }

    }
    else {
        //east --> west
        while (pos.x > 0) {
            SDL_Rect prev_pos = { pos.x, pos.y + 5, pos.w, pos.h - 12 };
            pos.x -= 2;
            SDL_RenderFillRect(renderer, &prev_pos);
            SDL_RenderCopyEx(renderer, vehicle, NULL, &pos, 90.0, NULL, SDL_FLIP_VERTICAL);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
    }
}

//moves vehicle in north-south direction
void move_vertical(SDL_Texture* vehicle, SDL_Rect pos) {
    //north-south road lengths: 215

    if (pos.y <= 215) {
        //north --> south
        while (pos.y < WINDOW_Y) {
            SDL_Rect prev_pos = { pos.x + 5, pos.y, pos.w - 10, pos.h };
            pos.y += 2;
            SDL_RenderFillRect(renderer, &prev_pos);
            SDL_RenderCopyEx(renderer, vehicle, NULL, &pos, 0.0, NULL, SDL_FLIP_VERTICAL);
            SDL_RenderPresent(renderer);

            //for testing right turns
            if (pos.y > 208) {//vehicle enters intersection
                right_turn(vehicle, pos);
                break;
            }

            SDL_Delay(16);
        }
    }
    else {
        //south --> north
        while (pos.y > 0) {
            SDL_Rect prev_pos = { pos.x + 5, pos.y, pos.w - 10, pos.h };
            pos.y -= 2;
            SDL_RenderFillRect(renderer, &prev_pos);
            SDL_RenderCopyEx(renderer, vehicle, NULL, &pos, 180.0, NULL, SDL_FLIP_VERTICAL);
            SDL_RenderPresent(renderer);

            //for testing right turns
            if (pos.y < WINDOW_Y - 240) {//vehicle enters intersection
                right_turn(vehicle, pos);
                break;
            }
            
            SDL_Delay(16);
        }
    }
}

//right turns
void right_turn(SDL_Texture* vehicle, SDL_Rect pos) {
    // north-south yellow line: 317.5

    if (pos.x > 317.5) {
        //south --> north
        SDL_Rect prev_pos = { pos.x + 5, pos.y, pos.w - 10, pos.h };
        SDL_RenderFillRect(renderer, &prev_pos);
        SDL_RenderCopyEx(renderer, vehicle, NULL, &pos, 270.0, NULL, SDL_FLIP_VERTICAL); //make right turn
        move_horizontal(vehicle, pos);
        SDL_RenderPresent(renderer);
    }
    else {
        //north --> south
        SDL_Rect prev_pos = { pos.x + 5, pos.y, pos.w - 10, pos.h };
        SDL_RenderFillRect(renderer, &prev_pos);
        SDL_RenderCopyEx(renderer, vehicle, NULL, &pos, 90.0, NULL, SDL_FLIP_VERTICAL); 
        move_horizontal(vehicle, pos);
        SDL_RenderPresent(renderer);
    }
}

//left turns

void draw_vehicle(vehicle_T* vehicle)
{
    //get attributes of texture
    curr_pos.w = CAR_SIZE;
    curr_pos.h = CAR_SIZE;
    curr_pos.x = vehicle->location.x;
    curr_pos.y = vehicle->location.y;
    SDL_RenderCopyEx(renderer, car_texture, NULL, &curr_pos, 0.0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
}

void draw_intersection(intersection_T* intersection)
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

    intersection_rects[intersection->id].divs[NORTH].w = DIV_WIDTH;
    intersection_rects[intersection->id].divs[NORTH].h = north.h;
    intersection_rects[intersection->id].divs[NORTH].x = north.x + DIV_OFFSET;
    intersection_rects[intersection->id].divs[NORTH].y = north.y;
    SDL_FillRect(win_surface, &(intersection_rects[intersection->id].divs[NORTH]), SDL_MapRGB(win_surface->format, COLOR_YELLOW));
        
    //east
    SDL_Rect east = { 
        .w = intersection->lengths[EAST],
        .h = INTERSECTION_SIZE,
        .x = center.x + INTERSECTION_SIZE,
        .y = center.y
    };
    SDL_FillRect(win_surface, &east, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    intersection_rects[intersection->id].divs[EAST].w = east.h;
    intersection_rects[intersection->id].divs[EAST].h = DIV_WIDTH;
    intersection_rects[intersection->id].divs[EAST].x = east.x;
    intersection_rects[intersection->id].divs[EAST].y = east.y + DIV_OFFSET;
    SDL_FillRect(win_surface, &(intersection_rects[intersection->id].divs[EAST]), SDL_MapRGB(win_surface->format, COLOR_YELLOW));
    
    //south
    SDL_Rect south = {
        .w = INTERSECTION_SIZE,
        .h = intersection->lengths[SOUTH],
        .x = center.x,
        .y = center.y + INTERSECTION_SIZE
    };
    SDL_FillRect(win_surface, &south, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    intersection_rects[intersection->id].divs[SOUTH].w = DIV_WIDTH;
    intersection_rects[intersection->id].divs[SOUTH].h = south.h;
    intersection_rects[intersection->id].divs[SOUTH].x = south.x + DIV_OFFSET;
    intersection_rects[intersection->id].divs[SOUTH].y = south.y;
    SDL_FillRect(win_surface, &(intersection_rects[intersection->id].divs[SOUTH]), SDL_MapRGB(win_surface->format, COLOR_YELLOW));

    //west
    SDL_Rect west = {
        .w = intersection->lengths[WEST],
        .h = INTERSECTION_SIZE,
        .x = center.x - intersection->lengths[WEST],
        .y = center.y
    };
    SDL_FillRect(win_surface, &west, SDL_MapRGB(win_surface->format, COLOR_BLACK));
    
    intersection_rects[intersection->id].divs[WEST].w = west.h;
    intersection_rects[intersection->id].divs[WEST].h = DIV_WIDTH;
    intersection_rects[intersection->id].divs[WEST].x = west.x;
    intersection_rects[intersection->id].divs[WEST].y = west.y + DIV_OFFSET;
    SDL_FillRect(win_surface, &(intersection_rects[intersection->id].divs[WEST]), SDL_MapRGB(win_surface->format, COLOR_YELLOW));
    
    /* Stopping points */
    intersection_rects[intersection->id].stops[NORTH].w = INTERSECTION_SIZE;
    intersection_rects[intersection->id].stops[NORTH].h = DIV_WIDTH;
    intersection_rects[intersection->id].stops[NORTH].x = center.x;
    intersection_rects[intersection->id].stops[NORTH].y = south.y;
    SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[NORTH]), SDL_MapRGB(win_surface->format, COLOR_GREEN));
    
    intersection_rects[intersection->id].stops[EAST].w = DIV_WIDTH;
    intersection_rects[intersection->id].stops[EAST].h = INTERSECTION_SIZE;
    intersection_rects[intersection->id].stops[EAST].x = center.x - DIV_WIDTH;
    intersection_rects[intersection->id].stops[EAST].y = center.y;
    SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[EAST]), SDL_MapRGB(win_surface->format, COLOR_GREEN));

    intersection_rects[intersection->id].stops[SOUTH].w = INTERSECTION_SIZE;
    intersection_rects[intersection->id].stops[SOUTH].h = DIV_WIDTH;
    intersection_rects[intersection->id].stops[SOUTH].x = center.x;
    intersection_rects[intersection->id].stops[SOUTH].y = center.y - DIV_WIDTH;
    SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[SOUTH]), SDL_MapRGB(win_surface->format, COLOR_GREEN));

    intersection_rects[intersection->id].stops[WEST].w = DIV_WIDTH;
    intersection_rects[intersection->id].stops[WEST].h = INTERSECTION_SIZE;
    intersection_rects[intersection->id].stops[WEST].x = east.x;
    intersection_rects[intersection->id].stops[WEST].y = center.y;
    SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[WEST]), SDL_MapRGB(win_surface->format, COLOR_GREEN));


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
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[NORTH]), SDL_MapRGB(win_surface->format, COLOR_BLUE));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[EAST]), SDL_MapRGB(win_surface->format, COLOR_RED));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[SOUTH]), SDL_MapRGB(win_surface->format, COLOR_BLUE));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[WEST]), SDL_MapRGB(win_surface->format, COLOR_RED));
            break;
        case VERTICAL_SR:
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[NORTH]), SDL_MapRGB(win_surface->format, COLOR_GREEN));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[EAST]), SDL_MapRGB(win_surface->format, COLOR_RED));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[SOUTH]), SDL_MapRGB(win_surface->format, COLOR_GREEN));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[WEST]), SDL_MapRGB(win_surface->format, COLOR_RED));
            break;
        case HORIZONTAL_L:
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[NORTH]), SDL_MapRGB(win_surface->format, COLOR_RED));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[EAST]), SDL_MapRGB(win_surface->format, COLOR_BLUE));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[SOUTH]), SDL_MapRGB(win_surface->format, COLOR_RED));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[WEST]), SDL_MapRGB(win_surface->format, COLOR_BLUE));
            break;
        case HORIZONTAL_SR:
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[NORTH]), SDL_MapRGB(win_surface->format, COLOR_RED));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[EAST]), SDL_MapRGB(win_surface->format, COLOR_GREEN));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[SOUTH]), SDL_MapRGB(win_surface->format, COLOR_RED));
            SDL_FillRect(win_surface, &(intersection_rects[intersection->id].stops[WEST]), SDL_MapRGB(win_surface->format, COLOR_GREEN));
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

    if (!car_texture) {
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