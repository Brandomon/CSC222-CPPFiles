#include <SDL.h>
#include <cstdio>
#include <cstdlib>  //contains random number functions
#include <ctime>    //use the computer's clock

//Window
SDL_Window* sdlGameWindow = NULL;

//Renderer
SDL_Renderer* sdlRenderer;

//Surfaces
SDL_Surface* tempSurface1 = NULL;
SDL_Surface* tempSurface2 = NULL;

//Textures
SDL_Texture* textureSprite1 = NULL;     //Sprite1 texture
SDL_Texture* textureSprite2 = NULL;     //Sprite2 texture

//Rectangles
SDL_Rect rectSprite1;                   //Sprite1 rectangle
SDL_Rect rectSprite2;                   //Sprite2 rectangle


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

    //Create rectangles for the sprite textures
    //Sprite1 square
    rectSprite1.x = 100; //x coord of upper left corner
    rectSprite1.y = 100; //y coord of upper left corner
    rectSprite1.w = 75; //width of sprite
    rectSprite1.h = 75; //height of sprite
    //Sprite2 square
    rectSprite2.x = 100; //x coord of upper left corner
    rectSprite2.y = 100; //y coord of upper left corner
    rectSprite2.w = 75; //width of sprite
    rectSprite2.h = 75; //height of sprite

    //Seed the random number generator
     srand(static_cast<unsigned int>(time(0)));

    //Game Loop
    while(ProgramIsRunning())
    {
        //Place sprites in random positions
        rectSprite1.x = rand()%800;     //select an x coordinate between 0 and 800
        rectSprite1.y = rand()%600;     //select a y coordinate between 0 and 600

        rectSprite2.x = rand()%800;     //select an x coordinate between 0 and 800
        rectSprite2.y = rand()%600;     //select a y coordinate between 0 and 600


        //create a white background
        SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

        //Clear the window
        SDL_RenderClear(sdlRenderer);

        //SDL_RenderCopy(sdlRenderer, textureBackground, NULL, &rectBackground);
        SDL_RenderCopy(sdlRenderer, textureSprite1, NULL, &rectSprite1);
        SDL_RenderCopy(sdlRenderer, textureSprite2, NULL, &rectSprite2);

        //put cleared window on screen
        SDL_RenderPresent(sdlRenderer);

        SDL_Delay(1000);  //delay a bit
    }

    //Debug messages to console screen
    printf("Use the console output for debugging and error checking.");

    //Free all objects
    SDL_DestroyTexture(textureSprite1);
    SDL_DestroyTexture(textureSprite2);
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
    tempSurface1 = SDL_LoadBMP("graphics/sprite1.bmp");
    if(tempSurface1 == NULL)
    {
        printf("Sprite1 image failed to load!\n");
        return 0;
    }
    tempSurface2 = SDL_LoadBMP("graphics/sprite2.bmp");
    if(tempSurface2 == NULL)
    {
        printf("Sprite2 image failed to load!\n");
        return 0;
    }
    //Convert surface to texture
    textureSprite1 = SDL_CreateTextureFromSurface(sdlRenderer, tempSurface1);
    textureSprite2 = SDL_CreateTextureFromSurface(sdlRenderer, tempSurface2);

    //Deleting the temporary surface
    SDL_FreeSurface(tempSurface1);
    SDL_FreeSurface(tempSurface2);

    return true;

}//end LoadImages
