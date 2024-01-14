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
SDL_Texture* textureRetro = NULL;       //RetroBackground texture
SDL_Texture* textureSprite1 = NULL;     //Sprite1 texture
SDL_Texture* textureSprite2 = NULL;     //Sprite2 texture

//Rectangles
SDL_Rect rectRetro;                     //RetroBackground rectangle
SDL_Rect rectSprite1;                   //Sprite1 rectangle
SDL_Rect rectSprite2;                   //Sprite2 rectangle

//Function Prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(std::string path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
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
    "Mod 3 Lab 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //Load background image
    tempSurface = loadImage("graphics/RetroBackground.bmp");

    //Create texture
    textureRetro = loadTexture(tempSurface);

    //Create a rectangle at position 0, 0 for RetroBackground
    FillRect(rectRetro, 0, 0, 800, 600);

    //Load image1
    tempSurface = loadImage("graphics/sprite1.bmp");

    //Set red as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 0);

    //Create texture
    textureSprite1 = loadTexture(tempSurface);

    //Create a rectangle at position 0, 263 for Sprite1
    FillRect(rectSprite1, 0, 263, 75, 75);

    //Load image2
    tempSurface = loadImage("graphics/sprite2.bmp");

    //Set red as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 0);

    //Create texture
    textureSprite2 = loadTexture(tempSurface);

    //Create a rectangle at position 725, 263 for Sprite2
    FillRect(rectSprite2, 725, 263, 75, 75);

    //Here is the Game Loop!!!
    while(ProgramIsRunning())
    {
        //Clear the window
        SDL_RenderClear(renderer);

        //Load the background);
        SDL_RenderCopy(renderer, textureRetro, NULL, &rectRetro);

        //Load sprite1
        SDL_RenderCopy(renderer, textureSprite1, NULL, &rectSprite1);

        //Load sprite2
        SDL_RenderCopy(renderer, textureSprite2, NULL, &rectSprite2);

        //Display the game screen with updated position of Sprite1 and Sprite 2
        SDL_RenderPresent(renderer);
        SDL_Delay(10);  //delay a bit

        //Update x coordinate of Sprite1 position
        rectSprite1.x+=4;                   //increment Sprite1's x coordinate

        if(rectSprite1.x > 800 - 75)        //if Sprite1 touches the right side of the game screen
          rectSprite1.x = 0;                //put Sprite1 back to the left side

        //Update x coordinate of Sprite2 position
        rectSprite2.x-=3;                   //increment Sprite2's x coordinate

        if(rectSprite2.x < 0)              //if Sprite2 touches the left side of the game screen
          rectSprite2.x = 725;              //put Sprite2 back to the right side

    }//end game loop

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
    SDL_DestroyTexture(textureRetro);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit(); //Quit the program
}//End Close Shop


SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b)
{
    //create the transparent color (pink in this case) and put in colorKey
    Uint32 colorKey;
    colorKey = SDL_MapRGB(surface->format, r, g, b);
    //set the colorKey color as the transparent one in the image
    SDL_SetColorKey(surface, SDL_TRUE, colorKey);
    return surface;
}//End setTransparent Color
