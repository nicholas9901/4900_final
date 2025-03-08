#include "prototypes.h"
#include <unistd.h>
#include <SDL2/SDL.h>

#define WINDOW_NAME "Traffic Simulator"
#define WINDOW_X    640
#define WINDOW_Y    480

#define COLOR_WHITE 255, 255, 255
#define COLOR_GRAY  150, 150, 150
#define COLOR_BLACK 0, 0, 0

SDL_Window* win;

/* draw_gui: Initial draw function, draws the background and intersections */
void draw_gui(vehicle_T* vehicles, intersection_T* intersections)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow(
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

    SDL_Surface* win_surface = SDL_GetWindowSurface(win);
    SDL_FillRect(win_surface, NULL, SDL_MapRGB(win_surface->format, COLOR_WHITE));
    SDL_UpdateWindowSurface(win);
    sleep(3);
}

/* update_gui: Updates the GUI with vehicle positions and intersection phase
 *             changes
 */
void update_gui(vehicle_T* vehicles, intersection_T* intersections)
{

}

void exit_gui()
{
    SDL_DestroyWindow(win);
}

void draw_vehicle(vehicle_T* vehicle)
{

}

void draw_intersection(intersection_T* intersection)
{

}