#include "prototypes.h"
#include <unistd.h>
#include <SDL2/SDL.h>

#define WINDOW_NAME "Traffic Simulator"
#define WINDOW_X    640
#define WINDOW_Y    480

#define COLOR_WHITE 255, 255, 255
#define COLOR_GRAY  150, 150, 150
#define COLOR_BLACK 0, 0, 0
#define COLOR_YELLOW 255, 255, 0

SDL_Window* win;

/* draw_gui: Initial draw function, draws the background and intersections */
void draw_gui(vehicle_T* vehicles, intersection_T* intersections)
{
    SDL_Init(SDL_INIT_EVERYTHING);
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

    SDL_Surface* win_surface = SDL_GetWindowSurface(win);
    SDL_FillRect(win_surface, NULL, SDL_MapRGB(win_surface->format, COLOR_WHITE));

    draw_intersection(&intersections[0]);
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
    if (!win) return;

    SDL_Surface* win_surface = SDL_GetWindowSurface(win);
    SDL_Rect rect, north, east, south, west; //intersection and roads
    SDL_Rect northDiv, eastDiv, southDiv, westDiv;

    //intersection
    rect.w = 50;
    rect.h = 50;
    rect.x = (WINDOW_X - rect.w) / 2;
    rect.y = (WINDOW_Y - rect.h) / 2;
    SDL_FillRect(win_surface, &rect, SDL_MapRGB(win_surface->format, COLOR_GRAY));

    //north
    north.w = 50;
    north.h = 215; 
    north.x = (WINDOW_X - rect.w) / 2;
    north.y = 0;
    SDL_FillRect(win_surface, &north, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    northDiv.w = 5;
    northDiv.h = 215;
    northDiv.x = ((WINDOW_X - rect.w) / 2) + 22.5;
    northDiv.y = 0;
    SDL_FillRect(win_surface, &northDiv, SDL_MapRGB(win_surface->format, COLOR_YELLOW));

    //east
    east.w = 295; 
    east.h = 50;
    east.x = ((WINDOW_X - rect.w) / 2) + 50;
    east.y = (WINDOW_Y - rect.h) / 2;
    SDL_FillRect(win_surface, &east, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    eastDiv.w = 295;
    eastDiv.h = 5;
    eastDiv.x = ((WINDOW_X - rect.w) / 2) + 50;
    eastDiv.y = ((WINDOW_Y - rect.h) / 2) + 20;
    SDL_FillRect(win_surface, &eastDiv, SDL_MapRGB(win_surface->format, COLOR_YELLOW));

    //south
    south.w = 50;
    south.h = 215;
    south.x = (WINDOW_X - rect.w) / 2;
    south.y = ((WINDOW_Y - rect.h) / 2) + 50;
    SDL_FillRect(win_surface, &south, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    southDiv.w = 5;
    southDiv.h = 215;
    southDiv.x = ((WINDOW_X - rect.w) / 2) + 22.5;
    southDiv.y = ((WINDOW_Y - rect.h) / 2) + 50;
    SDL_FillRect(win_surface, &southDiv, SDL_MapRGB(win_surface->format, COLOR_YELLOW));

    //west
    west.w = 295;
    west.h = 50;
    west.x = 0;
    west.y = (WINDOW_Y - rect.h) / 2;
    SDL_FillRect(win_surface, &west, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    westDiv.w = 295;
    westDiv.h = 5;
    westDiv.x = 0;
    westDiv.y = ((WINDOW_Y - rect.h) / 2) + 20;
    SDL_FillRect(win_surface, &westDiv, SDL_MapRGB(win_surface->format, COLOR_YELLOW));


    SDL_UpdateWindowSurface(win);
}