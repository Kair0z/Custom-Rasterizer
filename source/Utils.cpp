#include "pch.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>

Utils::IRect::IRect(const FPoint2& triPoint0, const FPoint2& triPoint1, const FPoint2& triPoint2, const FVector2& boundsX, const FVector2& boundsY)
{
	IRect temp{};

	// From left to right
	int minX = int(std::min({ triPoint0.x, triPoint1.x, triPoint2.x }));
	int maxX = int(std::max({ triPoint0.x, triPoint1.x, triPoint2.x }));

	// From top to down
	int minY = int(std::min({ triPoint0.y, triPoint1.y, triPoint2.y }));
	int maxY = int(std::max({ triPoint0.y, triPoint1.y, triPoint2.y }));

	temp.m_LT.y = minY - 2; // Removing these nasty ol' black lines
	temp.m_LT.x = minX - 2; // Removing these nasty ol' black lines

	temp.m_Width = (maxX - minX) + 2; // Removing these nasty ol' black lines
	temp.m_Height = (maxY - minY) + 2; // Removing these nasty ol' black lines

	if (temp.m_LT.x <= boundsX.x) temp.m_LT.x = int(boundsX.x);
	if (temp.m_LT.x + temp.m_Width >= boundsX.y) temp.m_Width = int(boundsX.y - temp.m_LT.x);

	if (temp.m_LT.y <= boundsY.x) temp.m_LT.y = int(boundsY.x);
	if (temp.m_LT.y + temp.m_Height >= boundsY.y) temp.m_Height = int(boundsY.y - temp.m_LT.y);

	*this = temp;
}

bool Utils::IRect::PointIsInside(int x, int y) const
{
	if (x < m_LT.x || y < m_LT.y) return false;
	if (x > m_LT.x + m_Width || y > m_LT.y + m_Height)
	{
		return false;
	}

	return true;
}

bool Utils::IRect::PointIsInside(const Elite::IPoint2& point) const
{
	return PointIsInside(point.x, point.y);
}

std::vector<std::string> Utils::GetCaptureGroups(const std::string& input, const std::regex& regex)
{
	std::vector<std::string> result;

	if (regex_match(input, regex))
	{
		std::smatch allMatches;

		if (regex_search(input, allMatches, regex))
		{
			for (int i{}; i < int(allMatches.size()); ++i)
			{
				result.push_back(allMatches[i]);
			}
		}
	}

	return result;
}

float Utils::GetRandomFloat(float min, float max)
{
	return float(rand()) / RAND_MAX * (max - min) + min;
}

float Utils::GetRandomFloat(float min, float max, const std::string& inclusivitySign)
{
	if (inclusivitySign == "[]") return GetRandomFloat(min - 0.01f, max);
	if (inclusivitySign == "]]") return GetRandomFloat(min, max - 0.01f);
	if (inclusivitySign == "[[") return GetRandomFloat(min - 0.01f, max - 0.01f);

	else return GetRandomFloat(min, max);
}

int Utils::GetRandomInt(int min, int max)
{
	int randInt{ ((rand() % (max - min)) + min + 1) };
	return randInt;
}

int Utils::GetRandomInt(int min, int max, const std::string& inclusivitySign)
{
	if (inclusivitySign == "[]") return GetRandomInt(min - 1, max);
	if (inclusivitySign == "]]") return GetRandomInt(min, max - 1);
	if (inclusivitySign == "[[") return GetRandomInt(min - 1, max - 1);

	else return GetRandomInt(min, max);
}

bool Utils::GetRandomBool()
{
	//std::cout << GetRandomInt(-1,1) << "\n";
	return bool(GetRandomInt(-1, 1));
}

RGBColor Utils::GetRandomColor(const FVector2& rBounds, const FVector2& gBounds, const FVector2& bBounds)
{
	RGBColor temp{ GetRandomFloat(rBounds.x, rBounds.y), GetRandomFloat(gBounds.x, gBounds.y), GetRandomFloat(bBounds.x, bBounds.y) };
	temp.MaxToOne();

	return temp;
}

FPoint3 Utils::GetRandomFPoint3(const FVector2& xBounds, const FVector2& yBounds, const FVector2& zBounds)
{
	FPoint3 temp{ Utils::GetRandomFloat(xBounds.x, xBounds.y), Utils::GetRandomFloat(yBounds.x, yBounds.y), Utils::GetRandomFloat(zBounds.x, zBounds.y) };
	
	return temp;
}

bool Utils::LineIsInBox(const FPoint3& p0, const FPoint3& p1, const FVector3& boxBounds)
{
	if (PointIsInBox(p0, boxBounds) && PointIsInBox(p1, boxBounds)) return true;

	return false;
}

bool Utils::PointIsInBox(const FPoint3& p, const FVector3& boxBounds)
{
	if (p.x < -boxBounds.x || p.x > boxBounds.x) return false;
	if (p.y < -boxBounds.y || p.y > boxBounds.y) return false;
	if (p.z < -boxBounds.z || p.z > boxBounds.z) return false;

	return true;
}

FVector3 Utils::GetRandomFVector3(const FVector2& xBounds, const FVector2& yBounds, const FVector2& zBounds)
{
	return FVector3{ Utils::GetRandomFloat(xBounds.x, xBounds.y), Utils::GetRandomFloat(yBounds.x, yBounds.y), Utils::GetRandomFloat(zBounds.x, zBounds.y) };
}

std::wstring Utils::StringToWString(const std::string& s)
{
	std::wstring wsTmp(s.begin(), s.end());

	return wsTmp;
}

Uint32 Utils::GetPixel(SDL_Surface* pSurface, int x, int y)
{
	// Accessing pixels solution: http://sdl.beuc.net/sdl.wiki/Pixel_Access
	// In our system every pixel has 4 bytes of data.
	int bpp = pSurface->format->BytesPerPixel;

	Uint8* p = (Uint8*)pSurface->pixels + y * pSurface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16*)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		// In our case
		return *(Uint32*)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void Utils::ChangeConsoleTextColor(TextAttribute attribute)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, uint16_t(attribute));
}

RGBColor Utils::Blend(BlendDesc blendDesc, const RGBColor& source, const RGBColor& dest, float sourceAlpha, float destAlpha)
{
	if (!blendDesc.blendEnabled) return source;

	RGBColor result{};

	switch (blendDesc.blendOperation)
	{
	case BlendDesc::BlendOP::Add:
		result = (source * sourceAlpha) + (dest * destAlpha);
		break;

	case BlendDesc::BlendOP::Subtract:
		result = (source * sourceAlpha) - (dest * destAlpha);
		break;

	case BlendDesc::BlendOP::RevSubtract:
		result = (dest * destAlpha) - (source * sourceAlpha);
		break;

	case BlendDesc::BlendOP::Max:
		// use the color with the biggest alpha value:
		if (sourceAlpha > destAlpha) result = source;
		else result = dest;
		break;

	case BlendDesc::BlendOP::Min:
		if (sourceAlpha > destAlpha) result = dest;
		else result = source;
		break;
	}

	return result;
}