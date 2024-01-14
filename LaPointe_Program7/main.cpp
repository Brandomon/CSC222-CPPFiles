#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;

//Create a window
SDL_Window *gameWindow;

//Textures
//Create a texture for the background
SDL_Texture* textureBackground;
//Create a texture for the tile sheet
SDL_Texture* textureMap = NULL;
SDL_Texture* textureDragonEastSheet = NULL;     //sheet of east dragons texture
SDL_Texture* textureDragonWestSheet = NULL;     //sheet of west dragons texture
SDL_Texture* textureDragonNorthSheet = NULL;    //sheet of north dragons texture
SDL_Texture* textureDragonSouthSheet = NULL;    //sheet of south dragons texture
SDL_Texture* textureDragonSheet = NULL;         //generic sheet of dragons texture

//Create a temporary surface because we always need one at hand
SDL_Surface* tempSurface = NULL;
//Create a Renderer 'cause we always need one of these as well
SDL_Renderer* renderer;

//Rectangles
//Source and destination rectangles for tile sheet and placement of one tile
SDL_Rect dstRect;
SDL_Rect srcRect;
SDL_Rect rectBackground;
SDL_Rect solidTileRect;
//All the dragon rectangles
SDL_Rect rectDragonEastSheet;   //Rectangle for sheet of east flying dragons
SDL_Rect rectDragonEast;        //One east flying dragon rectangle
SDL_Rect rectDragonWestSheet;   //Rectangle for sheet of west flying dragons
SDL_Rect rectDragonWest;        //One west flying dragon rectangle
SDL_Rect rectDragonNorthSheet;  //Rectangle for sheet of north flying dragons
SDL_Rect rectDragonNorth;       //One north flying dragon rectangle
SDL_Rect rectDragonSouthSheet;  //Rectangle for sheet of south flying dragons
SDL_Rect rectDragonSouth;       //One south flying dragon rectangle

SDL_Rect rectDragonPlacement;   //Where to place the dragon on the screen
SDL_Rect rectDragon;            //Rectangle containing dragon placed on screen
SDL_Rect rectDragonSheet;       //Sheet containing dragons for currect direction

//Game constants
const int SCREEN_WIDTH = 864;
const int SCREEN_HEIGHT = 640;

const int DRAGON_WIDTH = 128;
const int DRAGON_HEIGHT = 128;

const int FPS = 10;
const int FRAME_DELAY = 95/FPS;

const int DRAGON_SPEED = 2;
const int MAX_DRAGON_FRAME = 8;  //number of dragons per sheet

//Global Variables so everyone can share
//Background variables
int displayRow = 0;             //number of rows you want displayed
int displayCol = 0;             //number of columns you want displayed
int mapRow = 0;                 //number of rows in map
int mapCol = 0;                 //number of columns in map
int tileHeight = 0;             //height of tiles in tile sheet
int tileWidth = 0;              //width of tiles in tile sheet
//number of solid tiles in the map
int numSolidTiles = 0;

//Dragon variables
int SpriteFrame = 0;
int FrameCounter = 0;
int dragonWidth;                //width of one dragon frame
int dragonHeight;               //height of one dragon frame
int xDragonPos = 50;                          //x position of dragon on screen
int yDragonPos = 200;                          //y position of dragon on screen

//Note:  west = 0, east = 1, south = 2, north = 3
int dragonDirection;

//place holders for array of frame ID numbers
int* layer1, *layer2;

int sheetWidth = 0;             //width of destination game screen
int sheetHeight = 0;            //height of destination game screen

vector<int> solidTiles;         //array of solid tile frameIDs
struct SolidTileStruct          //structure to hold position of solid tile data
{
    int x;          //x position of solid tile
    int y;          //y position of solid tile
    bool solid;     //we know the tile is solid but put a 1 here anyway
    int frameID;    //frame ID of solid tile
};
SolidTileStruct tiles[100];

//Generic Function prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
bool ProgramIsRunning();
bool InitGame();
bool InitSDL();
void CreateGameImages();
void RunGame();
void DrawGame();

//Tile function prototypes
void getMapData();
void generateSolidLayer();
void drawLayer(int layer[]);
bool isTileSolid(int tile);

//Dragon function prototypes
void UpdateDragon();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();

