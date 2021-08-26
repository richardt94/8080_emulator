#include <SDL.h>
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

//write a 1bpp framebuffer to the screen
static void showBuffer(const byte *buffer, SDL_Surface *surf);
static void setPixel(SDL_Surface *surf, int x, int y, int val);

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <space invaders ROM files>\n", argv[0]);
        return 1;
    }

    SDL_Window *window = NULL;
    SDL_Surface *screenSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL init failed with error %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "SDL could not create a window - error %s\n", SDL_GetError());
    }

    screenSurface = SDL_GetWindowSurface(window);

    int memctr = 0;
    Machine *m = newMachine();
    for (int fidx = 1; fidx < argc; fidx++) {
        FILE *bin_file = fopen(argv[fidx], "rb");

        if (bin_file == NULL) {
            fprintf(stderr, "Failed to open ROM file!\n");
            exit(1);
        }

        fprintf(stderr, "Reading file %s into memory at 0x%04x\n",
            argv[fidx], memctr);
        //size of file in bytes
        fseek(bin_file, 0L, SEEK_END);
        int fsize = ftell(bin_file);
        fseek(bin_file, 0L, SEEK_SET);

        if (fsize > m->cs->mem_size - memctr) {
            printf("ROM file size too big!\n");
            return 1;
        }
        fread(m->cs->memory + memctr, sizeof(byte), fsize, bin_file);
        fclose(bin_file);
        memctr += fsize;
        fprintf(stderr, "File ends at 0x%04x\n",
            memctr - 1);
    }

    int quit = 0;
    SDL_Event e;

    unsigned int frames = 0;

    while (!quit) {
        stepFrame(m);
        showBuffer(m->framebuffer, screenSurface);
        SDL_UpdateWindowSurface(window);

        frames++;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                {
                    quit = 1;
                    uint32_t time = SDL_GetTicks();
                    double fps = 1000 * (double) frames / (double) time;
                    time /= 1000;
                    printf("Rendered %d frames in %d seconds = %.2f fps\n", frames, time, fps);
                }
        }

        uint32_t currentTime = SDL_GetTicks();
        //60 fps - each frame should take
        //approx. 16667 microseconds
        //modular multiplication avoids overflow
        unsigned int waitTime =
          (16667 - ((currentTime % 16667 * 1000) % 16667)) / 1000;
        SDL_Delay(waitTime);

    }

    destroyMachine(m);
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

static void showBuffer(const uint8_t *buffer, SDL_Surface *surf) {
    //write each pixel as a 3*3 block on the surface
    for (int sinv_px_ind = 0;
       sinv_px_ind < sinv_height * sinv_width;
       sinv_px_ind++) {
        int px_byte = sinv_px_ind / 8;
        int px_bit = sinv_px_ind % 8;
        int px = (buffer[px_byte] >> (px_bit)) & 0x01;
        //x, y measured from top left
        int px_x = sinv_px_ind / 256;
        int px_y = 255 - (sinv_px_ind % 256);
        for (int xoff = 0; xoff < 3; xoff++) 
            for (int yoff = 0; yoff < 3; yoff++)
                setPixel(surf, 3 * px_x + xoff, 3 * px_y + yoff, px);
    }
}

static void setPixel(SDL_Surface *surf, int x, int y, int val) {
    //this assumes 32 bpp
    uint8_t *pixel = surf->pixels;
    pixel += (y * surf->pitch) + (x * sizeof(uint32_t));
    *((uint32_t *) pixel) = 0xffffffff * val;
}
