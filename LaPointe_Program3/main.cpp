#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

/*******************************************************************
* Program Name: Adding Sound to the Module 4 Program
*
* Description: This program will allow a bitmap to move around the
*   game screen when controlled by the user with the arrow keys and will
*   allow another bitmap to be moved around the game screen with the
*   w, a, s, and d keys, stopping once the bitmap encounters a boundary.
*
* PseudoCode:
* Initialize SDL
* Create window
* Create renderer to manipulate the window
* Load background
* Create textures to convert surfaces to
* Create a rectangle at position 0, 0 for background
* Load Sprites bitmap
* Set magenta as transparency color
* Create texture
* Create a rectangle at position 0, 0 for Sprites
* WHILE Program Is Running
*   Get the key press from the keyboard
*   IF the escape key was pressed
*       CloseShop
*       Return 1
*   ENDIF
*   IF the A key was pressed
*       Move the first sprite left
*   ENDIF
*   IF the D key was pressed
*       Move the first sprite right
*   ENDIF
*   IF the W key was pressed
*       Move the first sprite up
*   ENDIF
*   IF the S key was pressed
*       Move the first sprite down
*   ENDIF
*   IF the left arrow was pressed
*       Move the second sprite left
*   ENDIF
*   IF the right arrow was pressed
*       Move the second sprite right
*   ENDIF
*   IF the up arrow was pressed
*       Move the second sprite up
*   ENDIF
*   IF the down arrow was pressed
*       Move the second sprite down
*   ENDIF
*   Clear the window
*   Copy the background
*   Call function to check borders
*   Copy Sprites
*   Display the game screen with updated position of Sprites
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
SDL_Texture* textureBackground = NULL;       //RetroBackground texture
SDL_Texture* textureSprite1 = NULL;     //Sprite1 texture
SDL_Texture* textureSprite2 = NULL;     //Sprite2 texture

//Rectangles
SDL_Rect rectBackground;         //RetroBackground rectangle
SDL_Rect rectSprite1;       //Sprite1 rectangle
SDL_Rect rectSprite2;       //Sprite2 rectangle

//Function Prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();
void CheckBorder(int& x, int& y, int w, int h);
void my_audio_callback(void *userdata, Uint8 *stream, int len);

//Global Variables
static Uint8 *audio_pos; //Global pointer to the audio buffer to be played
static Uint32 audio_len; //Global remaining length of the sample we have to play

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
    gameWindow = SDL_CreateWindow("Use the arrow keys or w, a, s, d to move the sprite around and ESC to exit.", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //Load background
    tempSurface = loadImage("graphics/RetroBackground.bmp");

    //Create texture
    textureBackground = loadTexture(tempSurface);

    //Create a rectangle at position 0, 0 for background
    FillRect(rectBackground, 0, 0, 800, 600);

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

        //if the A key was pressed then move Sprite1 left
        if(keys[SDL_SCANCODE_A])
            rectSprite1.x-=8;

        //if the D key was pressed then move Sprite1 right
        if(keys[SDL_SCANCODE_D])
            rectSprite1.x+=8;

        //if the W key was pressed then move Sprite1 up
        if(keys[SDL_SCANCODE_W])
            rectSprite1.y-=8;

        //if the S key was pressed then move Sprite1 down
        if(keys[SDL_SCANCODE_S])
            rectSprite1.y+=8;

        //if the left arrow was pressed then move Sprite2 left
        if(keys[SDL_SCANCODE_LEFT])
            rectSprite2.x-=8;

        //if the right arrow was pressed then move Sprite2 right
        if(keys[SDL_SCANCODE_RIGHT])
            rectSprite2.x+=8;

        //if the up arrow was pressed then move Sprite2 up
        if(keys[SDL_SCANCODE_UP])
            rectSprite2.y-=8;

        //if the down arrow was pressed then move Sprite2 down
        if(keys[SDL_SCANCODE_DOWN])
            rectSprite2.y+=8;

        //Clear the window
        SDL_RenderClear(renderer);

        //Copy the background
        SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);

        //Call function to check borders
        CheckBorder(rectSprite1.x, rectSprite1.y, 75, 75);
        CheckBorder(rectSprite2.x, rectSprite2.y, 75, 75);

        //Load Sprites
        SDL_RenderCopy(renderer, textureSprite1, NULL, &rectSprite1);

        //Copy Sprites
        SDL_RenderCopy(renderer, textureSprite1, NULL, &rectSprite1);
        SDL_RenderCopy(renderer, textureSprite2, NULL, &rectSprite2);

        //Display the game screen with updated position of Sprites
        SDL_RenderPresent(renderer);
        SDL_Delay(10);  //delay a bit

    }//end game loop
    printf("Every closed eye is not sleeping, and every open eye is not seeing.");
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
    SDL_DestroyTexture(textureSprite1);
    SDL_DestroyTexture(textureSprite2);
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
    //Local Variables
    SDL_Event event;
    static Uint32 wavLen;           //Length of the audio file selected
    static Uint8 *wavBuf;           //Buffer containing the audio file
    static SDL_AudioSpec wavSpec;   //A structure that contains the specs of the audio file
    bool running = true;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;

        if(event.type == SDL_KEYDOWN )      //if a key is pressed
        {
            switch(event.key.keysym.sym )   //now determine which key is pressed
            {
                case SDLK_TAB:              //Pause the music with the Tab Key
                    SDL_PauseAudio(1);      //one is for pause
                    break;

                case SDLK_SPACE:            //Play that funky music
                    // load WAV file
                    if(SDL_LoadWAV("audio/Biscuit.wav", &wavSpec, &wavBuf, &wavLen)== NULL)
                        printf("Biscuit won't load.");
                    wavSpec.callback = my_audio_callback;   //prepare a buffer to hold the audio file
                    wavSpec.userdata = NULL;                //initialize the data back to empty
                    audio_pos = wavBuf;                     //pointer to sound buffer
                    audio_len = wavLen;                     //copy length of audio file
                    //Open the audio device
                    SDL_OpenAudio(&wavSpec, NULL);
                    //Play audio
                    SDL_PauseAudio(0);          //zero is for play
                    break;

                case SDLK_BACKSPACE:
                    //stop playing that annoying music
                    SDL_CloseAudio();           //stop playing the note
                    SDL_FreeWAV(wavBuf);        //release the sound file
                    break;

                default:
                    break;
            }//end switch
        }//end if key pressed down
    }//end while
    return running;
}//end is running

/*******************************************************************
*   audio callback function
*   Copy the data (userdata) of the audio buffer into the
*   requesting audio buffer (stream)
*   Only copy the length (len) of the audio file
*******************************************************************/

void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
    audio_len = audio_len;    //length counter (only use half the length so note doesn't play too long)
    if (audio_len == 0)         //done playing note when length counter is at zero
        return;
    if (len > audio_len)        //if length of audio file is greater than counter
            len = audio_len;    //then set equal to counter
    SDL_memcpy (stream, audio_pos, len);     // simply copy from one buffer into the other
    //SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
    audio_pos += len;   //move to next position in audio file
    audio_len -= len;   //decrement counter to determine remaining length of audio file to play
}//end my_audio_callback

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
    if(x >= 800 - w)
         x = 800 - w;
    if(x <= 0)
         x = 0;
    if(y >= 600 - h)
         y = 600 - h;
    if(y <= 0)
         y = 0;
}
