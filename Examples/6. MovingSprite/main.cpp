#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

//Global Everything
//Window
SDL_Window* gameWindow = NULL;
//Renderer
SDL_Renderer* renderer;
//Surfaces
SDL_Surface* tempSurface;
//Textures
SDL_Texture* textureZeppy = NULL;      //Sprite texture
//Rectangles
SDL_Rect rectZeppy;        //Sprite rectangle

//Function Prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(std::string path);
SDL_Texture* loadTexture(SDL_Surface* surface);
void CloseShop();

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
    gameWindow = SDL_CreateWindow(
    "Moving a Sprite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //Load image
    tempSurface = loadImage("graphics/zeppy.bmp");

    //Convert to texture
    textureZeppy = loadTexture(tempSurface);

    // Create a 71 x 55 rectangle at position 50, 50 for Zeppy
    FillRect(rectZeppy, 50, 50, 71, 55);

    //Here is the Game Loop!!!
    while(ProgramIsRunning())
    {
        //create a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        //Clear the window
        SDL_RenderClear(renderer);

        //Draw Zeppy);
        SDL_RenderCopy(renderer, textureZeppy, NULL, &rectZeppy);

        //display the game screen with updated position of Zeppy
        SDL_RenderPresent(renderer);

        SDL_Delay(10);  //delay a bit

        //Update x coordinate of Zeppy's position
        rectZeppy.x+=5;                 //increment Zeppy's x coordinate

        if(rectZeppy.x > 800 - 50)      //if Zeppy touches the right side of the game screen
          rectZeppy.x = 0;              //put Zeppy back to the left side

    }//end WHILE game loop

    //Debug messages to console screen
    printf("Use the console output for debugging and error checking.");
    CloseShop();
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

SDL_Surface* loadImage(std::string path)
{
    //Load image at specified path
    tempSurface = SDL_LoadBMP(path.c_str());

    if( tempSurface == NULL )
    {
        printf("Unable to load image at path: %s\n", path.c_str());
    }
    return tempSurface;
}//end loadImage


SDL_Texture* loadTexture(SDL_Surface* tempSurface)
{
    //texture
    SDL_Texture* newTexture = NULL;
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    if( newTexture == NULL )
    {
        printf("Unable to create texture");
    }
    //Get rid of the surface
    SDL_FreeSurface( tempSurface );

    return newTexture;
}//end create a texture

void FillRect(SDL_Rect &rect, int x, int y, int width, int height)
{
    //Initialize the rectangle
    rect.x = x;         //initial x position of upper left corner
    rect.y = y;         //initial y position of upper left corner
    rect.w = width;     //width of rectangle
    rect.h = height;    //height of rectangle
}//end rectangle initializing

void CloseShop()
{
    //Destroy all objects
    SDL_DestroyTexture(textureZeppy);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit(); //Quit the program
}//end close shop
