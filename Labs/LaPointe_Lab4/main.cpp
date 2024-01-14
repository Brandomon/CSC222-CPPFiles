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
SDL_Texture* textureBackground = NULL;      //Background texture
SDL_Texture* textureBatSheet = NULL;      //sheet of demons texture
SDL_Texture* textureBat = NULL;           //one demon texture

//Rectangles
SDL_Rect rectBackground;        //Background rectangle
SDL_Rect rectBatPlacement;    //Where to place demon on game screen
SDL_Rect rectBatSheet;        //Rectangle for sheet of demons
SDL_Rect rectBat;             //Rectangle for one demon

//Sprite variables
int SpriteFrame = 0;
int FrameCounter = 0;
int batLocationX = 0;
int width;                      //width of one frame
int height;                     //height of one frame
const int MaxSpriteFrame = 12;  //number of frames
const int FrameDelay = 2;

//Function Prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(std::string path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL!\n");
        return 0;
    }

    //Create the game window
    gameWindow = SDL_CreateWindow(
    "Module 3 Lab 4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //Set up Background
    //Load image
    tempSurface = loadImage("graphics/backGrid.bmp");

    //Create texture
    textureBackground = loadTexture(tempSurface);

    // Create a rectangle at position 0, 0 for Background
    FillRect(rectBackground, 0, 0, 800, 600);

    //Set up Bat

    //create the surface - load the whole sheet
    tempSurface = loadImage("graphics/horzBats.bmp");

    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);

    //create the texture for the whole sheet
    textureBatSheet = loadTexture(tempSurface);

    //Create the rectangle to hold one bat

    //SDL_QueryTexture() method gets the width and height of the sheet of Bat
    SDL_QueryTexture(textureBatSheet, NULL, NULL, &rectBatSheet.w, &rectBatSheet.h);

    //get the width of one frame (one bat) by dividing the width of the sheet by the number of frames
    width = rectBatSheet.w / MaxSpriteFrame;

    //get the height of one frame
    height = rectBatSheet.h;  //height on one demon is same as height of sheet

    //Initialize the rectangle for one bat
    FillRect(rectBat, 0, 0, width, height);

    //Create the rectangle on game screen for bat placement
    FillRect(rectBatPlacement, 350, 250, width, height);

    while(ProgramIsRunning())
    {
        //Increment time to delay each frame
        FrameCounter++;

        //If delay time is greater than max amount
        if(FrameCounter > FrameDelay)
        {
            FrameCounter = 0;   //start new delay time
            SpriteFrame++;      //advance to next frame
        }

        //If sprite reaches last frame start over
        if(SpriteFrame > MaxSpriteFrame - 1) //if last frame is reached
            SpriteFrame = 0;                 //start at frame zero again

        //Advance to next frame on sheet
        rectBat.x = SpriteFrame * rectBat.w;

        //Update Background scrolling
        batLocationX+=5;
        if(batLocationX > 800 - 32)
            batLocationX = 0;

        rectBatPlacement.x = batLocationX;

        //Render the scene
        //Clear the window
        SDL_RenderClear(renderer);

        //Load the background)
        SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);

        //Load the Bat
        SDL_RenderCopy(renderer, textureBatSheet, &rectBat, &rectBatPlacement);

        //display the game screen with updated position of the background
        SDL_RenderPresent(renderer);
        SDL_Delay(20);

    }//end main
    CloseShop();
    return 0;
}

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

bool ProgramIsRunning()
{
    SDL_Event event;

    bool running = true;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;
    }

    return running;
}

void CloseShop()
{
    //Destroy all objects
    SDL_DestroyTexture(textureBackground);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit(); //Quit the program
}//End Close Shop

SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b)
{
    //create the transparent color and put in colorKey
    Uint32 colorKey;
    colorKey = SDL_MapRGB(surface->format, r, g, b);
    //set the colorKey color as the transparent one in the image
    SDL_SetColorKey(surface, SDL_TRUE, colorKey);
    return surface;
}//End setTransparent Color
