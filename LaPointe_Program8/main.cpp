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
//Create a texture for the tile sheet
SDL_Texture* textureMap = NULL;
SDL_Texture* texturePiggyEastSheet = NULL;      //sheet of east piggies texture
SDL_Texture* texturePiggyWestSheet = NULL;      //sheet of west piggies texture
SDL_Texture* texturePiggyNorthSheet = NULL;     //sheet of north piggies texture
SDL_Texture* texturePiggySouthSheet = NULL;     //sheet of south piggies texture
SDL_Texture* texturePiggySheet = NULL;          //generic sheet of piggies texture

SDL_Texture* textureFarmerEastSheet = NULL;     //sheet of east farmer texture
SDL_Texture* textureFarmerWestSheet = NULL;     //sheet of west farmer texture
SDL_Texture* textureFarmerSheetEW = NULL;       //generic sheet of farmer texture

SDL_Texture* textureFarmerNorthSheet = NULL;    //sheet of north farmer texture
SDL_Texture* textureFarmerSouthSheet = NULL;    //sheet of south farmer texture
SDL_Texture* textureFarmerSheetNS = NULL;       //generic sheet of farmer texture

SDL_Texture* textureZombieEastSheet = NULL;     //sheet of east zombie texture
SDL_Texture* textureZombieWestSheet = NULL;     //sheet of west zombie texture
SDL_Texture* textureZombieSheetEW = NULL;       //generic sheet of zombie texture

SDL_Texture* textureDragonEastSheet = NULL;     //sheet of east dragon texture
SDL_Texture* textureDragonWestSheet = NULL;     //sheet of west dragon texture
SDL_Texture* textureDragonSheetEW = NULL;       //generic sheet of dragon texture

SDL_Texture* textureExplosionSheet = NULL;      //sheet of explosion texture

//Create a temporary surface because we always need one at hand
SDL_Surface* tempSurface = NULL;
//Create a Renderer 'cause we always need one of these as well
SDL_Renderer* renderer;

//Rectangles
//Source and destination rectangles for tile sheet and placement of one tile
SDL_Rect dstRect;
SDL_Rect srcRect;
//SDL_Rect rectBackground;
SDL_Rect solidTileRect;

//All the piggy rectangles
SDL_Rect rectPiggyEastSheet;   //Rectangle for sheet of east walking piggies
SDL_Rect rectPiggyEast;        //One east walking piggy rectangle
SDL_Rect rectPiggyWestSheet;   //Rectangle for sheet of west walking piggies
SDL_Rect rectPiggyWest;        //One west walking piggy rectangle
SDL_Rect rectPiggyNorthSheet;  //Rectangle for sheet of north walking piggies
SDL_Rect rectPiggyNorth;       //One north walking piggy rectangle
SDL_Rect rectPiggySouthSheet;  //Rectangle for sheet of south walking piggies
SDL_Rect rectPiggySouth;       //One south walking piggy rectangle

SDL_Rect rectPiggyPlacement;   //Where to place the piggy on the screen
SDL_Rect rectPiggy;            //Rectangle containing piggy placed on screen
SDL_Rect rectPiggySheet;       //Sheet containing piggies for current direction

//All the farmer rectangles
SDL_Rect rectFarmerEastSheet;   //Rectangle for sheet of east walking farmer
SDL_Rect rectFarmerEast;        //One east walking farmer rectangle
SDL_Rect rectFarmerWestSheet;   //Rectangle for sheet of west walking farmer
SDL_Rect rectFarmerWest;        //One west walking farmer rectangle
SDL_Rect rectFarmerNorthSheet;  //Rectangle for sheet of north walking farmer
SDL_Rect rectFarmerNorth;       //One north walking farmer rectangle
SDL_Rect rectFarmerSouthSheet;  //Rectangle for sheet of south walking farmer
SDL_Rect rectFarmerSouth;       //One south walking farmer rectangle

SDL_Rect rectFarmerPlacementEW;   //Where to place the e/w farmer on the screen
SDL_Rect rectFarmerSheetEW;       //Sheet containing e/w farmer for current direction
SDL_Rect rectFarmerEW;            //Rectangle containing e/w farmer placed on screen

SDL_Rect rectFarmerPlacementNS;   //Where to place the n/s farmer on the screen
SDL_Rect rectFarmerSheetNS;       //Sheet containing n/s farmer for current direction
SDL_Rect rectFarmerNS;            //Rectangle containing n/s farmer placed on screen

//All the zombie rectangles
SDL_Rect rectZombieEastSheet;   //Rectangle for sheet of east walking zombie
SDL_Rect rectZombieEast;        //One east walking zombie rectangle
SDL_Rect rectZombieWestSheet;   //Rectangle for sheet of west walking zombie
SDL_Rect rectZombieWest;        //One west walking zombie rectangle

SDL_Rect rectZombiePlacementEW;   //Where to place the e/w zombie on the screen
SDL_Rect rectZombieSheetEW;       //Sheet containing e/w zombie for current direction
SDL_Rect rectZombieEW;            //Rectangle containing e/w zombie placed on screen

