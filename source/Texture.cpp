#include "pch.h"
#include "Texture.h"
#include <SDL_image.h>
#include <cassert>
#include <iostream>
#include "Utils.h"

Texture::Texture(const std::string& filepath)
	: m_pSurface{ IMG_Load(filepath.c_str()) }
{
	if (!m_pSurface) std::cout << "IMG load error: " << IMG_GetError() << "\n";
}

void Texture::Sample(const Elite::FVector2& uv, Elite::RGBColor& return_Color, float& return_Alpha) const
{
	if (!m_pSurface)
	{
		std::cout << "Invalid surface created! \n";
		return;
	}

	float x{uv.x};
	float y{uv.y}; // uv-coordinate axes are weird!

	//assert(x <= 1.f && y <= 1.f);

	if (x > 1.f || y > 1.f ||x < 0.f || y < 0.f) return;

	// Convert uv coordinates from [0-1] --> [tex_Width-tex_Height]
	x *= m_pSurface->w;
	y *= m_pSurface->h;

	uint8_t r{};
	uint8_t g{};
	uint8_t b{};
	uint8_t a{};

	// Sample color:
	SDL_GetRGBA(Utils::GetPixel(m_pSurface, int(x), int(y)), m_pSurface->format, &r, &g, &b, &a);

	Elite::RGBColor temp{float(r), float(g), float(b)};
	float alpha = float(a);

	assert(r <= 255 && g <= 255 && b <= 255);

	//std::cout << "finalColor = " << temp.r << " " << temp.g << " " << temp.b << "\n";

	// Remap color to [0-1]
	temp /= 255.f;
	alpha /= 255.f;

	return_Color = temp;
	return_Alpha = alpha;
}

Texture::~Texture()
{
	SDL_FreeSurface(m_pSurface);
}

