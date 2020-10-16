#include "pch.h"
#include "Map.h"
#include "Utils.h"

#include "SDL_image.h"

// ---------------------------------------------------
// Normal-Map: Sampling returns a vector in world space (using Tangent & face-normal)
// ---------------------------------------------------

NormalMap::NormalMap(const std::string& filepath)
	: m_pSurface{ IMG_Load(filepath.c_str()) }
{
	if (!m_pSurface) std::cout << "IMG load error: " << IMG_GetError() << "\n";
}

void NormalMap::Sample(const FVector2& uv, const Pixel_Info& pixInfo, FVector3& returnNormal) const
{
	if (!m_pSurface)
	{
		std::cout << "Invalid surface created! \n";
		return;
	}

	float x{ uv.x };
	float y{ uv.y }; // uv-coordinate axes are weird!

	//assert(x <= 1.f && y <= 1.f);

	if (x > 1.f || y > 1.f || x < 0.f || y < 0.f)
	{
		return;
	}

	// Convert uv coordinates from [0-1] --> [tex_Width-tex_Height]
	x *= m_pSurface->w;
	y *= m_pSurface->h;

	uint8_t r{};
	uint8_t g{};
	uint8_t b{};

	// Sample color:
	SDL_GetRGB(Utils::GetPixel(m_pSurface, int(x), int(y)), m_pSurface->format, &r, &g, &b);

	assert(r <= 255 && g <= 255 && b <= 255);

	Elite::RGBColor temp{ float(r), float(g), float(b) };

	temp /= 255.f;

	FVector3 finalNormal = FVector3{ 2 * temp.r - 1, 2 * temp.g - 1, 2 * temp.b - 1 };

	FVector3 binormal = Elite::Cross(pixInfo.GetTangent(), pixInfo.GetNormal());
	FMatrix3 tangentSpaceAxis = FMatrix3(pixInfo.GetTangent(), binormal, pixInfo.GetNormal());

	// from [0, 1] to [-1, 1]
	returnNormal = tangentSpaceAxis * finalNormal;
}

NormalMap::~NormalMap()
{
	SDL_FreeSurface(m_pSurface);
}

// ---------------------------------------------------
// Value-Map: Sampling returns average of RGB channels
// ---------------------------------------------------

ValueMap::ValueMap(const std::string& filepath)
	: m_pSurface{ IMG_Load(filepath.c_str()) }
{
	if (!m_pSurface) std::cout << "IMG load error: " << IMG_GetError() << "\n";
}

void ValueMap::Sample(const Elite::FVector2& uv, float& returnValue) const
{
	if (!m_pSurface)
	{
		std::cout << "Invalid surface created! \n";
		return;
	}

	float x{ uv.x };
	float y{ uv.y }; // uv-coordinate axes are weird!

	//assert(x <= 1.f && y <= 1.f);

	if (x > 1.f || y > 1.f || x < 0.f || y < 0.f) return;

	// Convert uv coordinates from [0-1] --> [tex_Width-tex_Height]
	x *= m_pSurface->w;
	y *= m_pSurface->h;

	uint8_t r{};
	uint8_t g{};
	uint8_t b{};

	// Sample color:
	SDL_GetRGB(Utils::GetPixel(m_pSurface, int(x), int(y)), m_pSurface->format, &r, &g, &b);

	assert(r <= 255 && g <= 255 && b <= 255);

	Elite::RGBColor temp{ float(r), float(g), float(b) };

	temp /= 255.f;

	float average = temp.r + temp.g + temp.b;
	average /= 3.f;

	returnValue = average;
}

ValueMap::~ValueMap()
{
	SDL_FreeSurface(m_pSurface);
}