#include <SDL2/SDL.h>
#include <stdio.h>

#include "machine.h"

/* 
   Emulator "shell" that wraps the complete machine emulation with
   a window to display video output and pass through keyboard input.
*/

// height and width of the native space invaders screen
const int sinv_height = 256;
const int sinv_width = 224;

//height and width of the canvas to draw on
//(larger than native res because modern pixel density is way more)
const int window_height = 3 * sinv_height;
const int window_width = 3 * sinv_width;

int main(int argc, char **argv) {
    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL init failed with error %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "SDL could not create a window - error %s\n", SDL_GetError);
    }

    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, NULL,
        SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0x00));

    SDL_UpdateWindowSurface(window);

    SDL_Delay(2000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}