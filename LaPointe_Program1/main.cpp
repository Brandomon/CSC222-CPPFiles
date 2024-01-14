#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

//Function Prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int bitMapWidth, int bitMapHeight);
SDL_Surface* loadImage(std::string path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();
void MoveBitMap(int& xPosition, int& yPosition, int width, int height, int& xDirection, int& yDirection);

/*******************************************************************
* Program Name: Bouncing Off The Walls
*
* Description: This program will allow a bitmap to move around the
*   game screen on its own, bouncing off the boundaries of the game
*   screen once reaching them.
*
* PseudoCode:
* Create window
* Create renderer to manipulate the window
* Create surface to load the images
* Create textures to convert surfaces to
* Create rectangles to display textures
* Declare Sprite Constants
*   Initialize const int MaxSpriteFrame to 4
*   Initialize const int FrameDelay to 5
* Declare Sprite Variables
*   Initialize int SpriteFrame to 0
*   Initialize int FrameCounter to 0
*   Initialize int xPos to 0
*   Initialize int yPos t 0
*   Declare bitMapHeight as int
*   Declare bitMapWidth as int
*   Initialize int xDir to 1
*   Initialize int yDir to 1
* Initialize SDL
* Create the game window
* Create a renderer for the window
* Set up Background
*   Load image
*   Create texture
*   Create a rectangle at position 0, 0 for Background
* Set up Spartan
*   Create the surface - load the whole sheet
*   Create the rectangle to hold one Spartan
*   Get the width of one frame (one Spartan) by dividing the width of the sheet by the number of frames
*   Get the bitMapHeight of one frame
*   Initialize the rectangle for one Spartan
*   Create the rectangle on game screen for Spartan placement
* Game Loop
*   Increment time to delay each frame
*   IF (delay time is greater than max amount)
*       Start new delay time
*       Advance to next frame
*   END IF
*   When sprite reaches last frame start over
*   IF (last frame is reached)
*       Start at frame zero again
*   END IF
*   Advance to next frame on sheet
*   Update Spartan Movement
*   Render the scene
*   Clear the window
*   Load the background
*   Load the spartan
*   Display the game screen with updated position of the background
* End main
*******************************************************************/

//Global Everything
//Create window
SDL_Window* gameWindow = NULL;

//Create renderer to manipulate the window
SDL_Renderer* renderer;

//Create surface to load the images
SDL_Surface* tempSurface;

//Create textures to convert surfaces to
SDL_Texture* textureBackground = NULL;      //Background texture
SDL_Texture* textureSpartanSheet = NULL;    //sheet of spartans texture
SDL_Texture* textureSpartan = NULL;         //one spartan texture

//Create rectangles to display textures
SDL_Rect rectBackground;        //Background rectangle
SDL_Rect rectSpartanPlacement;  //Where to place spartan on game screen
SDL_Rect rectSpartanSheet;      //Rectangle for sheet of spartans
SDL_Rect rectSpartan;           //Rectangle for one spartan

//Sprite constants
const int MaxSpriteFrame = 4;   //Number of frames
const int FrameDelay = 5;       //Frame delay

//Sprite variables
int SpriteFrame = 0;            //Indicator of current sprite frame
int FrameCounter = 0;           //Frame counter for sprite sheet
int xPos = 0;                   //Current x position of the upper left corner of the bitmap
int yPos = 0;                   //Current y position of the upper left corner of the bitmap
int bitMapHeight;               //Height of one frame
int bitMapWidth;                //Width of one frame
int xDir = 1;                   //X direction value
int yDir = 1;                   //Y direction value

int main(int argc, char* args[])
{
    //Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL!\n");
        return 0;
    }

    //Create the game window
    gameWindow = SDL_CreateWindow(
    "Bouncing Off The Walls", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //Set up Background
    //Load image
    tempSurface = loadImage("graphics/GameOverBackground.bmp");

    //Create texture
    textureBackground = loadTexture(tempSurface);

    //Create a rectangle at position 0, 0 for Background
    FillRect(rectBackground, 0, 0, 800, 600);

    //Set up Spartan
    //create the surface - load the whole sheet
    tempSurface = loadImage("graphics/Spartan.bmp");

    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);

    //Create the texture for the whole sheet
    textureSpartanSheet = loadTexture(tempSurface);

    //Create the rectangle to hold one Spartan
    //SDL_QueryTexture() method gets the width and bitMapHeight of the sheet of Spartan
    SDL_QueryTexture(textureSpartanSheet, NULL, NULL, &rectSpartanSheet.w, &rectSpartanSheet.h);

    //Get the width of one frame (one Spartan) by dividing the width of the sheet by the number of frames
    bitMapWidth = rectSpartanSheet.w/MaxSpriteFrame;

    //Get the bitMapHeight of one frame
    bitMapHeight = rectSpartanSheet.h;  //bitMapHeight on one spartan is same as bitMapHeight of sheet

    //Initialize the rectangle for one Spartan
    FillRect(rectSpartan, 0, 0, bitMapWidth, bitMapHeight);

    //Create the rectangle on game screen for Spartan placement
    FillRect(rectSpartanPlacement, 350, 250, bitMapWidth, bitMapHeight);

    //Game Loop
    while(ProgramIsRunning())
    {
        //Increment time to delay each frame
        FrameCounter++;

        //If delay time is greater than max amount
        if(FrameCounter > FrameDelay)
        {
            FrameCounter = 0;   //Start new delay time
            SpriteFrame++;      //Advance to next frame
        }

        //If sprite reaches last frame start over
        if(SpriteFrame > MaxSpriteFrame - 1) //If last frame is reached
            SpriteFrame = 0;                 //Start at frame zero again

        //Advance to next frame on sheet
        rectSpartan.x = SpriteFrame * rectSpartan.w;

        //Update Spartan Movement
        rectSpartanPlacement.x = xPos;                      //get the current x position
        rectSpartanPlacement.y = yPos;                      //get the current y position
        MoveBitMap( xPos, yPos, bitMapWidth, bitMapHeight, xDir, yDir); //move the bitmap
        rectSpartanPlacement.x = xPos;                      //update x with new position
        rectSpartanPlacement.y = yPos;                      //update y with new position

        //Render the scene
        //Clear the window
        SDL_RenderClear(renderer);

        //Load the background
        SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);

        //Load the Spartan
        SDL_RenderCopy(renderer, textureSpartanSheet, &rectSpartan, &rectSpartanPlacement);

        //Display the game screen with updated position of the background
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
    //Texture
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

void FillRect(SDL_Rect &rect, int x, int y, int bitMapWidth, int bitMapHeight)
{
    //Initialize the rectangle
    rect.x = x;             //Initial x position of upper left corner
    rect.y = y;             //Initial y position of upper left corner
    rect.w = bitMapWidth;   //Width of rectangle
    rect.h = bitMapHeight;  //Height of rectangle
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
    //Create the transparent color and put in colorKey
    Uint32 colorKey;
    colorKey = SDL_MapRGB(surface->format, r, g, b);
    //Set the colorKey color as the transparent one in the image
    SDL_SetColorKey(surface, SDL_TRUE, colorKey);
    return surface;
}//End setTransparent Color

/*******************************************************************
* Function Name: MoveBitMap
*
* Description: Moves the bitmap, so the x and y positions will be
*   changed and the x and y direction values will be changed once
*   the bitmap reaches the borders of the game screen in order to
*   bounce off each border.
*
* PseudoCode:
* Declare Variables
*   Declare & Initialize int x to xPosition
*   Declare & Initialize int y to yPosition
*   Declare & Initialize int dirX to xDirection
*   Declare & Initialize int dirY to yDirection
* Calculate new x position with x direction value
* Determine if the bitmap is going beyond the right border
*   IF (bitmap goes right beyond border)
*       Calculate new x position with x direction value
*   END IF
* Determine if the bitmap is going beyond the left border
*   IF (bitmap goes beyond left border)
*       Calculate new x position with x direction value
*   END IF
* Calculate new y position with y direction value
* Determine if the bitmap is going beyond the bottom border
*   IF (bitmap goes beyond bottom border)
*       Calculate new y position with y direction value
*   END IF
* Determine if the bitmap is going beyond the top border
*   IF (bitmap goes beyond top border)
*       Calculate new y position with y direction value
*   END IF
* Update the x and y positions to the new positions and directions
*   xPosition = x
*   yPosition = y
*   xDirection = dirX
*   yDirection = dirY
*******************************************************************/
void MoveBitMap(int& xPosition, int& yPosition, int width, int height, int& xDirection, int& yDirection)
{
    //Local Variables
    int x = xPosition;
    int y = yPosition;
    int dirX = xDirection;
    int dirY = yDirection;

    //Calculate new x position with x direction value
    x = x + dirX;

    //Determine if the bitmap is going beyond the right border
    if (x >= 800 - width)
    {
        //Calculate new x position with x direction value
        x = 800 - width - 5;
        dirX = -1;
    }//End if

    //Determine if the bitmap is going beyond the left border
    if (x <= 0)
    {
        //Calculate new x position with x direction value
        x = 5;
        dirX = 1;
    }//End if

    //Calculate new y position with y direction value
    y = y + dirY;

    //Determine if the bitmap is going beyond the bottom border
    if (y >= 600 - height)
    {
        //Calculate new y position with y direction value
        y = 600 - height - 5;
        dirY = -1;
    }//End if

    //Determine if the bitmap is going beyond the top border
    if (y <= 0)
    {
        //Calculate new y position with y direction value
        y = 5;
        dirY = 1;
    }//End if

    //Update the x and y positions to the new positions and directions
    xPosition = x;
    yPosition = y;
    xDirection = dirX;
    yDirection = dirY;
}
