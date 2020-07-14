#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 450;

void handleError(void* thing) {
    if (thing == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        printf("Error: %s", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
}

SDL_Texture* loadTexture(char* file, SDL_Renderer* ren) {
    SDL_Texture *texture = NULL;
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
    SDL_Surface *loadedImage = SDL_LoadBMP(file);
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
    handleError(loadedImage);
    texture = SDL_CreateTextureFromSurface(ren, loadedImage);
    SDL_FreeSurface(loadedImage);
    handleError(texture);
    return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, double xPercent, double yPercent, unsigned int scaling){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	unsigned int x = WIDTH * xPercent;
	unsigned int y = HEIGHT * yPercent;
	dst.x = x;
	dst.y = y;
	printf("X: %d, Y: %d", x, y);
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	dst.w *= scaling;
	dst.h *= scaling;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void onUpdate() {
    SDL_Texture* foreground = loadTexture("assets/test.bmp", renderer);
    SDL_Texture* background = loadTexture("assets/background.bmp", renderer);

    renderTexture(background, renderer, 0, 0, 50);
    renderTexture(foreground, renderer, 50, 50, 2);

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    SDL_DestroyTexture(foreground);
    SDL_DestroyTexture(background);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("STD_Init Error: %s", SDL_GetError());
        return 1;
    }
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
    
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(onUpdate, 0, 1);
    #else
    SDL_Event e;
    bool quit = false;
    while(!quit) {        
        onUpdate();
        while (SDL_PollEvent(&e)){
        //If user closes the window
        if (e.type == SDL_QUIT){
            quit = true;
        }
        //If user presses any key
        if (e.type == SDL_KEYDOWN){
        }
        //If user clicks the mouse
        if (e.type == SDL_MOUSEBUTTONDOWN){
        }
    }
	SDL_Delay(16);
    }
    #endif 
}
