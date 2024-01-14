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
SDL_Texture* textureBackground = NULL;
SDL_Texture* textureBall = NULL;
SDL_Texture* texturePlayer = NULL;
SDL_Texture* textureEnemy = NULL;

//Rectangles
SDL_Rect rectBackground;    //Background rectangle
SDL_Rect rectBall;          //Ball rectangle
SDL_Rect rectPlayer;        //SpaceShip rectangle
SDL_Rect rectPlayer2;       //SpaceShip rectangle

//Music
Mix_Music *GameMusic = NULL;

//Game constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;
const int BALL_WIDTH = 20;
const int BALL_HEIGHT = 20;

const int PLAYER_PADDLE_X = PADDLE_WIDTH;
const int PLAYER2_PADDLE_X = SCREEN_WIDTH - PADDLE_WIDTH*2;

const int FPS = 30;
const int FRAME_DELAY = 1000/FPS;

const int PLAYER_SPEED = 19;
const int PLAYER2_SPEED = 19;
const int BALL_MAX_SPEED = 15;

//Game Variables
//Scores
int PlayerScore;
int Player2Score;
//Incrementation speeds
int xDirection = 5;
int yDirection = 5;

//Function Prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();SDL_Surface* LoadImage(char* fileName);
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
bool ProgramIsRunning();
bool RectsOverlap(SDL_Rect rect1, SDL_Rect rect2);
bool InitSDL();
void ResetGame();
bool InitGame();
void UpdatePlayer();
void UpdateBall();
void RunGame();
void DrawGame();
void CreateGameImages();
void UpdatePlayer2();


int main(int argc, char *argv[])
{
    if(!InitGame())
    {
        CloseShop();   //If InitGame failed, kill the program
    }

    //Create all the game images
    CreateGameImages();

    //Load background music
    GameMusic = Mix_LoadMUS("audio/theHappySong.wav");     //Load background music
    //Error check music
    if(GameMusic == NULL)
       printf("Game music did not load.");
    Mix_PlayMusic(GameMusic, -1); //Play Background music

    while(ProgramIsRunning())
    {
        long int oldTime = SDL_GetTicks();  //We will use this later to see how long it took to update the frame
        DrawGame();                         //Draw the screen
        RunGame();                          //Update the game

        int frameTime = SDL_GetTicks() - oldTime;

        if(frameTime < FRAME_DELAY)                 //Dont delay if we dont need to
           SDL_Delay(FRAME_DELAY - frameTime);      //Delay
    }//end game loop
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
    SDL_DestroyTexture(textureBall);
    SDL_DestroyTexture(texturePlayer);
    SDL_DestroyTexture(textureEnemy);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    Mix_FreeMusic(GameMusic);
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

void ResetGame()
{
    //Position the player's paddle
    rectPlayer.x = PLAYER_PADDLE_X;
    rectPlayer.y = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    rectPlayer.w = PADDLE_WIDTH;
    rectPlayer.h = PADDLE_HEIGHT;

    //Position the enemy's paddle
    rectPlayer2.x = PLAYER2_PADDLE_X;
    rectPlayer2.y = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    rectPlayer2.w = PADDLE_WIDTH;
    rectPlayer2.h = PADDLE_HEIGHT;

    //Position the ball
    rectBall.x = SCREEN_WIDTH/2 - BALL_WIDTH/2;
    rectBall.y = SCREEN_HEIGHT/2 - BALL_HEIGHT/2;
    rectBall.w = BALL_WIDTH;
    rectBall.h = BALL_HEIGHT;

    //Reset x and y direction
    xDirection = 5;
    yDirection = 5;

    //Give it a 50% probability of going toward's the player
    if(rand()%2 == 0)
        xDirection *= -1;
}

bool InitGame()
{
    //Init SDL
    if(!InitSDL())
        return false;

    //Initiatialize game variables

    //Set scores to 0
    PlayerScore = 0;
    Player2Score = 0;

    return true;
}

void UpdateBall()
{
    rectBall.x += xDirection;
    rectBall.y += yDirection;

    //If the ball hits the player, make it bounce to the right
    if(RectsOverlap(rectBall, rectPlayer))
    {
        xDirection = rand()%BALL_MAX_SPEED + 5;
    }
    //If the ball hits the enemy, make it bounce to the left
    if(RectsOverlap(rectBall, rectPlayer2))
    {
        xDirection = (rand()%BALL_MAX_SPEED + 5) * -1;
    }
    //Make sure the ball doesn't leave the screen
    if(rectBall.y < 0)
    {
        rectBall.y = 0;
        yDirection = rand()%BALL_MAX_SPEED + 5;
    }
    if(rectBall.y > SCREEN_HEIGHT - rectBall.h)
    {
        rectBall.y = SCREEN_HEIGHT - rectBall.h;
        yDirection = (rand()%BALL_MAX_SPEED + 5) * -1;
    }
    //Player scores when ball goes off right side
    if(rectBall.x > SCREEN_WIDTH)
    {
        PlayerScore++;
        ResetGame();
    }
    //Enemy scores when ball goes off left side
    if(rectBall.x < 0-rectBall.h)
    {
        Player2Score++;
        ResetGame();
    }
}

void RunGame()
{
    UpdatePlayer();     //Update the players's paddle
    UpdatePlayer2();    //Update the enemy's paddle
    UpdateBall();       //Update position of ball
}

void DrawGame()
{
    //set up a buffer to store window caption in
    char scores[64];

    //store the updated scores in the buffer
    sprintf(scores, "Player 1 Score: %d ------- Player 2 Score: %d", PlayerScore, Player2Score);
    //print the scores in the game window caption
    SDL_SetWindowTitle(gameWindow, scores);

    //Clear the window
    SDL_RenderClear(renderer);
    //Copy the background);
    SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);
    //Copy ball
    SDL_RenderCopy(renderer, textureBall, NULL, &rectBall);
    //Copy player paddle
    SDL_RenderCopy(renderer, texturePlayer, NULL, &rectPlayer);
    //Copy enemy paddle
    SDL_RenderCopy(renderer, textureEnemy, NULL, &rectPlayer2);
    //display the game screen with updated position of ball and paddles
    SDL_RenderPresent(renderer);
    SDL_Delay(10);  //delay a bit
}

