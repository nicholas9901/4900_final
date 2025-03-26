#include "prototypes.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_NAME "Traffic Simulator"
#define WINDOW_X    1200
#define WINDOW_Y    1200

#define COLOR_WHITE  255, 255, 255
#define COLOR_GRAY   150, 150, 150
#define COLOR_BLACK  0, 0, 0
#define COLOR_YELLOW 255, 255, 0

/* Drawing definitions */
#define DIV_WIDTH 4 /* Lane division */
#define DIV_OFFSET ((INTERSECTION_SIZE / 2) - DIV_WIDTH + 1)

/* Asset-related definitions */
#define PATH_SIZE 256

#define PATH_BASE   "4900_final"
#define PATH_ASSETS "assets"
#define PATH_IMG_CAR PATH_ASSETS"/car.png"

extern char run_path[PATH_SIZE];

void init_gui();
void update_gui(vehicle_T*, intersection_T*);
void exit_gui();
void draw_vehicle(vehicle_T*);
void draw_intersection(intersection_T*);
void move_horizontal(SDL_Texture*, SDL_Rect);
void move_vertical(SDL_Texture*, SDL_Rect);
void right_turn(SDL_Texture*, SDL_Rect);

void poll_for_exit();
void trim_run_path();
void initialize_assets();