//Collision function prototypes
void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect );
void TileCollision(SDL_Rect movingRect);


int main(int argc, char *argv[])
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if(!InitGame())
    {
        printf("Unable to initialize from main.\n");
        CloseShop();   //If InitGame failed, stop the presses
    }

    //Create all the game images
    CreateGameImages();


    while(ProgramIsRunning())
    {
        //CHECK INPUT
        //Move the dragon when the up/down/left right key is pressed
        if(keys[SDL_SCANCODE_UP])
        {
            //move the shape north
            moveUp();
            yDragonPos = yDragonPos - DRAGON_SPEED; //decrement y
        }


        if(keys[SDL_SCANCODE_DOWN])
        {
            //move shape south
            moveDown();
            yDragonPos = yDragonPos + DRAGON_SPEED; //increment y
        }

        if(keys[SDL_SCANCODE_LEFT])
        {
            //move shape east
            moveLeft();
            xDragonPos = xDragonPos - DRAGON_SPEED; //decrement x
        }

        if(keys[SDL_SCANCODE_RIGHT])
        {
            //move shape west
            moveRight();
            xDragonPos = xDragonPos + DRAGON_SPEED;  //increment x
        }
        RunGame();          //Update & Draw
    }//end while not quit

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\tOur fears are like dragons guarding our deepest treasures.\n\n\n\n\n\n\n\n\n\n\n\n\n");
    CloseShop();
    return 0;
}

//Functions

void getMapData()
{
    //Get the tile data
    //Initialize the layers
    layer1 = layer2 = NULL;

    //Load map file
     std::ifstream in("graphics/map.map");

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
    //Get the height value
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

    //7. Get the line that says 'solid_tiles'
    getline(in, buffer, '\n');

    //8. Get the row containing solid tiles
    getline(in, buffer, '\n');

    char* string = new char[buffer.size() + 1];
    strcpy(string, buffer.c_str());

    char* token = strtok(string, " ,");

    while(token != NULL)
    {
        solidTiles.push_back(atoi(token));
        token = strtok(NULL, " ,");
    }

    delete[] string;

    //Set up an array for each layer before getting data for each layer
    layer1 = new int[mapCol * mapRow];
    layer2 = new int[mapCol * mapRow];

    //9. Get the row containing 'layer1' of background (not solid tiles)
    getline(in, buffer, '\n');

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
            layer1[i] = data;                    //store it in data array
            i++;                                //move to next cell in array
        }//end for col
    }//end for row

    //10. Get the row containing 'layer2' of solid tile placement
    getline(in, buffer, '\n');

    i = 0;  //array index
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
            layer2[i] = data;                   //store it in data array
            i++;                                //move to next cell in array
        }//end for col
    }//end for row

    in.close();

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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);

    SDL_DestroyTexture(textureBackground);
    SDL_DestroyTexture(textureMap);

    SDL_DestroyTexture(textureDragonSouthSheet);
    SDL_DestroyTexture(textureDragonNorthSheet);
    SDL_DestroyTexture(textureDragonEastSheet);
    SDL_DestroyTexture(textureDragonWestSheet);
    SDL_DestroyTexture(textureDragonSheet);

    //destroy pointers to arrays
    if (layer1 != NULL)
    {
        delete[] layer1;
        layer1 = NULL;
    }
    if (layer2 != NULL)
    {
        delete[] layer2;
        layer2 = NULL;
    }
    //release vector memory
    solidTiles.clear();

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
    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1)
    {
        printf("Open Audio failed to initialize!\n");
        return false;
    }
    return true;
}

