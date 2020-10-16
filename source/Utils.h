#pragma once
#include <vector>
#include <string>
#include <regex>
#include "EMath.h"
#include "ERGBColor.h"
#include <array>
#include "SDL.h"

using namespace Elite;

namespace Utils
{
	struct IRect
	{
		IRect() = default;
		IRect(const Elite::IPoint2& lb, int width, int height)
			: m_LT{ lb }, m_Width{ width }, m_Height{ height }{}
		IRect(const FPoint2& triPoint0, const FPoint2& triPoint1, const FPoint2& triPoint2, const FVector2& boundsX, const FVector2& boundsY);
		IRect(const FPoint2& triPoint0, const FPoint2& triPoint1, const FPoint2& triPoint2, const FVector2& windowDim)
			:IRect(triPoint0, triPoint1, triPoint2, FVector2{0.f, windowDim.x}, FVector2{0.f, windowDim.y})
		{

		}

		bool PointIsInside(int x, int y) const;
		bool PointIsInside(const Elite::IPoint2& point) const;

		Elite::IPoint2 m_LT;
		int m_Width;
		int m_Height;
	};


	struct Pixel
	{
		int x;
		int y;
	};

	enum class TextAttribute
	{
		Black,
		Blue,
		SwmpGreen,
		BBBlue, // Babyblue
		Red,
		Purple,
		Orange,
		LightGray,
		DarkGray,
		MidBlue,
		Green,
		Cyan,
		LightRed,
		PinkPurple,
		Cream,
		White
	};
	struct BlendDesc
	{
		enum class BlendOP
		{
			Add,
			Subtract,
			RevSubtract,
			Min,
			Max,
			END
		};

		BlendDesc(bool blendEnabled = true, BlendOP operation = BlendOP::Add)
			: blendEnabled{ blendEnabled }
			, blendOperation{ operation }
		{

		}

		bool blendEnabled{ true };
		BlendOP blendOperation{ BlendOP::Add };
	};

	void ChangeConsoleTextColor(TextAttribute attribute = TextAttribute::White);

	std::vector<std::string> GetCaptureGroups(const std::string& input, const std::regex& regex);

	bool LineIsInBox(const FPoint3& p0, const FPoint3& p1, const FVector3& boxBounds);
	bool PointIsInBox(const FPoint3& p, const FVector3& boxBounds);

	float GetRandomFloat(float min, float max);
	float GetRandomFloat(float min, float max, const std::string& inclusivitySign);
	int GetRandomInt(int min, int max);
	int GetRandomInt(int min, int max, const std::string& inclusivitySign);
	FPoint3 GetRandomFPoint3(const FVector2& xBounds, const FVector2& yBounds, const FVector2& zBounds);
	FVector3 GetRandomFVector3(const FVector2& xBounds, const FVector2& yBounds, const FVector2& zBounds);
	RGBColor GetRandomColor(const FVector2& rBounds = FVector2{ 0.f, 255.f }, const FVector2& gBounds = FVector2{ 0.f, 255.f }, const FVector2& bBounds = FVector2{0.f, 255.f});
	bool GetRandomBool();
	std::wstring StringToWString(const std::string& s);

	Uint32 GetPixel(SDL_Surface* pSurface, int x, int y);

	template <typename T>
	T InterpolateValue(const T& val0, const T& val1, const T& val2, float depth, float weights0, float weights1, float weights2)
	{
		return (val0 / depth) * weights0 + (val1 / depth) * weights1 + (val2 / depth) * weights2;
	}

	template <typename T>
	T Lerp(const T& a, const T& b, const float t)
	{
		return a + (b - a) * t;
	}

	RGBColor Blend(BlendDesc blendDesc, const RGBColor& source, const RGBColor& dest, float sourceAlpha, float destAlpha = 1.f);
}
