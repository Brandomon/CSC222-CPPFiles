#include <SDL.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>    //use the computer's clock

//Window
SDL_Window* gameWindow = NULL;
//Renderer
SDL_Renderer* renderer = NULL;
//Rectangle
SDL_Rect rectangle;

//Function Prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Failed to initialize SDL!\n");
        return 0;
    }
    //Create a window
    gameWindow = SDL_CreateWindow(
    "Draw a Rectangle for me!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    //Set up the renderer for the game window
    renderer =  SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);

    // Create a 32 x 48 rectangle at position 50, 50
    FillRect(rectangle, 50, 50, 32, 48);

    //Seed the random number generator off the clock
    srand(static_cast<unsigned int>(time(0)));

    //Game loop
    while(ProgramIsRunning())
    {
        // Set render color to orange ( background will be rendered in this color )
        SDL_SetRenderDrawColor(renderer, 200, 100, 0, 255);
        // Clear winow
        SDL_RenderClear(renderer);
        // Set render color to purple ( rectangle will be rendered in this color )
        SDL_SetRenderDrawColor( renderer, 100, 0, 255, 255 );
        // Render rectangle
        SDL_RenderFillRect( renderer, &rectangle);
        // Render everything to the screen
        SDL_RenderPresent(renderer);
        //Generate a new random position for the rectangle
        rectangle.x = rand()%800;     //select an x coordinate between 0 and 800
        rectangle.y = rand()%600;     //select a y coordinate between 0 and 600
        // Wait for 5 sec
        SDL_Delay(1000);
    }//end while

    //Time to clean up and go home
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();

    return 0;
}//end main

//Function Section
bool ProgramIsRunning()
{
    SDL_Event event;            //Input from User
    bool running = true;        //Game loop sentinel

    while(SDL_PollEvent(&event))    //While no input
    {
        if(event.type == SDL_QUIT)  //If the User clicks the X in game window
            running = false;        //Stop by setting game loop sentinel to false
    }
    return running;
}//end program running

void FillRect(SDL_Rect &rect, int x, int y, int width, int height)
{
    //Initialize the rectangle
    rect.x = x;         //initial x position of upper left corner
    rect.y = y;         //initial y position of upper left corner
    rect.w = width;     //width of rectangle
    rect.h = height;    //height of rectangle
}//end initialize rectangle