void CreateGameImages()
{
    //SDL_Init( SDL_INIT_EVERYTHING );
    //Create a window
    gameWindow = SDL_CreateWindow(
    "Solid Tile Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
    FillRect(rectBackground, 0, 0, 864, 696);

    //load the tile sheet
    tempSurface = loadImage("graphics/tiles.bmp");
    setTransparentColor(tempSurface, 255, 0, 255);
    textureMap = loadTexture(tempSurface);
    //get the map data and create the solid layer
    getMapData();
    generateSolidLayer();

    //load all dragon stuff
    //Set up East flying dragons
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/flyEast.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon flying east
    //SDL_QueryTexture() method gets the width and height of the sheet of dragons
    SDL_QueryTexture(textureDragonEastSheet, NULL, NULL, &rectDragonEastSheet.w, &rectDragonEastSheet.h);
    //get the width of one frame (one Dragon) by dividing the width of the sheet by the number of frames
    dragonWidth = rectDragonEastSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    dragonHeight = rectDragonEastSheet.h;  //height on one Dragon is same as height of sheet
    //Initialize the rectangle dragon east
    FillRect(rectDragonEast, 0, 0, dragonWidth, dragonHeight);

    //Set up West flying dragons
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/flyWest.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon flying east
    //SDL_QueryTexture() method gets the width and height of the sheet of dragons
    SDL_QueryTexture(textureDragonWestSheet, NULL, NULL, &rectDragonWestSheet.w, &rectDragonWestSheet.h);
    //get the width of one frame (one Dragon) by dividing the width of the sheet by the number of frames
    dragonWidth = rectDragonWestSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    dragonHeight = rectDragonWestSheet.h;  //height on one dragon is same as height of sheet
    //Initialize the rectangle for dragon west
    FillRect(rectDragonWest, 0, 0, dragonWidth, dragonHeight);

    //Set up North flying dragons
    //create the surface - load the north sheet
    tempSurface = loadImage("graphics/flyNorth.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonNorthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon flying east
    //SDL_QueryTexture() method gets the width and height of the sheet of dragons
    SDL_QueryTexture(textureDragonNorthSheet, NULL, NULL, &rectDragonNorthSheet.w, &rectDragonNorthSheet.h);
    //get the width of one frame (one Dragon) by dividing the width of the sheet by the number of frames
    dragonWidth = rectDragonNorthSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    dragonHeight = rectDragonNorthSheet.h;  //height on one dragon is same as height of sheet
    //Initialize the rectangle for dragon north
    FillRect(rectDragonNorth, 0, 0, dragonWidth, dragonHeight);

    //Set up South flying dragons
    //create the surface - load the south sheet
    tempSurface = loadImage("graphics/flySouth.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonSouthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon flying east
    //SDL_QueryTexture() method gets the width and height of the sheet of dragons
    SDL_QueryTexture(textureDragonSouthSheet, NULL, NULL, &rectDragonSouthSheet.w, &rectDragonSouthSheet.h);
    //get the width of one frame (one Dragon) by dividing the width of the sheet by the number of frames
    dragonWidth = rectDragonSouthSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    dragonHeight = rectDragonSouthSheet.h;  //height on one dragon is same as height of sheet
    //Initialize the rectangle for dragon south
    FillRect(rectDragonSouth, 0, 0, dragonWidth, dragonHeight);
}

void generateSolidLayer()   //create a structure to hold solid tile data
{
    int begIndex = 0;   //position of solid tile in tiles structure
    for (int row = 0; row < mapRow; ++row)
    {
            for (int col = 0; col < mapCol; ++col)
            {
                int frame = layer2[row*mapCol+col];//get frame ID of solid tile from layer 2
                if (frame > 0)  //if the frame is not zero
                {
                    if (isTileSolid(layer2[row*mapCol+col])) //if tile is solid
                    {
                        tiles[begIndex].solid = 1;              //then solid field is 1
                        tiles[begIndex].frameID = frame;        //save the frame ID
                        tiles[begIndex].x = col*tileWidth;      //save THIS x position of the solid tile
                        tiles[begIndex].y = row*tileHeight;     //save THIS y position of the solid tile
                        begIndex = begIndex + 1;                //move to the next tile
                    }//end if tile is solid
                }
            }
    }
    //save how many solid tiles there are in the map (not the solid tile array)
    numSolidTiles = begIndex - 1;
}

bool isTileSolid(int tile)
{
    for(unsigned int i = 0; i < solidTiles.size(); i++)
    {
        if(solidTiles[i] == tile)
        {
            return true;
        }
    }
    return false;
}

void drawLayer(int layer[])
{
    //Draw a Layer
        for (int row = 0; row < mapRow; ++row)
        {
            for (int col = 0; col < mapCol; ++col)
            {
                int frame = layer[row*mapCol+col];//get frame ID number to draw
                //if frameID is not a zero then grab it from sheet
                if(frame > 0)
                {
                    //Compute the x and y destination tile position
                    int xRawBytes = ((frame - 1) * tileWidth);
                    int yRawBytes = ((frame - 1) * tileHeight);
                    int xSource = xRawBytes % sheetWidth;
                    int ySource = (yRawBytes / sheetWidth) * tileHeight;
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
}
void UpdateDragon()
{
    //Increment time to delay each frame
    FrameCounter++;
    //If delay time is greater than max amount
    if(FrameCounter > FRAME_DELAY)
    {
        FrameCounter = 0;   //start new delay time
        SpriteFrame++;      //advance to next frame
    }
    if(SpriteFrame > MAX_DRAGON_FRAME - 1) //if last frame is reached
        SpriteFrame = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (dragonDirection == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectDragonEast.x = SpriteFrame * rectDragonEast.w;
        if ( xDragonPos > SCREEN_WIDTH - DRAGON_WIDTH)       //check right
        //if edge of bitmap collides with screen
        {
            xDragonPos = SCREEN_WIDTH - DRAGON_WIDTH;        //stop and kiss the side
        }
        //create the rectangle on screen for dragon placement
        FillRect(rectDragon, rectDragonEast.x, rectDragonEast.y, rectDragonEast.w, rectDragonEast.h);
        textureDragonSheet = textureDragonEastSheet;
    }// end flying east

    if (dragonDirection == 0)                       //flying to the west (left)
    {
        //Advance to next frame on sheet
        rectDragonWest.x = SpriteFrame * rectDragonWest.w;
       // xDragonPos = xDragonPos - DRAGON_SPEED;              //decrement x
        if ( xDragonPos < 0 )                                //check left
        //if edge of bitmap collides with screen
        {
            xDragonPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for dragon placement
        FillRect(rectDragon, rectDragonWest.x, rectDragonWest.y, rectDragonWest.w, rectDragonWest.h);
        textureDragonSheet = textureDragonWestSheet;
    }// end flying West

    if (dragonDirection == 2)                       //if we are going south
    {
        //Advance to next frame on sheet
        rectDragonSouth.x = SpriteFrame * rectDragonSouth.w;
        //yDragonPos = yDragonPos + DRAGON_SPEED;                       //increment y
        if ( yDragonPos > SCREEN_HEIGHT - DRAGON_HEIGHT )    //check bottom
        {
            yDragonPos = SCREEN_HEIGHT - DRAGON_HEIGHT;      //stop and kiss the side
        }
        //create the rectangle on screen for dragon placement
        FillRect(rectDragon, rectDragonSouth.x, rectDragonSouth.y, rectDragonSouth.w, rectDragonSouth.h);
        //copy the south facing dragons to the generic sheet
        textureDragonSheet = textureDragonSouthSheet;
    }// end flying south

    if (dragonDirection == 3)                       //if we are going north
    {
        //Advance to next frame on sheet
        rectDragonNorth.x = SpriteFrame * rectDragonNorth.w;
        //yDragonPos = yDragonPos - DRAGON_SPEED;                       //decrement y
        if ( yDragonPos < 0 )                                //check top
        //if edge of bitmap collides with screen
        {
            yDragonPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for dragon placement
        FillRect(rectDragon, rectDragonNorth.x, rectDragonNorth.y, rectDragonNorth.w, rectDragonNorth.h);
        //copy the north facing dragon to the generic dragon sheet
        textureDragonSheet = textureDragonNorthSheet;
    }// end flying north

    //Initialize the placement rectangle to the position on the screen the dragon will be drawn.
    FillRect(rectDragonPlacement, xDragonPos, yDragonPos, dragonWidth, dragonHeight);

    //Now check to see if the dragon has collided with a solid tile
    TileCollision(rectDragonPlacement);
}
void RunGame()
{
    UpdateDragon();   //Update position of dragon
    DrawGame();
}

void DrawGame()
{
    SDL_RenderPresent(renderer);

    //Clear the window
    SDL_RenderClear(renderer);

    //Copy the background)
    SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);

    //draw each layer
    drawLayer (layer1);
    drawLayer (layer2);

    //Copy the dragon
    SDL_RenderCopy(renderer, textureDragonSheet, &rectDragon, &rectDragonPlacement);

    //display the game screen with updated position of Dragon
    SDL_RenderPresent(renderer);
    SDL_Delay(5);  //delay a bit
}

    //Note:  west = 0, east = 1, south = 2, north = 3
    void moveUp()
    {
        //y's decrease to move north
        //if we are not facing north
        if (dragonDirection != 3)
        {
            dragonDirection = 3;    //then face north
            FrameCounter = 0;       //start new position
        }
    }

    void moveDown()
    {
        //y's increase to move south
        //if we are not facing south
        if (dragonDirection != 2)
        {
            dragonDirection = 2;     //then face south
            FrameCounter = 0;        //start new position
        }
    }

    void moveLeft()
    {
        //x's increase to move west
        //if we are not facing west
        if (dragonDirection != 0)
        {
            dragonDirection = 0;    //then face west
            FrameCounter = 0;       //start new position
        }
    }

    void moveRight()
    {
        //x's decrease to move east
        //if we are not facing east
        if (dragonDirection != 1)
        {
            dragonDirection = 1;    //then face east
            FrameCounter = 0;       //start new position
        }
    }

void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect )
{
        //we're here because we've had a collision with a solid tile
        //now determine what side of the solid tile did the movingRect collide with
        bool intersectsTop = false;     //Collision with top of obstacle
        bool intersectsRight = false;   //Collision with right side of obstacle
        bool moveInXDirection = false;  //Did collision occur while moving right(east) or left(west)?

        if(movingRect.x > obstacleRect.x)   //if the movingRect x is greater than the obstacle x
            intersectsRight = true;         //then the collision is on the right side of the obstacle
        // y up is neg
        if(movingRect.y < obstacleRect.y)   //if the movingRect y is less than the obstacle y
            intersectsTop = true;           //then the collision is on the top of the obstacle

        //compute the height & width of the intersection rectangle (where the two rects collide)
        //to determine which side of the obstacle the movingRect hit
        int cHeight, cWidth;

        if(intersectsTop)  //if the collision is on the top of the obstacle
        {
           cHeight = abs(obstacleRect.y - (movingRect.y + movingRect.h));
        }
        else    //else collision was on bottom, left, or right
        {
            cHeight = abs(obstacleRect.y + obstacleRect.h - movingRect.y);
        }
        if(intersectsRight) //if the collision is on the right of the obstacle
        {
             cWidth = abs(obstacleRect.x + obstacleRect.w - movingRect.x);
        }
        else    //else collision was on left, top, or bottom
        {
            cWidth = abs(obstacleRect.x - (movingRect.x + movingRect.w));
        }

        if (cHeight > cWidth) //was the collision from the left or the right?
            moveInXDirection = true;    //if so then moving in the x direction is true
        else
            moveInXDirection = false;   //if not then moving in the x direction is false

        //now determine which direction the sprite needs to back off from
        if(moveInXDirection)            //if moving left or right
        {
            if(intersectsRight)         //if colliding on the right
            {
                xDragonPos = xDragonPos + 1;    //stop by increasing x (move back right)
            }
            else                        //else colliding on the left
            {
                xDragonPos = xDragonPos - 1;    //stop by decreasing x (move back left)
            }
        }
        else                            //else moving up or down
        {
            if(intersectsTop)           //if colliding with top
            {
                yDragonPos = yDragonPos - 1;    //stop by decreasing y (move back up)
            }
            else                        //else colliding with bottom
            {
                yDragonPos = yDragonPos + 1;    //stop by increasing y (move back down)
            }
        }//end: back off, bub.
}//end Collide

void TileCollision(SDL_Rect movingRect)
{
    //loop indexes to cycle through solid tile vector array
    int begIndex = 0;
    int endIndex = numSolidTiles;

    //make each rectangle a constant
    const SDL_Rect& moving = movingRect;

    for (int i = begIndex; i < endIndex; i++)
    {
        //Get the x and y location of solid tile
        int xDest = tiles[i].x;
        int yDest = tiles[i].y;

        //make the solid tile a rectangle
        FillRect(solidTileRect, xDest, yDest, tileWidth, tileHeight);
        const SDL_Rect& solid = solidTileRect;
        //now check to see if the moving rectangle (sprite) has intersected a solid tile
        if( SDL_HasIntersection(&moving, &solid))
        {
            //the moving sprite has collided with the solid tile so stop that sprite in its tracks
            Collide(movingRect, solidTileRect);
        }//end if intersected
    }//end for
}//end tile collision
