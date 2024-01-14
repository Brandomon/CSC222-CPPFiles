#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

/*******************************************************************
* Program Name: Adding Boundaries to the BufferedKey Example
*
* Description: This program will allow a bitmap to move around the
*   game screen when controlled by the user with the arrow keys, stopping
*   once the bitmap encounters a boundary.
*
* PseudoCode:
* Initialize SDL
* Create window
* Create renderer to manipulate the window
* Load background
* Create textures to convert surfaces to
* Create a rectangle at position 0, 0 for background
* Load spaceShip bitmap
* Set magenta as transparency color
* Create texture
* Create a rectangle at position 0, 0 for spaceShip
* WHILE Program Is Running
*   Get the key press from the keyboard
*   IF the escape key was pressed
*       CloseShop
*       Return 1
*   ENDIF
*   IF the left arrow was pressed
*       Move the bitmap left
*   ENDIF
*   IF the right arrow was pressed
*       Move the bitmap right
*   ENDIF
*   IF the up arrow was pressed
*       Move the bitmap up
*   ENDIF
*   IF the down arrow was pressed
*       Move the bitmap down
*   ENDIF
*   Clear the window
*   Copy the background
*   Call function to check borders
*   Copy SpaceShip
*   Display the game screen with updated position of spaceShip
* ENDWHILE
* End main
*******************************************************************/

//Global Everything
//Window
SDL_Window* gameWindow = NULL;

//Renderer
SDL_Renderer* renderer;

//Surfaces
SDL_Surface* tempSurface;

//Textures
SDL_Texture* textureBackground = NULL;
SDL_Texture* textureSpaceShip = NULL;

//Rectangles
SDL_Rect rectSpaceShip;     //SpaceShip rectangle
SDL_Rect rectBackground;    //Background rectangle

//Function Prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();
void CheckBorder(int& x, int& y, int w, int h);

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
    gameWindow = SDL_CreateWindow("Use the arrow keys to move the sprite around and ESC to exit.", SDL_WINDOWPOS_UNDEFINED,          SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //Load background
    tempSurface = loadImage("graphics/background.bmp");

    //Create texture
    textureBackground = loadTexture(tempSurface);

    //Create a rectangle at position 0, 0 for background
    FillRect(rectBackground, 0, 0, 800, 600);

     //Load spaceShip bitmap
    tempSurface = loadImage("graphics/spaceship.bmp");

    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);

    //Create texture
    textureSpaceShip = loadTexture(tempSurface);

    // Create a rectangle at position 0, 0 for spaceShip
    FillRect(rectSpaceShip, 0, 0, 120, 95);

    while(ProgramIsRunning())
    {
        //Get the key press from the keyboard
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        //if the escape key was pressed
        if(keys[SDL_SCANCODE_ESCAPE])
        {
            CloseShop();
            return 1;
        }

        //if the left arrow was pressed then move to the left
        if(keys[SDL_SCANCODE_LEFT])
            rectSpaceShip.x-=8;

        //if the right arrow was pressed then move to the right
        if(keys[SDL_SCANCODE_RIGHT])
            rectSpaceShip.x+=8;

        //if the up arrow was pressed then move up
        if(keys[SDL_SCANCODE_UP])
            rectSpaceShip.y-=8;

        //if the down arrow was pressed then move down
        if(keys[SDL_SCANCODE_DOWN])
            rectSpaceShip.y+=8;

        //Clear the window
        SDL_RenderClear(renderer);
        //Copy the background
        SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);
        //Call function to check borders
        CheckBorder(rectSpaceShip.x, rectSpaceShip.y, 120, 95);
        //Copy SpaceShip
        SDL_RenderCopy(renderer, textureSpaceShip, NULL, &rectSpaceShip);
        //Display the game screen with updated position of spaceShip
        SDL_RenderPresent(renderer);
        SDL_Delay(10);  //delay a bit
    }//end game loop
    printf("You will never be able to make everyone happy.  You are not a Nutella jar.");
    CloseShop();
    return 1;  //Because it's C
}//end main

//Function Section
SDL_Surface* loadImage(const char* path)
{
    //Load image at specified path
    tempSurface = SDL_LoadBMP(path);

    if( tempSurface == NULL )
    {
        printf("Unable to load image at path: %s\n", path);
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
    SDL_DestroyTexture(textureBackground);
    SDL_DestroyTexture(textureSpaceShip);
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
}//end is running

/*******************************************************************
* Function Name: CheckBorders
*
* Description: Stops the bitmap at the borders of the screen.
*
* PseudoCode:
* Determine if the bitmap is going beyond the right border
*   IF (bitmap goes right beyond border)
*       Calculate new x position
*   END IF
* Determine if the bitmap is going beyond the left border
*   IF (bitmap goes beyond left border)
*       Calculate new x position
*   END IF
* Calculate new y position with y direction value
* Determine if the bitmap is going beyond the bottom border
*   IF (bitmap goes beyond bottom border)
*       Calculate new y position
*   END IF
* Determine if the bitmap is going beyond the top border
*   IF (bitmap goes beyond top border)
*       Calculate new y position
*   END IF
* Return int x and int y by reference
*******************************************************************/

void CheckBorder(int& x, int& y, int w, int h)
{
    if(x >= 800-120)
         x = 800 - 120;
    if(x <= 0)
         x = 0;
    if(y >= 600-95)
         y = 600 - 95;
    if(y <= 0)
         y = 0;
}
