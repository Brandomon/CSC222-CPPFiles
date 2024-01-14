#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

/*******************************************************************
* Program Name: Single Player Pong
*
* Description: This program will allow a bitmap to move up and down
*       controlled by the user with the up and down arrow keys. Another
*       bitmap of a target will be automatically controlled to move away
*       from the ball while staying in the top or bottom half of the
*       game window. A ball bitmap will move around the screen, bouncing
*       off of all but the player's wall. The player starts with a score
*       of zero and three lives. The goal is to score three points by
*       hitting the target with the bouncing ball. If the ball passes the
*       player and hits the wall behind the player, the player looses a
*       life.
*
* PseudoCode:
* Initialize SDL
* Create window
* Create renderer to manipulate the window
* Load background surfaces
* Create textures to convert surfaces to
* Create rectangles
* Initialize Game constants
* Initialize Game Variables
* Function Prototypes
* Create all the game images
* While program is running
*   Get the key press from the keyboard
*       If the escape key was pressed
*           Close shop
*   Draw the screen
*   Update the game
*   End game if score equals 3
*   End game if lives equals 0
* End main
*******************************************************************/

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
SDL_Texture* textureTarget = NULL;

//Rectangles
SDL_Rect rectBackground;    //Background rectangle
SDL_Rect rectBall;          //Ball rectangle
SDL_Rect rectPlayer;        //SpaceShip rectangle
SDL_Rect rectTarget;       //SpaceShip rectangle

//Game constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;
const int BALL_WIDTH = 20;
const int BALL_HEIGHT = 20;

const int PLAYER_PADDLE_X = SCREEN_WIDTH - PADDLE_WIDTH*2;
const int TARGET_PADDLE_X = PADDLE_WIDTH;

const int FPS = 30;
const int FRAME_DELAY = 1000/FPS;

const int TARGET_SPEED = 15;
const int PLAYER_SPEED = 19;
const int BALL_MAX_SPEED = 13;

//Game Variables
//Scores
int PlayerScore;
int PlayerLife;

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
void UpdateAI();
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
        return 1;
    }

    //Create all the game images
    CreateGameImages();

    while(ProgramIsRunning())
    {
        //Get the key press from the keyboard
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        //if the escape key was pressed
        if(keys[SDL_SCANCODE_ESCAPE])
        {
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tEscape key was used to exit\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            CloseShop();
            return 1;
        }

        long int oldTime = SDL_GetTicks();  //We will use this later to see how long it took to update the frame
        DrawGame();                         //Draw the screen
        RunGame();                          //Update the game

        int frameTime = SDL_GetTicks() - oldTime;

        if(frameTime < FRAME_DELAY)                 //Dont delay if we dont need to
           SDL_Delay(FRAME_DELAY - frameTime);      //Delay

        //End game if score equals 3
        if(PlayerScore == 3)
        {
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t3 points! Congratulations, you won!\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            CloseShop();
            return 1;
        }

        //End game if lives equals 0
        if(PlayerLife == 0)
        {
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tOut of lives! Better luck next time!\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            CloseShop();
            return 1;
        }

    }//end game loop

    CloseShop();    //Gracefully release SDL and its resources.
    return 1;       //Because C++ says so

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
    SDL_DestroyTexture(textureTarget);
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
    return true;
}

void ResetGame()
{
    //Position the player's paddle
    rectPlayer.x = PLAYER_PADDLE_X;
    rectPlayer.y = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    rectPlayer.w = PADDLE_WIDTH;
    rectPlayer.h = PADDLE_HEIGHT;

    //Position the target paddle in a random y location within the boundary
    rectTarget.x = TARGET_PADDLE_X;
    rectTarget.y = rand()%(SCREEN_HEIGHT - PADDLE_HEIGHT);
    rectTarget.w = PADDLE_WIDTH;
    rectTarget.h = PADDLE_HEIGHT;

    //Position the ball
    rectBall.x = rand()%(SCREEN_WIDTH / 2 - BALL_WIDTH);
    rectBall.y = rand()%(SCREEN_HEIGHT - BALL_HEIGHT);
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
    //Set score to 0 and lives to 3
    PlayerScore = 0;
    PlayerLife = 3;

    return true;
}

void UpdateBall()
{
    rectBall.x += xDirection;
    rectBall.y += yDirection;

    //If the ball hits the player, make it bounce to the left
    if(RectsOverlap(rectBall, rectPlayer))
    {
        xDirection = (rand()%BALL_MAX_SPEED + 8) * -1;
    }
    //If the ball hits the target, player scores, reset game
    if(RectsOverlap(rectBall, rectTarget))
    {
        PlayerScore++;
        ResetGame();
    }
    //Make sure the ball doesn't leave the screen
    if(rectBall.y < 0)
    {
        rectBall.y = 0;
        yDirection = rand()%BALL_MAX_SPEED + 8;
    }
    if(rectBall.y > SCREEN_HEIGHT - rectBall.h)
    {
        rectBall.y = SCREEN_HEIGHT - rectBall.h;
        yDirection = (rand()%BALL_MAX_SPEED + 8) * -1;
    }
    //Player looses life when ball goes off right side
    if(rectBall.x > SCREEN_WIDTH)
    {
        PlayerLife--;
        ResetGame();
    }
    //If the ball hits the left side, make it bounce to the right
    if(rectBall.x <= 0)
    {
        xDirection = rand()%BALL_MAX_SPEED + 8;
    }
}

void RunGame()
{
    UpdatePlayer();     //Update the player's paddle
    UpdateAI();         //Update the target's position
    UpdateBall();       //Update position of ball
}

