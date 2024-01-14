
#include <SDL.h>
#include <cstdio>
int main(int argc, char* args[])
{
      SDL_Init(SDL_INIT_EVERYTHING);
      printf("\"Hello World!!\"\n"
             "-Brandon LaPointe\n");
      SDL_Quit();
      return 0;
}
