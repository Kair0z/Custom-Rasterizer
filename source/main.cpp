#include "pch.h"
//#undef main

#include <vld.h>

//Standard includes
#include <iostream>
#include <string>
#include <thread>

//Project includes
#include "Master.h"

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
}

int main(int argc, char* args[])
{
	srand(unsigned int(time(nullptr)));

	//Unreferenced parameters
	(void)argc;
	(void)args;

	SDL_Init(SDL_INIT_VIDEO);

	Master master{};

	SDL_Event e;

	while (!master.ShouldTerminate())
	{
		// Loop Master
		master.Loop();

		// Poll next event from queue
		while (SDL_PollEvent(&e))
		{
			// Handle the polled event
			switch (e.type)
			{
			case SDL_QUIT:
				master.Terminate();
				break;
			case SDL_KEYDOWN:
				master.ProcessKeyDownEvent(e.key);
				break;
			case SDL_KEYUP:
				master.ProcessKeyUpEvent(e.key);
				break;
			case SDL_MOUSEMOTION:
				master.ProcessMouseMotionEvent(e.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
				master.ProcessMouseDownEvent(e.button);
				break;
			case SDL_MOUSEBUTTONUP:
				master.ProcessMouseUpEvent(e.button);
				break;
			}
		}
	}

	SDL_Quit();

	return 0;
}