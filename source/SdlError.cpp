#include "SdlError.h"
#include "SDL.h"
#include <iostream>
#include <string>

void SdlError::Die(std::string msg)
{
    std::cout << msg << " " << SDL_GetError() << std::flush;
    SDL_Quit();
    exit(1);
}

void SdlError::Check(int line)
{
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
}