void CreateGameImages()
{
   //Rectangle settings for player and enemy paddles
    int xPlayer;
    int yPlayer;
    int wPlayer;
    int hPlayer;
    int xPlayer2;
    int yPlayer2;
    int wPlayer2;
    int hPlayer2;

    //Create a window
    gameWindow = SDL_CreateWindow(
    "Welcome to Paddle Ball!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);
     //load background
    tempSurface = loadImage("graphics/blackTexture.bmp");
    //create texture
    textureBackground = loadTexture(tempSurface);
    // Create a rectangle at position 0, 0 for background
    FillRect(rectBackground, 0, 0, 800, 600);
    //Load Ball bitmap
    tempSurface = loadImage("graphics/ball.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);
    //Create texture
    textureBall = loadTexture(tempSurface);
    // Create a rectangle at position 0, 0 for Ball
    FillRect(rectBall, 0, 0, 20, 20);

    //Load Player Paddle bitmap
    tempSurface = loadImage("graphics/player.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);
    //Create texture
    texturePlayer = loadTexture(tempSurface);
    //Initialize rectangle fields for player paddle
    xPlayer = PLAYER_PADDLE_X;
    yPlayer = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    wPlayer = PADDLE_WIDTH;
    hPlayer = PADDLE_HEIGHT;
    // Create a rectangle at position 0, 0 for Player
    FillRect(rectPlayer, xPlayer, yPlayer, wPlayer, hPlayer);

    //Load Enemy Paddle bitmap
    tempSurface = loadImage("graphics/enemy.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);
    //Create texture
    textureEnemy = loadTexture(tempSurface);
    //Position the enemy's paddle
    xPlayer2 = PLAYER2_PADDLE_X;
    yPlayer2 = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    wPlayer2 = PADDLE_WIDTH;
    hPlayer2 = PADDLE_HEIGHT;
    // Create a rectangle at position 0, 0 for Enemy
    FillRect(rectPlayer2, xPlayer2, yPlayer2, wPlayer2, hPlayer2);
}

/**********************************************************
* Function Name : UpdatePlayer
* Description : Reads the keyboard input for the W and S
*               arrow keys to move the first paddle and makes
*               sure the paddle doesn't move off the screen.
* Initial Algorithm :
*   Move the paddle when the W/S keys are pressed
*   IF W arrow key is pressed
*       Move the paddle up
*   IF S arrow key is pressed
*       Move the paddle down
*   Make sure the paddle doesn't leave the screen
*   IF player1 paddle y < 0
*       Player1 paddle y = 0
*   IF player1 paddle y > SCREEN_HEIGHT - player height
*       Player1 paddle y = SCREEN_HEIGHT - player height
***********************************************************/

void UpdatePlayer()
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    //Move the paddle when the W/S keys are pressed
   if(keys[SDL_SCANCODE_W])
        rectPlayer.y -= PLAYER_SPEED;

   if(keys[SDL_SCANCODE_S])
        rectPlayer.y += PLAYER_SPEED;

    //Make sure the paddle doesn't leave the screen
    if(rectPlayer.y < 0)
        rectPlayer.y = 0;

    if(rectPlayer.y > SCREEN_HEIGHT-rectPlayer.h)
        rectPlayer.y = SCREEN_HEIGHT-rectPlayer.h;
}

/**********************************************************
* Function Name : UpdatePlayer2
* Description : Reads the keyboard input for the up and down
*               arrow keys to move the second paddle and makes
*               sure the paddle doesn't move off the screen.
* Initial Algorithm :
*   Move the paddle when the up/down keys are pressed
*   IF up arrow key is pressed
*       Move the paddle up
*   IF down arrow key is pressed
*       Move the paddle down
*   Make sure the paddle doesn't leave the screen
*   IF player2 paddle y < 0
*       Player2 paddle y = 0
*   IF player2 paddle y > SCREEN_HEIGHT - player2 height
*       Player2 paddle y = SCREEN_HEIGHT - player2 height
***********************************************************/

void UpdatePlayer2()
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    //Move the paddle when the up/down keys are pressed
   if(keys[SDL_SCANCODE_UP])
        rectPlayer2.y -= PLAYER2_SPEED;

   if(keys[SDL_SCANCODE_DOWN])
        rectPlayer2.y += PLAYER2_SPEED;

    //Make sure the paddle doesn't leave the screen
    if(rectPlayer2.y < 0)
        rectPlayer2.y = 0;

    if(rectPlayer2.y > SCREEN_HEIGHT-rectPlayer2.h)
        rectPlayer2.y = SCREEN_HEIGHT-rectPlayer2.h;
}