//All the dragon rectangles
SDL_Rect rectDragonEastSheet;   //Rectangle for sheet of east flying dragon
SDL_Rect rectDragonEast;        //One east flying dragon rectangle
SDL_Rect rectDragonWestSheet;   //Rectangle for sheet of west flying dragon
SDL_Rect rectDragonWest;        //One west flying dragon rectangle

SDL_Rect rectDragonPlacementEW;   //Where to place the e/w dragon on the screen
SDL_Rect rectDragonSheetEW;       //Sheet containing e/w dragon for current direction
SDL_Rect rectDragonEW;            //Rectangle containing e/w dragon placed on screen

//Explosion rectangles
SDL_Rect rectExplosionSheet;    //Rectangle for sheet of explosion
SDL_Rect rectExplosion;         //One explosion rectangle

SDL_Rect rectExplosionPlacement;    //Where to place the explosion on the screen

//Game constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

const int PIGGY_WIDTH = 32;
const int PIGGY_HEIGHT = 32;

const int FARMER_WIDTH = 32;
const int FARMER_HEIGHT = 32;

const int ZOMBIE_WIDTH = 32;
const int ZOMBIE_HEIGHT = 30;

const int DRAGON_WIDTH = 48;
const int DRAGON_HEIGHT = 48;

const int EXPLOSION_WIDTH = 105;
const int EXPLOSION_HEIGHT = 105;

const int FPS = 30;
const int FRAME_DELAY = 100/FPS;

const int PIGGY_SPEED = 1;
const int MAX_PIGGY_FRAME = 9;  //number of piggies per sheet

const int FARMER_SPEED = 1;     //the farmer moves the same speed as the piggy and zombie
const int MAX_FARMER_FRAME = 8; //number of farmers per sheet

const int ZOMBIE_SPEED = 1;     //the zombie moves the same speed as the piggy and farmer
const int MAX_ZOMBIE_FRAME = 8; //number of zombies per sheet

const int DRAGON_SPEED = 2;     //the dragon moves faster than all
const int MAX_DRAGON_FRAME = 8; //number of dragons per sheet

const int MAX_EXPLOSION_FRAME = 10;  //number of explosions per sheet

//Global Variables so everyone can share
//Background variables
int displayRow = 0;             //number of rows you want displayed
int displayCol = 0;             //number of columns you want displayed
int mapRow = 0;                 //number of rows in map
int mapCol = 0;                 //number of columns in map
int tileHeight = 0;             //height of tiles in tile sheet
int tileWidth = 0;              //width of tiles in tile sheet

//Piggy variables
int piggyWidth;                 //width of one piggy frame
int piggyHeight;                //height of one piggy frame
int SpriteFrame = 0;
int FrameCounter = 0;
int xPiggyPos = 0;              //x position of piggy on screen
int yPiggyPos = 64;             //y position of piggy on screen
bool piggyDestroyed = false;

//Farmer variables
int farmerWidth;                //width of one farmer frame
int farmerHeight;               //height of one farmer frame
int SpriteFrameFarmerEW = 0;
int FrameCounterFarmerEW = 0;
int xFarmerEWpos = 320;         //x position of e/w farmer on screen
int yFarmerEWpos = 210;         //y position of e/w farmer on screen
bool farmerDestroyed = false;

//Farmer2 variables
int farmer2Width;                //width of one farmer2 frame
int farmer2Height;               //height of one farmer2 frame
int SpriteFrameFarmer2EW = 0;
int FrameCounterFarmer2EW = 0;
int xFarmer2EWpos = 320;         //x position of e/w farmer2 on screen
int yFarmer2EWpos = 210;         //y position of e/w farmer2 on screen
bool farmer2Destroyed = false;

//Zombie variables
int zombieWidth;                //width of one zombie frame
int zombieHeight;               //height of one zombie frame
int SpriteFrameZombieEW = 0;
int FrameCounterZombieEW = 0;
int xZombieEWpos = 300;         //x position of e/w zombie on screen
int yZombieEWpos = 350;         //y position of e/w zombie on screen
bool zombieDestroyed = false;

//Dragon variables
int dragonWidth;                //width of one dragon frame
int dragonHeight;               //height of one dragon frame
int SpriteFrameDragonEW = 0;
int FrameCounterDragonEW = 0;
int xDragonEWpos = 352;         //x position of e/w dragon on screen
int yDragonEWpos = 560;         //y position of e/w dragon on screen
bool dragonDestroyed = false;

//Explosion variables
int explosionWidth;
int explosionHeight;
int SpriteFrameExplosion = 0;
int FrameCounterExplosion = 0;
int xExplosionPos = 0;
int yExplosionPos = 0;

//Note:  west = 0, east = 1, south = 2, north = 3
int piggyDirection;
int farmerDirectionEW = 1;
int farmerDirectionNS = 2;
int zombieDirectionEW = 1;
int dragonDirectionEW = 1;

//place holders for array of frame ID numbers
int* layer1, *layer2;

int sheetWidth = 0;     //width of destination game screen
int sheetHeight = 0;    //height of destination game screen

vector<int> solidTiles;         //array of solid tile frameIDs
struct SolidTileStruct          //structure to hold position of solid tile data
{
    int x;          //x position of solid tile
    int y;          //y position of solid tile
    bool solid;     //we know the tile is solid but put a 1 here anyway
    int frameID;    //frame ID of solid tile
};
SolidTileStruct tiles[1000];
int numSolidTiles = 0;

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

