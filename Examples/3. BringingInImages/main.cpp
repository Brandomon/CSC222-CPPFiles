#include <SDL.h>
#include <cstdio>

//Create a window
SDL_Window* sdlWindow;

//Create a renderer to manipulate the window
SDL_Renderer* sdlRenderer;

//Create a surface to load the background image
SDL_Surface* tempSurface;

//Create a texture to convert surface to
SDL_Texture* texture;

int main(int argc, char* args[])
{

    //Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)

    {
        printf("SDL failed to initialize!");
        SDL_Quit();
        return 0;
    }

    //Create a window
    sdlWindow = SDL_CreateWindow(

    "Ithaca Falls", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);

    //Load the image
    tempSurface = SDL_LoadBMP("graphics/ithacaFalls.bmp");

    if(tempSurface == NULL)
    {
        printf("Ithaca falls image failed to load!\n");
        return 0;
    }

    //Convert surface to texture
    texture = SDL_CreateTextureFromSurface(sdlRenderer, tempSurface);

    //Deleting the temporary surface
    SDL_FreeSurface(tempSurface);

    //Clear the window
    SDL_RenderClear(sdlRenderer);

    //Copying the texture on to the window
    SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);

    //put cleared window on screen
    SDL_RenderPresent(sdlRenderer);

    //Debug messages to console screen
    printf("Use the console output for debugging and error checking.");

    SDL_Delay(6000);  //delay a bit

    //Free all objects
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);

    SDL_Quit(); //Quit the program
    return 1;   //Because this is C++ and we have to return from the main function
}
