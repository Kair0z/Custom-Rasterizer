#pragma once
#include <string>
#include "ERGBColor.h"
#include "EMath.h"

struct SDL_Surface;

class Texture
{
public:
	Texture(const std::string& filepath);
	void Sample(const Elite::FVector2& uv, Elite::RGBColor& return_Color, float& return_Alpha) const;

	~Texture();

private:
	SDL_Surface* m_pSurface;
};



