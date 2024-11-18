#ifndef SDL_ERROR_H
#define SDL_ERROR_H

#include <string>

namespace SdlError
{
	void Die(std::string msg);
	void Check(int line);
}

#endif