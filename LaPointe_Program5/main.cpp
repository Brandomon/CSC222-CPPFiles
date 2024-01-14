#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

//Window
SDL_Window* gameWindow = NULL;
//Renderer
SDL_Renderer* renderer;

//Surfaces
SDL_Surface* tempSurface;

//Textures

//Background
SDL_Texture* textureBackground = NULL;

//Sprites
SDL_Texture* textureDragonEastSheet = NULL;     //sheet of east dragons texture
SDL_Texture* textureDragonWestSheet = NULL;     //sheet of west dragons texture
SDL_Texture* textureDragonNorthSheet = NULL;    //sheet of north dragons texture
SDL_Texture* textureDragonSouthSheet = NULL;    //sheet of south dragons texture
SDL_Texture* textureDragonSheet = NULL;         //generic sheet of dragons texture

//Obstacles
SDL_Texture* textureCloud = NULL;
SDL_Texture* textureCloud2 = NULL;
SDL_Texture* textureCloud3 = NULL;
SDL_Texture* textureCloud4 = NULL;

//Rectangles

//Background
SDL_Rect rectBackground;        //Background rectangle

//Sprites
SDL_Rect rectDragonEastSheet;   //Rectangle for sheet of east walking dragons
SDL_Rect rectDragonEast;        //One east walking dragon rectangle
SDL_Rect rectDragonWestSheet;   //Rectangle for sheet of west walking dragons
SDL_Rect rectDragonWest;        //One west walking dragon rectangle
SDL_Rect rectDragonNorthSheet;  //Rectangle for sheet of north walking dragons
SDL_Rect rectDragonNorth;       //One north walking dragon rectangle
SDL_Rect rectDragonSouthSheet;  //Rectangle for sheet of south walking dragons
SDL_Rect rectDragonSouth;       //One south walking dragon rectangle

SDL_Rect rectDragonPlacement;   //Where to place the dragon on the screen
SDL_Rect rectDragon;            //Rectangle containing dragon placed on screen
SDL_Rect rectDragonSheet;       //Sheet containing dragons for currect direction

//Obstacles
SDL_Rect rectCloud;
SDL_Rect rectCloud2;
SDL_Rect rectCloud3;
SDL_Rect rectCloud4;

//Music
Mix_Music *GameMusic = NULL;

//Game constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int DRAGON_WIDTH = 128;
const int DRAGON_HEIGHT = 128;

const int FPS = 10;
const int FRAME_DELAY = 95/FPS;

const int DRAGON_SPEED = 2;
const int MAX_DRAGON_FRAME = 8;  //number of dragons per sheet

const int CLOUD_WIDTH = 40;
const int CLOUD_HEIGHT = 30;

const int CLOUD2_WIDTH = 70;
const int CLOUD2_HEIGHT = 30;

const int CLOUD3_WIDTH = 90;
const int CLOUD3_HEIGHT = 30;

const int CLOUD4_WIDTH = 110;
const int CLOUD4_HEIGHT = 30;

//Game Variables we need to be global
int SpriteFrame = 0;
int FrameCounter = 0;
int width;                      //width of one frame
int height;                     //height of one frame
int x;                          //x position of dragon on screen
int y;                          //y position of dragon on screen

//Note:  west = 0, east = 1, south = 2, north = 3
int dragonDirection;

//Function Prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();SDL_Surface* LoadImage(char* fileName);
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
bool ProgramIsRunning();
bool RectsOverlapBottomLeft(SDL_Rect rect1, SDL_Rect rect2);
bool RectsOverlapTopRight(SDL_Rect rect1, SDL_Rect rect2);
bool InitSDL();
void ResetGame();
bool InitGame();
void UpdateDragon();
void RunGame();
void DrawGame();
void CreateGameImages();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();
bool CheckCollision( SDL_Rect a, SDL_Rect b );
void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect );


int main(int argc, char *argv[])
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if(!InitGame())
    {
        printf("Unable to initialize from main.");
        CloseShop();   //If InitGame failed, kill the program
    }

    //Create all the game images
    CreateGameImages();

    //Load background music
    //GameMusic = Mix_LoadMUS("audio/zombieTunes.wav");       //Load background music
    //Error check music
    //if(GameMusic == NULL)
       //printf("Game music did not load.");
    //Mix_PlayMusic(GameMusic, -1);                           //Play Background music

    while(ProgramIsRunning())
    {
        //CHECK INPUT
        //Move the dragon when the up/down/left right key is pressed
        if(keys[SDL_SCANCODE_UP])
            //move the shape north
            moveUp();

        if(keys[SDL_SCANCODE_DOWN])
            //move shape south
            moveDown();

        if(keys[SDL_SCANCODE_LEFT])
            //move shape east
            moveLeft();

        if(keys[SDL_SCANCODE_RIGHT])
        {
            //move shape west
            moveRight();
        }
        DrawGame();
        RunGame();          //Update & Draw
    }//end game loop
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\tOur fears are like dragons guarding our deepest treasures.\n\n\n\n\n\n\n\n\n\n\n\n\n");
    CloseShop();    //Gracefully release SDL and its resources.
    return 0;       //Because C++ says so
}//end main

