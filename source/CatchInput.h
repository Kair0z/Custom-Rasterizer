#pragma once
#include "Utils.h"
#include <iostream>

enum class Action
{
	PrintInput		= SDLK_p,

	ToggleMaterial	= SDLK_m,
	ToggleViewtype	= SDLK_v,

	ToggleScene		= SDLK_u,
	ToggleCamera	= SDLK_c,
	ResetCamera		= SDLK_w,

	MoveCamDown		= SDLK_DOWN,
	MoveCamUp		= SDLK_UP,
	MoveCamRight	= SDLK_RIGHT,
	MoveCamLeft		= SDLK_LEFT,

	END
};

namespace CatchInput
{
	static void PrintInputCommands();
}

void CatchInput::PrintInputCommands()
{
	Utils::ChangeConsoleTextColor(Utils::TextAttribute::Blue);
	std::cout << "- P: Print Input-commands \n";

	Utils::ChangeConsoleTextColor(Utils::TextAttribute::Cream);
	std::cout << "- M: Toggle current material \n";
	std::cout << "- V: Toggle viewtype [Materials - Triangledata - Depth - Normals - Boundingbox] \n";
	std::cout << "- U: Toggle between created Scenes \n";
	std::cout << "- C: Toggle between created cameras in active scene \n";
	std::cout << "- W: Reset the position & rotation of current camera \n";

	Utils::ChangeConsoleTextColor(Utils::TextAttribute::LightRed);
	std::cout << "- LEFT-RIGHT: Move camera horizontally \n";
	std::cout << "- UP-DOWN: Move Camera vertically \n";

	Utils::ChangeConsoleTextColor(Utils::TextAttribute::White);
}