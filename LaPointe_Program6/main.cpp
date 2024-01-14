#include <SDL.h>
#include <cstdlib>
#include <string>
#include <fstream>      // std::ifstream
using namespace std;

//Create a window
SDL_Window *gameWindow;
//Create a texture for the background
SDL_Texture* textureBackground;
//Create a texture for the tile sheet
SDL_Texture* textureMap = NULL;
//Create a temporary surface because we always need one at hand
SDL_Surface* tempSurface = NULL;
//Create a Renderer 'cause we always need one of these as well
SDL_Renderer* renderer;

//Source and destination rectangles for tile sheet and placement of one tile
SDL_Rect dstRect;
SDL_Rect srcRect;
SDL_Rect rectBackground;

//Global Variables so everyone can share
int mapRow = 0;                 //number of rows in map
int mapCol = 0;                 //number of columns in map
int tileHeight = 0;             //height of tiles in tile sheet
int tileWidth = 0;              //width of tiles in tile sheet
int* layer;                     //place holder for array of frame ID numbers
int sheetWidth = 0;             //width of destination game screen
int sheetHeight = 0;            //height of destination game screen


//Function prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();
SDL_Surface* LoadImage(char* fileName);
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
bool ProgramIsRunning();
bool InitGame();
bool InitSDL();
void getMapData();
void CreateGameImages();

int main(int argc, char *argv[])
{
    if(!InitGame())
    {
        printf("Unable to initialize from main.\n");
        CloseShop();   //If InitGame failed, stop the presses
    }

    //Create all the game images
    CreateGameImages();

        SDL_RenderClear(renderer);

         //Load the background;
        SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);
        for (int row = 0; row < mapRow; ++row)
        {
            for (int col = 0; col < mapCol; ++col)
            {
                int frame = layer[row*mapCol+col];//get frame ID number to draw
                //if frameID is not a zero then grab it from sheet
                if(frame > 0)
                {
                    //Compute the x and y source tile position
                    int xRawBytes = ((frame - 1) * tileWidth);
                    int yRawBytes = ((frame - 1) * tileHeight);
                    int xSource = xRawBytes % sheetWidth;
                    int ySource = (yRawBytes / sheetHeight) * tileHeight;

                    //Initialize the source rectangle with position of tile you want to draw
                    FillRect(srcRect, xSource, ySource, tileWidth, tileHeight);

                    // Compute the x and y location on game screen
                    int xDest = col*tileWidth;
                    int yDest = row*tileHeight;

                    //Initialize the destination rectangle with x and y position on game screen
                    FillRect(dstRect, xDest, yDest, tileWidth, tileHeight);

                    //Grab tile from sheet and place on game screen
                    SDL_RenderCopy(renderer, textureMap, &srcRect, &dstRect);

                }//end if frame > 0

            }//end for col

        }//end for row

        SDL_RenderPresent(renderer);
        while(ProgramIsRunning())
        {
        }//end while not quit

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\"Do or do not... There is no try.\"\n\t\t\t\t\t\t\t\t-Yoda\n\n\n\n\n\n\n\n\n\n\n");
    SDL_DestroyTexture(textureMap);
    SDL_DestroyTexture(textureBackground);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);

    SDL_Quit();
    return 0;
}

//Functions

void getMapData()
{
    //Get the tile data

    //Load map file
     std::ifstream in("graphics/Map.map");

    if(!in.good())
        printf("The map did not load.");

    std::string buffer;

    //1. Get the line that says 'destWidth'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    sheetWidth = atoi(buffer.c_str());

    //2. Get the line that says 'destHeight'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    sheetHeight = atoi(buffer.c_str());

    //3. Get the line that says 'mapWidth'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    mapCol = atoi(buffer.c_str());

    //4. Get the line that says 'mapHeight'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    mapRow = atoi(buffer.c_str());

    //5. Get the line that says 'tile_width'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    tileWidth = atoi(buffer.c_str());

    //6. Get the line that says 'tile_height'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    tileHeight = atoi(buffer.c_str());

    //7. Get the line that says 'solid_tiles' and ignore it for now
    getline(in, buffer, '\n');

    //8. Get the row containing solid tiles and ignore it for now
    getline(in, buffer, '\n');

    //9. Get the row containing 'layer1' and ignore it for now
    getline(in, buffer, '\n');
    layer = new int [mapCol * mapRow];

    int i = 0;  //array index
    for(int row = 0; row < mapRow; row++)
    {
        for(int col = 0; col < mapCol; col++)
        {
            char delim = ','; //comma delimiter between frame numbers
            if(col == mapCol - 1)
            {
                delim = '\n';   //EOL delimiter for next line in map
            }
            getline(in, buffer, delim);         //get the frame ID without delimeter
            int data = atoi(buffer.c_str());    //convert to integer
            layer[i] = data;                    //store it in data array
            i++;                                //move to next cell in array
        }//end for col
    }//end for row

}//end load map
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
        printf("Unable to create texture\n");
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
   // map.free();
    SDL_DestroyTexture(textureBackground);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    SDL_DestroyTexture(textureMap);

    //Mix_FreeMusic(GameMusic);
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
}

bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2)
{
    if(rect1.x >= rect2.x+rect2.w)
        return false;
    if(rect1.y >= rect2.y+rect2.h)
        return false;
    if(rect2.x >= rect1.x+rect1.w)
        return false;
    if(rect2.y >= rect1.y+rect1.h)
        return false;
    return true;
}

bool InitGame()
{
    //Init SDL
    if(!InitSDL())
        return false;

    //Initiatialize game variables

    return true;
}

bool InitSDL()
{
     //Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL failed to initialize!\n");
        SDL_Quit();
    }
    //Init audio subsystem
    //if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1)
    //{
    //    printf("Open Audio failed to initialize!\n");
    //    return false;
    //}
    return true;
}

void CreateGameImages()
{
    //SDL_Init( SDL_INIT_EVERYTHING );
    //Create a window
    gameWindow = SDL_CreateWindow(
    "LaPointe - Module 9 Program 6", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 864, 640, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

     //load background
    tempSurface = loadImage("graphics/background.bmp");
    if (tempSurface == NULL)
        printf("Background did not load.\n");

    //create texture
    textureBackground = loadTexture(tempSurface);
    if (textureBackground == NULL)
        printf("textureBackground did not load.\n");

    // Create a rectangle at position 0, 0 for background
    FillRect(rectBackground, 0, 0, 864, 640);

    //load the tile sheet
    tempSurface = loadImage("graphics/tiles.bmp");
    setTransparentColor(tempSurface, 255, 0, 255);
    textureMap = loadTexture(tempSurface);

    getMapData();
}