//Functions
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
    SDL_DestroyTexture(textureDragonSouthSheet);
    SDL_DestroyTexture(textureDragonNorthSheet);
    SDL_DestroyTexture(textureDragonEastSheet);
    SDL_DestroyTexture(textureDragonWestSheet);
    SDL_DestroyTexture(textureDragonSheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
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

bool InitGame()
{
    //Init SDL
    if(!InitSDL())
        return false;

    //Initiatialize game variables

    return true;
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
        SpriteFrame = 0;                 //start at frame zero again

    //first check to see if the dragon has collided with the clouds
    Collide(rectDragonPlacement, rectCloud);
    Collide(rectDragonPlacement, rectCloud2);
    Collide(rectDragonPlacement, rectCloud3);
    Collide(rectDragonPlacement, rectCloud4);

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (dragonDirection == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectDragonEast.x = SpriteFrame * rectDragonEast.w;
        x = x + DRAGON_SPEED;   //increment x

        if ( x > SCREEN_WIDTH - DRAGON_WIDTH)       //check right
        //if edge of bitmap collides with screen
        {
            x = SCREEN_WIDTH - DRAGON_WIDTH;        //stop and kiss the side
        }
        //create the rectangle on screen for dragon placement
        FillRect(rectDragon, rectDragonEast.x, rectDragonEast.y, rectDragonEast.w, rectDragonEast.h);
        textureDragonSheet = textureDragonEastSheet;
    }// end walking east

    if (dragonDirection == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectDragonWest.x = SpriteFrame * rectDragonWest.w;
        x = x - DRAGON_SPEED;                       //decrement x
        if ( x < 0 )                                //check left
        //if edge of bitmap collides with screen
        {
            x = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for dragon placement
        FillRect(rectDragon, rectDragonWest.x, rectDragonWest.y, rectDragonWest.w, rectDragonWest.h);
        textureDragonSheet = textureDragonWestSheet;
    }// end walking West

    if (dragonDirection == 2)                       //if we are going south
    {
        //Advance to next frame on sheet
        rectDragonSouth.x = SpriteFrame * rectDragonSouth.w;
        y = y + DRAGON_SPEED;                       //increment y
        if ( y > SCREEN_HEIGHT - DRAGON_HEIGHT )    //check bottom
        {
            y = SCREEN_HEIGHT - DRAGON_HEIGHT;      //stop and kiss the side
        }
        //create the rectangle on screen for dragon placement
        FillRect(rectDragon, rectDragonSouth.x, rectDragonSouth.y, rectDragonSouth.w, rectDragonSouth.h);
        //copy the south facing dragons to the generic sheet
        textureDragonSheet = textureDragonSouthSheet;
    }// end walking south

    if (dragonDirection == 3)                       //if we are going north
    {
        //Advance to next frame on sheet
        rectDragonNorth.x = SpriteFrame * rectDragonNorth.w;
        y = y - DRAGON_SPEED;                       //decrement y
        if ( y < 0 )                                //check top
        //if edge of bitmap collides with screen
        {
            y = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for dragon placement
        FillRect(rectDragon, rectDragonNorth.x, rectDragonNorth.y, rectDragonNorth.w, rectDragonNorth.h);
        //copy the north facing dragon to the generic dragon sheet
        textureDragonSheet = textureDragonNorthSheet;
    }// end walking north

    //Load the dragon facing the direction indicated by the arrow keys
    FillRect(rectDragonPlacement, x, y, width, height);
}

void RunGame()
{
    UpdateDragon();   //Update position of dragon
}

void DrawGame()
{
    //Clear the window
    SDL_RenderClear(renderer);
    //Copy the background)
    SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);
    //Copy the dragon
    SDL_RenderCopy(renderer, textureDragonSheet, &rectDragon, &rectDragonPlacement);
    //Copy the clouds
    SDL_RenderCopy(renderer, textureCloud, NULL, &rectCloud);
    SDL_RenderCopy(renderer, textureCloud2, NULL, &rectCloud2);
    SDL_RenderCopy(renderer, textureCloud3, NULL, &rectCloud3);
    SDL_RenderCopy(renderer, textureCloud4, NULL, &rectCloud4);
    //display the game screen with updated position of Dragon
    SDL_RenderPresent(renderer);
    SDL_Delay(5);  //delay a bit
}

void CreateGameImages()
{
    //Create a window
    gameWindow = SDL_CreateWindow(
    "Dragons are a flyin'!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

     //load background
    tempSurface = loadImage("graphics/castle.bmp");
    //create texture
    textureBackground = loadTexture(tempSurface);
    // Create a rectangle at position 0, 0 for background
    FillRect(rectBackground, 0, 0, 800, 600);

    //load cloud
    tempSurface = loadImage("graphics/cloud.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);
    //create texture
    textureCloud = loadTexture(tempSurface);
    // Create a rectangle at position 0, 0 for background
    FillRect(rectCloud, 270, 150, CLOUD_WIDTH, CLOUD_HEIGHT);

    //load cloud2
    tempSurface = loadImage("graphics/cloud2.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);
    //create texture
    textureCloud2 = loadTexture(tempSurface);
    // Create a rectangle at position 0, 0 for background
    FillRect(rectCloud2, 600, 130, CLOUD2_WIDTH, CLOUD2_HEIGHT);

    //load cloud3
    tempSurface = loadImage("graphics/cloud3.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);
    //create texture
    textureCloud3 = loadTexture(tempSurface);
    // Create a rectangle at position 0, 0 for background
    FillRect(rectCloud3, 130, 440, CLOUD3_WIDTH, CLOUD3_HEIGHT);

    //load cloud4
    tempSurface = loadImage("graphics/cloud4.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);
    //create texture
    textureCloud4 = loadTexture(tempSurface);
    // Create a rectangle at position 0, 0 for background
    FillRect(rectCloud4, 540, 400, CLOUD4_WIDTH, CLOUD4_HEIGHT);

    //Set up East walking dragons
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/flyEast.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of dragons
    SDL_QueryTexture(textureDragonEastSheet, NULL, NULL, &rectDragonEastSheet.w, &rectDragonEastSheet.h);
    //get the width of one frame (one Dragon) by dividing the width of the sheet by the number of frames
    width = rectDragonEastSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    height = rectDragonEastSheet.h;  //height on one Dragon is same as height of sheet
    //Initialize the rectangle for one Dragon
    FillRect(rectDragonEast, 0, 0, width, height);

    //Set up West walking dragons
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/flyWest.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of dragons
    SDL_QueryTexture(textureDragonWestSheet, NULL, NULL, &rectDragonWestSheet.w, &rectDragonWestSheet.h);
    //get the width of one frame (one Dragon) by dividing the width of the sheet by the number of frames
    width = rectDragonWestSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    height = rectDragonWestSheet.h;  //height on one dragon is same as height of sheet
    //Initialize the rectangle for one dragon
    FillRect(rectDragonWest, 0, 0, width, height);

    //Set up North walking dragons
    //create the surface - load the north sheet
    tempSurface = loadImage("graphics/flyNorth.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonNorthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of dragons
    SDL_QueryTexture(textureDragonNorthSheet, NULL, NULL, &rectDragonNorthSheet.w, &rectDragonNorthSheet.h);
    //get the width of one frame (one Dragon) by dividing the width of the sheet by the number of frames
    width = rectDragonNorthSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    height = rectDragonNorthSheet.h;  //height on one dragon is same as height of sheet
    //Initialize the rectangle for one dragon
    FillRect(rectDragonNorth, 0, 0, width, height);

    //Set up South walking dragons
    //create the surface - load the south sheet
    tempSurface = loadImage("graphics/flySouth.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 255, 255);
    //create the texture for the whole sheet
    textureDragonSouthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one dragon walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of dragons
    SDL_QueryTexture(textureDragonSouthSheet, NULL, NULL, &rectDragonSouthSheet.w, &rectDragonSouthSheet.h);
    //get the width of one frame (one Dragon) by dividing the width of the sheet by the number of frames
    width = rectDragonSouthSheet.w/MAX_DRAGON_FRAME;
    //get the height of one frame
    height = rectDragonSouthSheet.h;  //height on one dragon is same as height of sheet
    //Initialize the rectangle for one dragon
    FillRect(rectDragonSouth, 0, 0, width, height);
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
void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect )
{
    //collide returns a true if a collision occurred
    if(CheckCollision(movingRect, obstacleRect))
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
        if(moveInXDirection)            //if moving left or right
        {
            if(intersectsRight)         //if colliding on the right
            {
                dragonDirection = 1;    //change direction to left
            }
            else                        //else colliding on the left
            {
                dragonDirection = 0;    //change direction to right
            }
        }
        else                            //else moving up or down
        {
            if(intersectsTop)           //if colliding with top
            {
                dragonDirection = 3;    //then change direction to up
            }
            else                        //else colliding with bottom
            {
                dragonDirection = 2;    //then change direction to down
            }
        }//end adjustment of direction
     }//end check if there is a collision
}//end Collide
