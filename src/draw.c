#include "prototypes.h"
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_NAME "Traffic Simulator"
#define WINDOW_X    640
#define WINDOW_Y    480

#define COLOR_WHITE 255, 255, 255
#define COLOR_GRAY  150, 150, 150
#define COLOR_BLACK 0, 0, 0
#define COLOR_YELLOW 255, 255, 0

void move_horizontal(SDL_Texture*, SDL_Rect);
void move_vertical(SDL_Texture*, SDL_Rect);
void right_turn(SDL_Texture*, SDL_Rect);

SDL_Window* win;
SDL_Renderer* renderer; //apply transformations to images

/* draw_gui: Initial draw function, draws the background and intersections */
void draw_gui(vehicle_T* vehicles, intersection_T* intersections)
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

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);

    if (!renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Surface* win_surface = SDL_GetWindowSurface(win);
    SDL_FillRect(win_surface, NULL, SDL_MapRGB(win_surface->format, COLOR_WHITE));

    draw_intersection(&intersections[0]);
    draw_vehicle(&vehicles[0]);
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
    if (!win) return;

    SDL_Surface* win_surface = SDL_GetWindowSurface(win);
    SDL_Texture* icon = IMG_LoadTexture(renderer, "../assets/car.png");
    if (!icon) {
        printf("Failed to load vehicle icon \n");
    }

    //get attributes of texture
    int icon_w, icon_h;
    SDL_QueryTexture(icon, NULL, NULL, &icon_w, &icon_h);

    //vehicle posiitons for testing
    
    //west --> east
    //SDL_Rect icon_pos = { 100, 235, icon_w, icon_h };
    //SDL_RenderCopyEx(renderer, icon, NULL, &icon_pos, 90.0, NULL, SDL_FLIP_NONE);

    //east --> west
    //SDL_Rect icon_pos = { 550, 210, icon_w, icon_h };
    //SDL_RenderCopyEx(renderer, icon, NULL, &icon_pos, 90.0, NULL, SDL_FLIP_VERTICAL);

    //north --> south
    //SDL_Rect icon_pos = { 290, 80, icon_w, icon_h };
    //SDL_RenderCopyEx(renderer, icon, NULL, &icon_pos, 180.0, NULL, SDL_FLIP_NONE);

    //south --> north
    SDL_Rect icon_pos = { 318, 400, icon_w, icon_h };
    SDL_RenderCopyEx(renderer, icon, NULL, &icon_pos, 0.0, NULL, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
    //move_horizontal(icon, icon_pos);
    move_vertical(icon, icon_pos);
}

void draw_intersection(intersection_T* intersection)
{
    if (!win) return;

    SDL_Surface* win_surface = SDL_GetWindowSurface(win);

    //intersection
    SDL_Rect rect = { .w = 50, .h = 50 };
    rect.x = (WINDOW_X - rect.w) / 2;
    rect.y = (WINDOW_Y - rect.h) / 2;
    SDL_FillRect(win_surface, &rect, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    //north
    SDL_Rect north = { (WINDOW_X - rect.w) / 2, 0, 50, 215 };
    SDL_FillRect(win_surface, &north, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    SDL_Rect northDiv = { ((WINDOW_X - rect.w) / 2) + 22.5, 0, 5, 215 };
    SDL_FillRect(win_surface, &northDiv, SDL_MapRGB(win_surface->format, COLOR_YELLOW));

    //east
    SDL_Rect east = { ((WINDOW_X - rect.w) / 2) + 50, (WINDOW_Y - rect.h) / 2, 295, 50 };
    SDL_FillRect(win_surface, &east, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    SDL_Rect eastDiv = { ((WINDOW_X - rect.w) / 2) + 50, ((WINDOW_Y - rect.h) / 2) + 20, 295, 5 };
    SDL_FillRect(win_surface, &eastDiv, SDL_MapRGB(win_surface->format, COLOR_YELLOW));

    //south
    SDL_Rect south = { (WINDOW_X - rect.w) / 2, ((WINDOW_Y - rect.h) / 2) + 50, 50, 215 };
    SDL_FillRect(win_surface, &south, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    SDL_Rect southDiv = { ((WINDOW_X - rect.w) / 2) + 22.5, ((WINDOW_Y - rect.h) / 2) + 50, 5, 210 };
    SDL_FillRect(win_surface, &southDiv, SDL_MapRGB(win_surface->format, COLOR_YELLOW));

    //west
    SDL_Rect west = { 0, (WINDOW_Y - rect.h) / 2, 295, 50 };
    SDL_FillRect(win_surface, &west, SDL_MapRGB(win_surface->format, COLOR_BLACK));

    SDL_Rect westDiv = { 0, ((WINDOW_Y - rect.h) / 2) + 20, 295, 5 };
    SDL_FillRect(win_surface, &westDiv, SDL_MapRGB(win_surface->format, COLOR_YELLOW));

    SDL_UpdateWindowSurface(win);
}