//void setUpEachSheet(const char* path , int r, int g, int b, int maxFrame, SDL_Texture textureSheet, SDL_Rect rectSheet, SDL_rectDirSheet)

//Tile function prototypes
void getMapData();
void generateSolidLayer();
void drawLayer(int layer[]);
bool isTileSolid(int tile);

//Piggy function prototypes
void UpdatePiggy();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();

//Farmer function prototypes
void updateEastWestFarmer();
void updateNorthSouthFarmer();
void updateEastWestZombie();
void updateEastWestDragon();

//Collision function prototypes
void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &xPos, int &yPos );
void enemyCollide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &direction);
void PlayerTileCollision(SDL_Rect movingRect, int &xPos, int &yPos);
bool EnemyTileCollision(SDL_Rect movingRect, int &direction);
void playerEnemyCollide(SDL_Rect playerRect, SDL_Rect enemyRect);
bool CheckCollision(SDL_Rect r1,SDL_Rect r2);
void explosion(int xEnemyPos, int yEnemyPos);

//Destruction function prototypes
void destroyFarmer();
void destroyFarmer2();
void destroyPiggy();
void destroyZombie();
void destroyDragon();
void destroyExplosion();


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
        //Move the piggy when the up/down/left right key is pressed
        if(keys[SDL_SCANCODE_UP])
        {
            //move the shape north
            moveUp();
            yPiggyPos = yPiggyPos - PIGGY_SPEED; //decrement y
        }

        if(keys[SDL_SCANCODE_DOWN])
        {
            //move shape south
            moveDown();
            yPiggyPos = yPiggyPos + PIGGY_SPEED; //increment y
        }

        if(keys[SDL_SCANCODE_LEFT])
        {
            //move shape east
            moveLeft();
            xPiggyPos = xPiggyPos - PIGGY_SPEED; //decrement x
        }

        if(keys[SDL_SCANCODE_RIGHT])
        {
            //move shape west
            moveRight();
            xPiggyPos = xPiggyPos + PIGGY_SPEED;  //increment x
        }
        RunGame();          //Update & Draw
    }//end while not quit

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\"Somewhere, something incredible is waiting to be known.\"\n\t\t\t\t\t\t\t\t-Carl Sagan\n\n\n\n\n\n\n\n\n\n\n");
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

    //SDL_DestroyTexture(textureBackground);
    SDL_DestroyTexture(textureMap);

    if (piggyDestroyed == false)
    {
        destroyPiggy();
    }
    if (farmerDestroyed == false)
    {
        destroyFarmer();
    }

    if (zombieDestroyed == false)
    {
        destroyZombie();
    }

    if (dragonDestroyed == false)
    {
        destroyDragon();
    }

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

void destroyPiggy()
{
    SDL_DestroyTexture(texturePiggySouthSheet);
    SDL_DestroyTexture(texturePiggyNorthSheet);
    SDL_DestroyTexture(texturePiggyEastSheet);
    SDL_DestroyTexture(texturePiggyWestSheet);
    SDL_DestroyTexture(texturePiggySheet);
}
void destroyFarmer()
{
    SDL_DestroyTexture(textureFarmerEastSheet);
    SDL_DestroyTexture(textureFarmerWestSheet);
    SDL_DestroyTexture(textureFarmerSheetEW);
}

void destroyZombie()
{
    SDL_DestroyTexture(textureZombieEastSheet);
    SDL_DestroyTexture(textureZombieWestSheet);
    SDL_DestroyTexture(textureZombieSheetEW);
}

void destroyDragon()
{
    SDL_DestroyTexture(textureDragonEastSheet);
    SDL_DestroyTexture(textureDragonWestSheet);
    SDL_DestroyTexture(textureDragonSheetEW);
}

