#include <SDL.h>
#include <cstdio>
#include <cstdlib>  //contains random number functions
#include <ctime>    //use the computer's clock

//Window
SDL_Window* sdlGameWindow = NULL;

//Renderer
SDL_Renderer* sdlRenderer;

//Surfaces
SDL_Surface* tempSurface = NULL;

//Textures
SDL_Texture* textureSprite = NULL;      //Sprite texture

//Rectangles
SDL_Rect rectSprite;        //Sprite rectangle


//Function Prototypes
bool ProgramIsRunning();
bool LoadImages();
void FreeImages();

int main(int argc, char* args[])
{
    //Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL failed to initialize!\n");
        SDL_Quit();
        return 0;
    }
    //Create a window
    sdlGameWindow = SDL_CreateWindow(
    "Brandon LaPointe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    sdlRenderer = SDL_CreateRenderer(sdlGameWindow, -1, 0);

    //Load images
    LoadImages();

    //Create rectangles for the sprite texture
    rectSprite.x = 100; //x coord of upper left corner
    rectSprite.y = 100; //y coord of upper left corner
    rectSprite.w = 71; //width of sprite
    rectSprite.h = 55; //height of sprite

    //Seed the random number generator
     srand(static_cast<unsigned int>(time(0)));

    //Here is the Game Loop!!!
    while(ProgramIsRunning())
    {
        rectSprite.x = rand()%800;     //select an x coordinate between 0 and 800
        rectSprite.y = rand()%600;     //select a y coordinate between 0 and 600


        //create a white background
        SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
        //Clear the window
        SDL_RenderClear(sdlRenderer);
        //SDL_RenderCopy(sdlRenderer, textureBackground, NULL, &rectBackground);
        SDL_RenderCopy(sdlRenderer, textureSprite, NULL, &rectSprite);
        //put cleared window on screen
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000);  //delay a bit
    }
    //Debug messages to console screen
    printf("Use the console output for debugging and error checking.");

    //Free all objects
    SDL_DestroyTexture(textureSprite);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlGameWindow);
    SDL_Quit(); //Quit the program

    return 1;   //Because this is C++ and we have to return from the main function
}//end main

//Function Section
bool ProgramIsRunning()
{
    //keep running until window is x'ed out
    SDL_Event event;
    bool running = true;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;
    }
    return running;
}//end ProgramIsRunning

bool LoadImages()
{
    //Load the background image
    tempSurface = SDL_LoadBMP("graphics/zeppy.bmp");
    if(tempSurface == NULL)
    {
        printf("Zeppy image failed to load!\n");
        return 0;
    }
    //Convert surface to texture
    textureSprite = SDL_CreateTextureFromSurface(sdlRenderer, tempSurface);
    //Deleting the temporary surface
    SDL_FreeSurface(tempSurface);


    return true;
}//end LoadImages
