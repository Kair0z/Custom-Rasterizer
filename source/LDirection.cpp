#include "pch.h"
#include "LDirection.h"
#include <cassert>


LDirection::LDirection(const Elite::FVector3& direction, const Elite::RGBColor& color, float intensity, bool isOn)
	: Light(color, intensity , isOn)
	, m_Direction{direction}
{
	Elite::Normalize(m_Direction);
}

Elite::FPoint3 LDirection::GetPos() const
{
	return Elite::FPoint3{ 0.f, 0.f, 0.f };
}

Elite::FVector3 LDirection::GetDirection(const Pixel_Info& hitInfo) const
{
	return m_Direction;
}

Elite::RGBColor LDirection::BiradianceValue(const Pixel_Info& hitInfo) const
{
	RGBColor temp = m_Color * m_Intensity;

	return temp;

	// ------------------
	// This is easier: Directional light has no area! --> Take Pointlight calculation without anything concerning area
	// E == color * Intensity
	// ------------------
}

void LDirection::Update(float elapsedSec)
{
	// DoNothing
}