void DrawGame()
{
    //set up a buffer to store window caption in
    char scores[64];

    //store the updated scores in the buffer
    sprintf(scores, "Score: %d - Lives: %d", PlayerScore, PlayerLife);
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

    //Copy Target paddle
    SDL_RenderCopy(renderer, textureTarget, NULL, &rectTarget);

    //display the game screen with updated position of ball and paddles
    SDL_RenderPresent(renderer);
    SDL_Delay(10);  //delay a bit
}

void CreateGameImages()
{
   //Rectangle settings for player and Target paddles
    int xPlayer;
    int yPlayer;
    int wPlayer;
    int hPlayer;
    int xTarget;
    int yTarget;
    int wTarget;
    int hTarget;

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

    //Initialize rectangle fields for ball in random location of left side
    rectBall.x = rand()%(SCREEN_WIDTH / 2 - BALL_WIDTH);
    rectBall.y = rand()%(SCREEN_HEIGHT - BALL_HEIGHT);
    rectBall.w = BALL_WIDTH;
    rectBall.h = BALL_HEIGHT;

    // Create a rectangle at position 0, 0 for Ball
    FillRect(rectBall, rectBall.x, rectBall.y, rectBall.w, rectBall.h);

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

    //Load Target Paddle bitmap
    tempSurface = loadImage("graphics/target.bmp");

    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 255, 0, 255);

    //Create texture
    textureTarget = loadTexture(tempSurface);

    //Position the Target
    xTarget = TARGET_PADDLE_X;
    yTarget = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;
    wTarget = PADDLE_WIDTH;
    hTarget = PADDLE_HEIGHT;

    // Create a rectangle at position 0, 0 for Target
    FillRect(rectTarget, xTarget, yTarget, wTarget, hTarget);
}

/**********************************************************
* Function Name : UpdatePlayer
* Description : Reads the keyboard input for the up and down
*               arrow keys to move the player's paddle and makes
*               sure the paddle doesn't move off the screen.
* Initial Algorithm :
*   Move the paddle when the up/down keys are pressed
*   IF up arrow key is pressed
*       Move the paddle up
*   IF down arrow key is pressed
*       Move the paddle down
*   Make sure the paddle doesn't leave the screen
*   IF player paddle y < 0
*       Player paddle y = 0
*   IF player paddle y > SCREEN_HEIGHT - player height
*       Player paddle y = SCREEN_HEIGHT - player height
***********************************************************/

void UpdatePlayer()
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    //Move the paddle when the up/down keys are pressed
   if(keys[SDL_SCANCODE_UP])
        rectPlayer.y -= PLAYER_SPEED;

   if(keys[SDL_SCANCODE_DOWN])
        rectPlayer.y += PLAYER_SPEED;

    //Make sure the paddle doesn't leave the screen
    if(rectPlayer.y < 0)
        rectPlayer.y = 0;

    if(rectPlayer.y > SCREEN_HEIGHT-rectPlayer.h)
        rectPlayer.y = SCREEN_HEIGHT-rectPlayer.h;
}

/**********************************************************
* Function Name : UpdateAI
* Description : Keeps the target within the top or bottom
*               half of the screen while automatically
*               moving the target away from the ball.
* Initial Algorithm :
*   If the target spawns in the top half of the screen
*       If the target's center higher than the ball's center, move the target down
*       If the target's center lower than the ball's center, move the target up
*       Make sure the target doesn't leave the top half of the screen
*   If the target spawns in the bottom half of the screen
*       If the target's center higher than the ball's center, move the target down
*       If the target's center lower than the ball's center, move the target up
*       Make sure the target doesn't leave the bottom half of the screen
***********************************************************/
void UpdateAI()
{
    //If the target spawns in the top half of the screen
    if (rectTarget.y + ( rectTarget.h / 2) <= SCREEN_HEIGHT / 2)
    {
        //If the target's center higher than the ball's center, move the target down
        if((rectTarget.y + rectTarget.h/2) > (rectBall.y+rectBall.h/2))
            rectTarget.y += TARGET_SPEED;

        //If the target's center lower than the ball's center, move the target up
        if((rectTarget.y + rectTarget.h/2) < (rectBall.y+rectBall.h/2))
            rectTarget.y -= TARGET_SPEED;

        //Make sure the target doesn't leave the top half of the screen
        if(rectTarget.y < 0)
            rectTarget.y = 0;

        if(rectTarget.y > SCREEN_HEIGHT / 2 - rectTarget.h)
            rectTarget.y = SCREEN_HEIGHT / 2 - rectTarget.h;
    }

    //If the target spawns in the bottom half of the screen
    if (rectTarget.y + (rectTarget.h / 2) > SCREEN_HEIGHT / 2)
    {
        //If the target's center higher than the ball's center, move the target down
        if((rectTarget.y + rectTarget.h/2) > (rectBall.y+rectBall.h/2))
            rectTarget.y += TARGET_SPEED;

        //If the target's center lower than the ball's center, move the target up
        if((rectTarget.y + rectTarget.h/2) < (rectBall.y+rectBall.h/2))
            rectTarget.y -= TARGET_SPEED;

        //Make sure the target doesn't leave the bottom half of the screen
        if(rectTarget.y < SCREEN_HEIGHT / 2)
            rectTarget.y = SCREEN_HEIGHT / 2;

        if(rectTarget.y > SCREEN_HEIGHT - rectTarget.h)
            rectTarget.y = SCREEN_HEIGHT - rectTarget.h;
    }
}
