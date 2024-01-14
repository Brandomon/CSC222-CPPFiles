#include <SDL.h>         //allows access to SDL files
#include <cstdio>        //allows access to c command printf

//Create a window
SDL_Window *sdlWindow;   //create a window

//Create a renderer to manipulate the window
SDL_Renderer *sdlRenderer;

int main(int argc, char* args[])
{
    //Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)      //this initializes all SDL components
    {
        printf("SDL failed to initialize!");  //print out message if initialize fails
        SDL_Quit();
        return 0;
    }
     //create the window, set title, position it, size it
     sdlWindow = SDL_CreateWindow(
     "Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN);

     //Create a renderer for the window
     sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
     //Create a background color (r, b, g, alpha)
     SDL_SetRenderDrawColor(sdlRenderer, 222, 111, 0, 255);

     //Clear the window
     SDL_RenderClear(sdlRenderer);

     //Put cleared window on screen
     SDL_RenderPresent(sdlRenderer);

     //Demonstrate print to console window
     printf("Use the console output for debugging and error checking.");

     SDL_Delay(5000);  //delay for a bit

     //Free all objects
     SDL_DestroyRenderer(sdlRenderer);
     SDL_DestroyWindow(sdlWindow);

     SDL_Quit();       //quit

     return 1;
}