void destroyExplosion()
{
    SDL_DestroyTexture(textureExplosionSheet);
}

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
    "Tile Maze with More Enemies", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //load the tile sheet
    tempSurface = loadImage("graphics/tiles.bmp");
    //no need to set transparent color for the tiles
    textureMap = loadTexture(tempSurface);
    //get the map data and create the solid layer
    getMapData();
    generateSolidLayer();

    //setUpEachSheet("graphics/WalkEast/eastPiggies.bmp", 106, 76, 48, MAX_PIGGY_FRAME, texturePiggyEastSheet, SDL_Rect rectPiggyEast);

    //load all piggy stuff
    //Set up East walking piggys
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/piggy/eastPiggies.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    texturePiggyEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one piggy walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of piggys
    SDL_QueryTexture(texturePiggyEastSheet, NULL, NULL, &rectPiggyEastSheet.w, &rectPiggyEastSheet.h);
    //get the width of one frame (one Piggy) by dividing the width of the sheet by the number of frames
    piggyWidth = rectPiggyEastSheet.w/MAX_PIGGY_FRAME;
    //get the height of one frame
    piggyHeight = rectPiggyEastSheet.h;  //height on one Piggy is same as height of sheet
    //Initialize the rectangle piggy east
    FillRect(rectPiggyEast, 0, 0, piggyWidth, piggyHeight);

    //Set up West walking piggys
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/piggy/westPiggies.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    texturePiggyWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one piggy walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of piggys
    SDL_QueryTexture(texturePiggyWestSheet, NULL, NULL, &rectPiggyWestSheet.w, &rectPiggyWestSheet.h);
    //get the width of one frame (one Piggy) by dividing the width of the sheet by the number of frames
    piggyWidth = rectPiggyWestSheet.w/MAX_PIGGY_FRAME;
    //get the height of one frame
    piggyHeight = rectPiggyWestSheet.h;  //height on one piggy is same as height of sheet
    //Initialize the rectangle for piggy west
    FillRect(rectPiggyWest, 0, 0, piggyWidth, piggyHeight);

    //Set up North walking piggys
    //create the surface - load the north sheet
    tempSurface = loadImage("graphics/piggy/northPiggies.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    texturePiggyNorthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one piggy walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of piggys
    SDL_QueryTexture(texturePiggyNorthSheet, NULL, NULL, &rectPiggyNorthSheet.w, &rectPiggyNorthSheet.h);
    //get the width of one frame (one Piggy) by dividing the width of the sheet by the number of frames
    piggyWidth = rectPiggyNorthSheet.w/MAX_PIGGY_FRAME;
    //get the height of one frame
    piggyHeight = rectPiggyNorthSheet.h;  //height on one piggy is same as height of sheet
    //Initialize the rectangle for piggy north
    FillRect(rectPiggyNorth, 0, 0, piggyWidth, piggyHeight);

    //Set up South walking piggys
    //create the surface - load the south sheet
    tempSurface = loadImage("graphics/piggy/southPiggies.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    texturePiggySouthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one piggy walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of piggys
    SDL_QueryTexture(texturePiggySouthSheet, NULL, NULL, &rectPiggySouthSheet.w, &rectPiggySouthSheet.h);
    //get the width of one frame (one Piggy) by dividing the width of the sheet by the number of frames
    piggyWidth = rectPiggySouthSheet.w/MAX_PIGGY_FRAME;
    //get the height of one frame
    piggyHeight = rectPiggySouthSheet.h;  //height on one piggy is same as height of sheet
    //Initialize the rectangle for piggy south
    FillRect(rectPiggySouth, 0, 0, piggyWidth, piggyHeight);

    //load all farmer stuff
    //Set up East walking farmer
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/farmer/eastFarmer.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureFarmerEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one farmer walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of farmer
    SDL_QueryTexture(textureFarmerEastSheet, NULL, NULL, &rectFarmerEastSheet.w, &rectFarmerEastSheet.h);
    //get the width of one frame (one Farmer) by dividing the width of the sheet by the number of frames
    farmerWidth = rectFarmerEastSheet.w/MAX_FARMER_FRAME;
    //get the height of one frame
    farmerHeight = rectFarmerEastSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer east
    FillRect(rectFarmerEast, 0, 0, farmerWidth, farmerHeight);

    //Set up West walking farmer
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/farmer/westFarmer.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureFarmerWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one farmer walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of farmer
    SDL_QueryTexture(textureFarmerWestSheet, NULL, NULL, &rectFarmerWestSheet.w, &rectFarmerWestSheet.h);
    //get the width of one frame (one Farmer) by dividing the width of the sheet by the number of frames
    farmerWidth = rectFarmerWestSheet.w/MAX_FARMER_FRAME;
    //get the height of one frame
    farmerHeight = rectFarmerWestSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer west
    FillRect(rectFarmerWest, 0, 0, farmerWidth, farmerHeight);

    //Set up North walking farmer
    //create the surface - load the north sheet
    tempSurface = loadImage("graphics/farmer/northFarmer.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureFarmerNorthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one farmer walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of farmer
    SDL_QueryTexture(textureFarmerNorthSheet, NULL, NULL, &rectFarmerNorthSheet.w, &rectFarmerNorthSheet.h);
    //get the width of one frame (one Farmer) by dividing the width of the sheet by the number of frames
    farmerWidth = rectFarmerNorthSheet.w/MAX_FARMER_FRAME;
    //get the height of one frame
    farmerHeight = rectFarmerNorthSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer east
    FillRect(rectFarmerNorth, 0, 0, farmerWidth, farmerHeight);

    //Set up South walking farmer
    //create the surface - load the south sheet
    tempSurface = loadImage("graphics/farmer/southFarmer.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureFarmerSouthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one farmer walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of farmer
    SDL_QueryTexture(textureFarmerSouthSheet, NULL, NULL, &rectFarmerSouthSheet.w, &rectFarmerSouthSheet.h);
    //get the width of one frame (one Farmer) by dividing the width of the sheet by the number of frames
    farmerWidth = rectFarmerSouthSheet.w/MAX_FARMER_FRAME;
    //get the height of one frame
    farmerHeight = rectFarmerSouthSheet.h;  //height on one farmer is same as height of sheet
    //Initialize the rectangle farmer west
    FillRect(rectFarmerSouth, 0, 0, farmerWidth, farmerHeight);

    //load all zombie stuff
    //Set up East walking zombie
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/zombie/eastZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombieEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombieEastSheet, NULL, NULL, &rectZombieEastSheet.w, &rectZombieEastSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombieWidth = rectZombieEastSheet.w/MAX_ZOMBIE_FRAME;
    //get the height of one frame
    zombieHeight = rectZombieEastSheet.h;  //height on one zombie is same as height of sheet
    //Initialize the rectangle zombie east
    FillRect(rectZombieEast, 0, 0, zombieWidth, zombieHeight);

    //Set up West walking zombie
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/zombie/westZombie.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureZombieWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one zombie walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of zombie
    SDL_QueryTexture(textureZombieWestSheet, NULL, NULL, &rectZombieWestSheet.w, &rectZombieWestSheet.h);
    //get the width of one frame (one zombie) by dividing the width of the sheet by the number of frames
    zombieWidth = rectZombieWestSheet.w/MAX_ZOMBIE_FRAME;
    //get the height of one frame
    zombieHeight = rectZombieWestSheet.h;  //height on one zombie is same as height of sheet
    //Initialize the rectangle zombie west
    FillRect(rectZombieWest, 0, 0, zombieWidth, zombieHeight);

    //load all dragon stuff
    //Set up East flying dragon
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/dragon/flyEast.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon flying east
    //SDL_QueryTexture() method gets the width and height of the sheet of dragon
    SDL_QueryTexture(textureDragonEastSheet, NULL, NULL, &rectDragonEastSheet.w, &rectDragonEastSheet.h);
    //get the width of one frame (one dragon) by dividing the width of the sheet by the number of frames
    dragonWidth = rectDragonEastSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    dragonHeight = rectDragonEastSheet.h;  //height on one dragon is same as height of sheet
    //Initialize the rectangle dragon east
    FillRect(rectDragonEast, 0, 0, dragonWidth, dragonHeight);

    //Set up West flying dragon
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/dragon/flyWest.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon flying west
    //SDL_QueryTexture() method gets the width and height of the sheet of dragon
    SDL_QueryTexture(textureDragonWestSheet, NULL, NULL, &rectDragonWestSheet.w, &rectDragonWestSheet.h);
    //get the width of one frame (one dragon) by dividing the width of the sheet by the number of frames
    dragonWidth = rectDragonWestSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    dragonHeight = rectDragonWestSheet.h;  //height on one dragon is same as height of sheet
    //Initialize the rectangle dragon west
    FillRect(rectDragonWest, 0, 0, dragonWidth, dragonHeight);

    //load all explosion stuff
    //Set up explosion
    //create the surface - load the sheet
    tempSurface = loadImage("graphics/explosion.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);
    //create the texture for the whole sheet
    textureExplosionSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one explosion
    //SDL_QueryTexture() method gets the width and height of the sheet of explosion
    SDL_QueryTexture(textureExplosionSheet, NULL, NULL, &rectExplosionSheet.w, &rectExplosionSheet.h);
    //get the width of one frame (one explosion) by dividing the width of the sheet by the number of frames
    explosionWidth = rectExplosionSheet.w/MAX_EXPLOSION_FRAME;
    //get the height of one frame
    explosionHeight = rectExplosionSheet.h;  //height on one dragon is same as height of sheet
    //Initialize the rectangle explosion
    FillRect(rectExplosion, 0, 0, explosionWidth, explosionHeight);
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

void UpdatePiggy()
{
    //Increment time to delay each frame
    FrameCounter++;
    //If delay time is greater than max amount
    if(FrameCounter > FRAME_DELAY)
    {
        FrameCounter = 0;   //start new delay time
        SpriteFrame++;      //advance to next frame
    }
    if(SpriteFrame > MAX_PIGGY_FRAME - 1) //if last frame is reached
        SpriteFrame = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (piggyDirection == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectPiggyEast.x = SpriteFrame * rectPiggyEast.w;
        if ( xPiggyPos > SCREEN_WIDTH - PIGGY_WIDTH)       //check right
        //if edge of bitmap collides with screen
        {
            xPiggyPos = SCREEN_WIDTH - PIGGY_WIDTH;        //stop and kiss the side
        }
        //create the rectangle on screen for piggy placement
        FillRect(rectPiggy, rectPiggyEast.x, rectPiggyEast.y, rectPiggyEast.w, rectPiggyEast.h);
        texturePiggySheet = texturePiggyEastSheet;
    }// end walking east

    if (piggyDirection == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectPiggyWest.x = SpriteFrame * rectPiggyWest.w;
       // xPiggyPos = xPiggyPos - PIGGY_SPEED;              //decrement x
        if ( xPiggyPos < 0 )                                //check left
        //if edge of bitmap collides with screen
        {
            xPiggyPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for piggy placement
        FillRect(rectPiggy, rectPiggyWest.x, rectPiggyWest.y, rectPiggyWest.w, rectPiggyWest.h);
        texturePiggySheet = texturePiggyWestSheet;
    }// end walking West

    if (piggyDirection == 2)                       //if we are going south
    {
        //Advance to next frame on sheet
        rectPiggySouth.x = SpriteFrame * rectPiggySouth.w;
        //yPiggyPos = yPiggyPos + PIGGY_SPEED;                       //increment y
        if ( yPiggyPos > SCREEN_HEIGHT - PIGGY_HEIGHT )    //check bottom
        {
            yPiggyPos = SCREEN_HEIGHT - PIGGY_HEIGHT;      //stop and kiss the side
        }
        //create the rectangle on screen for piggy placement
        FillRect(rectPiggy, rectPiggySouth.x, rectPiggySouth.y, rectPiggySouth.w, rectPiggySouth.h);
        //copy the south facing piggys to the generic sheet
        texturePiggySheet = texturePiggySouthSheet;
    }// end walking south

    if (piggyDirection == 3)                       //if we are going north
    {
        //Advance to next frame on sheet
        rectPiggyNorth.x = SpriteFrame * rectPiggyNorth.w;
        //yPiggyPos = yPiggyPos - PIGGY_SPEED;                       //decrement y
        if ( yPiggyPos < 0 )                                //check top
        //if edge of bitmap collides with screen
        {
            yPiggyPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for piggy placement
        FillRect(rectPiggy, rectPiggyNorth.x, rectPiggyNorth.y, rectPiggyNorth.w, rectPiggyNorth.h);
        //copy the north facing piggy to the generic piggy sheet
        texturePiggySheet = texturePiggyNorthSheet;
    }// end walking north

    //Initialize the placement rectangle to the position on the screen the piggy will be drawn.
    FillRect(rectPiggyPlacement, xPiggyPos, yPiggyPos, piggyWidth, piggyHeight);

    //Now check to see if the piggy has collided with a solid tile
    PlayerTileCollision(rectPiggyPlacement, xPiggyPos, yPiggyPos);

    //Now check to see if the piggy has collided with an enemy farmer
    playerEnemyCollide(rectPiggyPlacement, rectFarmerPlacementEW);

    //Now check to see if the piggy has collided with an enemy zombie
    playerEnemyCollide(rectPiggyPlacement, rectZombiePlacementEW);

    //Now check to see if the piggy has collided with an enemy dragon
    playerEnemyCollide(rectPiggyPlacement, rectDragonPlacementEW);
}

void updateEastWestFarmer()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterFarmerEW++;
    //If delay time is greater than max amount
    if(FrameCounterFarmerEW > FRAME_DELAY)
    {
        FrameCounterFarmerEW = 0;   //start new delay time
        SpriteFrameFarmerEW++;      //advance to next frame
    }
    if(SpriteFrameFarmerEW > MAX_FARMER_FRAME - 1) //if last frame is reached
        SpriteFrameFarmerEW = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (farmerDirectionEW == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectFarmerEast.x = SpriteFrameFarmerEW * rectFarmerEast.w;
        xFarmerEWpos = xFarmerEWpos + FARMER_SPEED;              //increment x
        //create the rectangle on screen for farmer placement
        FillRect(rectFarmerEW, rectFarmerEast.x, rectFarmerEast.y, rectFarmerEast.w, rectFarmerEast.h);
        textureFarmerSheetEW = textureFarmerEastSheet;
    }// end walking east

    if (farmerDirectionEW == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectFarmerWest.x = SpriteFrameFarmerEW * rectFarmerWest.w;
        xFarmerEWpos = xFarmerEWpos - FARMER_SPEED;              //decrement x
        //create the rectangle on screen for farmer placement
        FillRect(rectFarmerEW, rectFarmerWest.x, rectFarmerWest.y, rectFarmerWest.w, rectFarmerWest.h);
        textureFarmerSheetEW = textureFarmerWestSheet;
    }// end walking West

    //Now check to see if the farmer has collided with a solid tile
    collision = EnemyTileCollision(rectFarmerPlacementEW, farmerDirectionEW);

    if (collision == true)
    {
        FrameCounterFarmerEW = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the farmer will be drawn.
    FillRect(rectFarmerPlacementEW, xFarmerEWpos, yFarmerEWpos, farmerWidth, farmerHeight);
}

void updateEastWestZombie()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterZombieEW++;
    //If delay time is greater than max amount
    if(FrameCounterZombieEW > FRAME_DELAY)
    {
        FrameCounterZombieEW = 0;   //start new delay time
        SpriteFrameZombieEW++;      //advance to next frame
    }
    if(SpriteFrameZombieEW > MAX_FARMER_FRAME - 1) //if last frame is reached
        SpriteFrameZombieEW = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (zombieDirectionEW == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectZombieEast.x = SpriteFrameZombieEW * rectZombieEast.w;
        xZombieEWpos = xZombieEWpos + ZOMBIE_SPEED;              //increment x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombieEW, rectZombieEast.x, rectZombieEast.y, rectZombieEast.w, rectZombieEast.h);
        textureZombieSheetEW = textureZombieEastSheet;
    }// end walking east

    if (zombieDirectionEW == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectZombieWest.x = SpriteFrameZombieEW * rectZombieWest.w;
        xZombieEWpos = xZombieEWpos - ZOMBIE_SPEED;              //decrement x
        //create the rectangle on screen for zombie placement
        FillRect(rectZombieEW, rectZombieWest.x, rectZombieWest.y, rectZombieWest.w, rectZombieWest.h);
        textureZombieSheetEW = textureZombieWestSheet;
    }// end walking West

    //Now check to see if the zombie has collided with a solid tile
    collision = EnemyTileCollision(rectZombiePlacementEW, zombieDirectionEW);

    if (collision == true)
    {
        FrameCounterZombieEW = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the zombie will be drawn.
    FillRect(rectZombiePlacementEW, xZombieEWpos, yZombieEWpos, zombieWidth, zombieHeight);
}

void updateEastWestDragon()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterDragonEW++;
    //If delay time is greater than max amount
    if(FrameCounterDragonEW > FRAME_DELAY)
    {
        FrameCounterDragonEW = 0;   //start new delay time
        SpriteFrameDragonEW++;      //advance to next frame
    }
    if(SpriteFrameDragonEW > MAX_FARMER_FRAME - 1) //if last frame is reached
        SpriteFrameDragonEW = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (dragonDirectionEW == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectDragonEast.x = SpriteFrameDragonEW * rectDragonEast.w;
        xDragonEWpos = xDragonEWpos + DRAGON_SPEED;              //increment x
        //create the rectangle on screen for zombie placement
        FillRect(rectDragonEW, rectDragonEast.x, rectDragonEast.y, rectDragonEast.w, rectDragonEast.h);
        textureDragonSheetEW = textureDragonEastSheet;
    }// end walking east

    if (dragonDirectionEW == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectDragonWest.x = SpriteFrameDragonEW * rectDragonWest.w;
        xDragonEWpos = xDragonEWpos - DRAGON_SPEED;              //decrement x
        //create the rectangle on screen for zombie placement
        FillRect(rectDragonEW, rectDragonWest.x, rectDragonWest.y, rectDragonWest.w, rectDragonWest.h);
        textureDragonSheetEW = textureDragonWestSheet;
    }// end walking West

    //Now check to see if the dragon has collided with a solid tile
    collision = EnemyTileCollision(rectDragonPlacementEW, dragonDirectionEW);

    if (collision == true)
    {
        FrameCounterDragonEW = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the dragon will be drawn.
    FillRect(rectDragonPlacementEW, xDragonEWpos, yDragonEWpos, dragonWidth, dragonHeight);
}

void RunGame()
{
    UpdatePiggy();              //Update position of piggy
    updateEastWestFarmer();     //Update position of east/west farmer
    updateEastWestZombie();     //Update position of east/west zombie
    updateEastWestDragon();     //Update position of east/west dragon
    DrawGame();
}

void DrawGame()
{
    SDL_RenderPresent(renderer);

    //Clear the window
    SDL_RenderClear(renderer);

    //Copy the background)
    //SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);

    //draw each layer
    drawLayer (layer1);
    drawLayer (layer2);

    //Render the piggy
    SDL_RenderCopy(renderer, texturePiggySheet, &rectPiggy, &rectPiggyPlacement);
    //Render the farmer
    SDL_RenderCopy(renderer, textureFarmerSheetEW, &rectFarmerEW, &rectFarmerPlacementEW);
    SDL_RenderCopy(renderer, textureZombieSheetEW, &rectZombieEW, &rectZombiePlacementEW);
    SDL_RenderCopy(renderer, textureDragonSheetEW, &rectDragonEW, &rectDragonPlacementEW);

    //display the game screen with updated position of Piggy
    SDL_RenderPresent(renderer);
    SDL_Delay(5);  //delay a bit
}

//Note:  west = 0, east = 1, south = 2, north = 3
void moveUp()
{
    //y's decrease to move north
    //if we are not facing north
    if (piggyDirection != 3)
    {
        piggyDirection = 3;    //then face north
        FrameCounter = 0;       //start new position
    }
}

void moveDown()
{
    //y's increase to move south
    //if we are not facing south
    if (piggyDirection != 2)
    {
        piggyDirection = 2;     //then face south
        FrameCounter = 0;        //start new position
    }
}

void moveLeft()
{
    //x's increase to move west
    //if we are not facing west
    if (piggyDirection != 0)
    {
        piggyDirection = 0;    //then face west
        FrameCounter = 0;       //start new position
    }
}

void moveRight()
{
    //x's decrease to move east
    //if we are not facing east
    if (piggyDirection != 1)
    {
        piggyDirection = 1;    //then face east
        FrameCounter = 0;       //start new position
    }
}

void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &xPos, int &yPos )
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
                xPos = xPos + 1;    //stop by increasing x (move back right)
            }
            else                        //else colliding on the left
            {
                xPos = xPos - 1;    //stop by decreasing x (move back left)
            }
        }
        else                            //else moving up or down
        {
            if(intersectsTop)           //if colliding with top
            {
                yPos = yPos - 1;    //stop by decreasing y (move back up)
            }
            else                        //else colliding with bottom
            {
                yPos = yPos + 1;    //stop by increasing y (move back down)
            }
        }//end: back off, bub.
}//end Collide

void PlayerTileCollision(SDL_Rect movingRect, int &xPos, int &yPos)
{
    //loop indexes to cycle through solid tile vector array
    int begIndex = 0;
    //int endIndex = solidTiles.size();
    int endIndex = numSolidTiles;

    //make the moving rectangle a constant
    const SDL_Rect& moving = movingRect;

    for (int i = begIndex; i <= endIndex; i++)
    {
        //Get the x and y location of solid tile
        int xDest = tiles[i].x;
        int yDest = tiles[i].y;

        //make the solid tile a rectangle
        FillRect(solidTileRect, xDest, yDest, tileWidth, tileHeight);

        //make the solid rectangle a constant
        const SDL_Rect& solid = solidTileRect;

        //now check to see if the moving rectangle (sprite) has intersected a solid tile
        if( SDL_HasIntersection(&moving, &solid))
        {
            //the moving sprite has collided with the solid tile so stop that sprite in its tracks
            Collide(movingRect, solidTileRect, xPos, yPos);
        }//end if intersected
    }//end for
}//end tile collision

bool EnemyTileCollision(SDL_Rect movingRect, int &direction)
{
    //loop indexes to cycle through solid tile vector array
    int i = 0;
    //int endIndex = solidTiles.size();

    //make the moving rectangle a constant
    const SDL_Rect& moving = movingRect;
    bool done = false;

        while (i <= numSolidTiles && done == false)
        {
            //Get the x and y location of solid tile
            int xDest = tiles[i].x;
            int yDest = tiles[i].y;

            //make the solid tile a rectangle
            FillRect(solidTileRect, xDest, yDest, tileWidth, tileHeight);

            //make the solid rectangle a constant
            const SDL_Rect& solid = solidTileRect;

            //now check to see if the moving rectangle (sprite) has intersected a solid tile
            if( SDL_HasIntersection(&moving, &solid))
            {
                //the moving sprite has collided with the solid tile so stop that sprite in its tracks
                enemyCollide(movingRect, solidTileRect, direction);
                done = true;       //done checking solid tiles as soon as 1 collision occurs
            }//end if intersected
            i++;
        }//end while
        if (done == true)
            return true;
        else
            return false;
}//end tile collision

void enemyCollide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &direction)
{
    {
        //now determine what side of the obstacle did the movingRect collide with?
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

        // adjust moving object's position accordingly
        if(moveInXDirection == true)            //if moving left or right
        {
            if(intersectsRight == true)         //if colliding on the right
            {
                direction = 1;    //change direction to left
            }
            //else
            if(intersectsRight == false)
                //else colliding on the left
            {
                direction = 0;    //change direction to right
            }
        }
        //else                            //else moving up or down
        if (moveInXDirection == false)
        {
            if(intersectsTop == true)           //if colliding with top
            {
                direction = 3;    //then change direction to up
            }
            //else                        //else colliding with bottom
            if(intersectsTop == false)
            {
                direction = 2;    //then change direction to down
            }
        }//end adjustment of direction
     }//end check if there is a collision
}//end Collide

void playerEnemyCollide(SDL_Rect playerRect, SDL_Rect enemyRect)
{
    //make the moving rectangle a constant
    const SDL_Rect& PLAYER_RECT = playerRect;
    //make the solid rectangle a constant
    const SDL_Rect& ENEMY_RECT = enemyRect;
    //create a constant rectangle with current zombie position for comparing
    const SDL_Rect& CONST_ZOMBIE_RECT = rectZombiePlacementEW;
    //create a constant rectangle with current farmer position for comparing
    const SDL_Rect& CONST_FARMER_RECT = rectFarmerPlacementEW;
    //create a constant rectangle with current dragon position for comparing
    const SDL_Rect& CONST_DRAGON_RECT = rectDragonPlacementEW;

   // if( SDL_HasIntersection(&PLAYER_RECT, &ENEMY_RECT))
   if (CheckCollision(playerRect, enemyRect))
    {
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_ZOMBIE_RECT))
        {
            zombieDestroyed = true;
            explosion(ENEMY_RECT.x, CONST_ZOMBIE_RECT.y);
            destroyZombie();
        }
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_FARMER_RECT))
        {
            farmerDestroyed = true;
            destroyFarmer();
        }
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_DRAGON_RECT))
        {
            dragonDestroyed = true;
            destroyDragon();
        }
    }
}

bool CheckCollision(SDL_Rect r1,SDL_Rect r2)
{
    //check to see if rectangles overlap
    //if not, return false
    if(r1.x > r2.x+r2.w)
    {
      return false;
    }
    else if(r1.y > r2.y+r2.h)
    {
      return false;
    }
    else if(r1.x+r1.w < r2.x)
    {
      return false;
    }
    else if(r1.y+r1.h < r2.y)
    {
      return false;
    }
    else
    {
      return true;
    }
}

void explosion(int xEnemyPos, int yEnemyPos)
{
    rectExplosion.x = xEnemyPos;
    rectExplosion.y = yEnemyPos;
    SDL_RenderCopy(renderer, textureExplosionSheet, &rectExplosion, &rectExplosionPlacement);
    //Increment time to delay each frame
    FrameCounterExplosion++;
    //If delay time is greater than max amount
    if(FrameCounterExplosion > FRAME_DELAY)
    {
        FrameCounterExplosion = 0;      //start new delay time
        SpriteFrameExplosion++;         //advance to next frame
    }
    if(SpriteFrameExplosion > MAX_EXPLOSION_FRAME - 1) //if last frame is reached
        destroyExplosion();
    //Advance to next frame on sheet
    rectExplosion.x = SpriteFrameExplosion * rectExplosion.w;
    //create the rectangle on screen for farmer placement
    FillRect(rectExplosion, rectExplosion.x, rectExplosion.y, rectExplosion.w, rectExplosion.h);
}
